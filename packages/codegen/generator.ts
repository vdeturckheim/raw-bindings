import { mkdir, writeFile } from 'node:fs/promises';
import { dirname, join } from 'node:path';
import { type HeaderAST, parseHeader } from './h-parser/index.ts';
import { BuildGenerator } from './generators/build-generator.ts';
import { CppGenerator } from './generators/cpp-generator.ts';
import { TsGenerator } from './generators/ts-generator.ts';
import type { GeneratedFiles, GeneratorOptions } from './types.ts';

function combineASTs(asts: HeaderAST[], headerPaths: string[]): HeaderAST {
  // Use the first AST as base and merge others into it
  const combined: HeaderAST = {
    header: headerPaths.map(path => path.split('/').pop() || '').join('+'),
    interfaces: [],
    protocols: [],
    enums: [],
    structs: [],
    typedefs: [],
    functions: [],
    classes: [],
    // Convenience aliases
    objc_interfaces: [],
    objc_protocols: [],
    objc_categories: [],
  };

  // Combine all declarations from all ASTs
  for (const ast of asts) {
    combined.interfaces.push(...(ast.interfaces || []));
    combined.protocols.push(...(ast.protocols || []));
    combined.enums.push(...(ast.enums || []));
    combined.structs.push(...(ast.structs || []));
    combined.typedefs.push(...(ast.typedefs || []));
    combined.functions.push(...(ast.functions || []));
    combined.classes.push(...(ast.classes || []));
    combined.objc_interfaces.push(...(ast.objc_interfaces || []));
    combined.objc_protocols.push(...(ast.objc_protocols || []));
    combined.objc_categories.push(...(ast.objc_categories || []));
  }

  console.log(`🔗 Combined ${asts.length} ASTs:`);
  console.log(`   Functions: ${combined.functions.length}`);
  console.log(`   Enums: ${combined.enums.length}`);
  console.log(`   Structs: ${combined.structs.length}`);
  console.log(`   Typedefs: ${combined.typedefs.length}`);
  console.log(`   Interfaces: ${combined.interfaces.length}`);

  return combined;
}

export async function generateBindings(
  headerPaths: string[],
  options: GeneratorOptions,
): Promise<void> {
  // Parse options for header files
  const parseOptions = {
    language: options.defines?.includes('__cplusplus') ? 'c++' : 'c' as const,
    includePaths: options.includePaths || [],
    frameworkPaths: options.frameworkPaths || [],
    frameworks: options.frameworks || [],
    defines: options.defines || [],
    includeDocumentation: true,
    detailedProcessing: true,
  };

  // Parse all header files and combine ASTs
  let combinedAST: HeaderAST;

  if (headerPaths.length === 1) {
    // Single header file - parse normally
    combinedAST = parseHeader(headerPaths[0], parseOptions);
  } else {
    // Multiple header files - parse each and combine ASTs
    console.log(`📚 Parsing ${headerPaths.length} header files...`);
    
    const asts = headerPaths.map((headerPath, index) => {
      console.log(`  ${index + 1}. ${headerPath}`);
      return parseHeader(headerPath, parseOptions);
    });

    // Combine all ASTs into one
    combinedAST = combineASTs(asts, headerPaths);
  }

  // Extract header include paths from the actual header paths
  const headerIncludePaths = headerPaths.map(headerPath => {
    // Convert absolute path to include path format
    // e.g., "/opt/homebrew/include/clang-c/Index.h" -> "clang-c/Index.h"
    if (options.includePaths) {
      for (const includePath of options.includePaths) {
        if (headerPath.startsWith(includePath + '/')) {
          return headerPath.slice(includePath.length + 1);
        }
      }
    }
    // Fallback - use just the filename
    return headerPath.split('/').pop() || headerPath;
  });

  // Create updated options with header include paths
  const updatedOptions: GeneratorOptions = {
    ...options,
    headerIncludePath: headerIncludePaths[0], // Keep backward compatibility 
    headerIncludePaths: headerIncludePaths,
  };

  // Generate all files (deterministic ordering)
  const files = generateFilesFromAST(combinedAST, updatedOptions);

  // Write all files
  await writeGeneratedFiles(options.outputDir, files);

  console.log(`✅ Generated bindings for ${headerPaths.length} header file(s)`);
  headerPaths.forEach(path => console.log(`   - ${path}`));
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
    options.headerIncludePaths,
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
