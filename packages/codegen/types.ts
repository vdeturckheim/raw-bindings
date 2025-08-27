export interface GeneratorOptions {
  outputDir: string;
  packageName: string;
  packageVersion?: string;
  description?: string;
  author?: string;
  license?: string;
  libraryName?: string;
  includePaths?: string[];
  libraryPaths?: string[];
  libraries?: string[];
  frameworks?: string[];
  frameworkPaths?: string[];
  defines?: string[];
  headerIncludePath?: string;
  headerIncludePaths?: string[];
}

export interface GeneratedFiles {
  'src/binding.cpp': string;
  'CMakeLists.txt': string;
  'package.json': string;
  'index.ts': string;
  'test.ts': string;
}

export interface TypeMapping {
  cType: string;
  napiType: string;
  tsType: string;
  needsConversion: boolean;
  conversionTo?: string;
  conversionFrom?: string;
}
