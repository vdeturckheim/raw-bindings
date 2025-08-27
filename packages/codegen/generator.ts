import { mkdir, writeFile } from 'node:fs/promises';
import { dirname, join } from 'node:path';
import { type HeaderAST, parseHeader } from 'h-parser';
import { BuildGenerator } from './generators/build-generator.ts';
import { CppGenerator } from './generators/cpp-generator.ts';
import { TsGenerator } from './generators/ts-generator.ts';
import type { GeneratedFiles, GeneratorOptions } from './types.ts';

export async function generateBindings(
  headerPath: string,
  options: GeneratorOptions,
): Promise<void> {
  // Parse the header file
  const ast = parseHeader(headerPath, {
    language: options.defines?.includes('__cplusplus') ? 'c++' : 'c',
    includePaths: options.includePaths || [],
    frameworkPaths: options.frameworkPaths || [],
    frameworks: options.frameworks || [],
    defines: options.defines || [],
    includeDocumentation: true,
    detailedProcessing: true,
  });

  // Generate all files
  const files = generateFilesFromAST(ast, options);

  // Write all files
  await writeGeneratedFiles(options.outputDir, files);

  console.log(`✅ Generated bindings for ${headerPath}`);
  console.log(`   Output directory: ${options.outputDir}`);
  console.log(`   Package name: ${options.packageName}`);
  console.log('');
  console.log('To build the generated addon:');
  console.log(`  cd ${options.outputDir}`);
  console.log('  npm install');
  console.log('  npm run build');
  console.log('');
  console.log('To test the generated addon:');
  console.log('  npm test');
}

export async function generateBindingsFromJSON(
  astJson: string,
  options: GeneratorOptions,
): Promise<void> {
  const ast = JSON.parse(astJson) as HeaderAST;

  // Generate all files
  const files = generateFilesFromAST(ast, options);

  // Write all files
  await writeGeneratedFiles(options.outputDir, files);

  console.log(`✅ Generated bindings from AST`);
  console.log(`   Output directory: ${options.outputDir}`);
  console.log(`   Package name: ${options.packageName}`);
}

function generateFilesFromAST(
  ast: HeaderAST,
  options: GeneratorOptions,
): GeneratedFiles {
  // Initialize generators
  const cppGen = new CppGenerator(
    ast,
    options.packageName,
    options.headerIncludePath,
  );
  const tsGen = new TsGenerator(ast, options.packageName);
  const buildGen = new BuildGenerator(options);

  // Generate all files
  const files: GeneratedFiles = {
    'src/binding.cpp': cppGen.generate(),
    'CMakeLists.txt': buildGen.generateCMakeLists(),
    'package.json': buildGen.generatePackageJson(),
    'index.ts': tsGen.generate(),
    'test.ts': buildGen.generateTestFile(),
  };

  return files;
}

async function writeGeneratedFiles(
  outputDir: string,
  files: GeneratedFiles,
): Promise<void> {
  // Create output directory
  await mkdir(outputDir, { recursive: true });

  // Write each file
  for (const [relativePath, content] of Object.entries(files)) {
    const fullPath = join(outputDir, relativePath);

    // Ensure directory exists
    await mkdir(dirname(fullPath), { recursive: true });

    // Write file
    await writeFile(fullPath, content, 'utf-8');
    console.log(`  📝 ${relativePath}`);
  }
}
