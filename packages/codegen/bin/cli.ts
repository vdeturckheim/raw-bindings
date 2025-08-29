#!/usr/bin/env node

import { resolve } from 'node:path';
import { parseArgs } from 'node:util';
import { generateBindings } from '../lib/generator.ts';
import type { GeneratorOptions } from '../types.ts';

// Parse command-line arguments
const { values, positionals } = parseArgs({
  args: process.argv.slice(2),
  options: {
    output: {
      type: 'string',
      short: 'o',
      default: './generated-binding',
    },
    name: {
      type: 'string',
      short: 'n',
    },
    version: {
      type: 'string',
      short: 'v',
      default: '0.0.1',
    },
    library: {
      type: 'string',
      short: 'l',
    },
    include: {
      type: 'string',
      short: 'I',
      multiple: true,
    },
    'library-path': {
      type: 'string',
      short: 'L',
      multiple: true,
    },
    framework: {
      type: 'string',
      short: 'F',
      multiple: true,
    },
    'framework-path': {
      type: 'string',
      multiple: true,
    },
    'header-include': {
      type: 'string',
      short: 'H',
    },
    define: {
      type: 'string',
      short: 'D',
      multiple: true,
    },
    help: {
      type: 'boolean',
      short: 'h',
    },
  },
  allowPositionals: true,
});

function showHelp() {
  console.log(`
Usage: codegen [options] <header-file> [additional-header-files...]

Generate Node.js bindings from C/C++/Objective-C header files

Options:
  -h, --help              Show this help message
  -o, --output <dir>      Output directory (default: ./generated-binding)
  -n, --name <name>       Package name (required)
  -v, --version <ver>     Package version (default: 0.0.1)
  -l, --library <lib>     Library to link against
  -I, --include <path>    Include directory (can be specified multiple times)
  -L, --library-path <p>  Library search path (can be specified multiple times)
  -F, --framework <f>     Framework to link (macOS) (can be specified multiple times)
  --framework-path <p>    Framework search path (macOS) (can be specified multiple times)
  -H, --header-include <p> Path to use in #include directive (e.g., clang-c/Index.h)
  -D, --define <macro>    Define preprocessor macro (can be specified multiple times)

Examples:
  # Generate bindings for a simple C library
  codegen -n mylib -o ./mylib-binding /usr/include/mylib.h

  # Generate bindings for libclang with multiple headers
  codegen -n clang-bindings -l clang -I /opt/homebrew/opt/llvm/include -L /opt/homebrew/opt/llvm/lib \\
    /opt/homebrew/opt/llvm/include/clang-c/Index.h \\
    /opt/homebrew/opt/llvm/include/clang-c/CXString.h \\
    /opt/homebrew/opt/llvm/include/clang-c/CXSourceLocation.h

  # Generate bindings for an Objective-C framework
  codegen -n myframework -F CoreFoundation --framework-path /System/Library/Frameworks /path/to/header.h
`);
}

async function main() {
  if (values.help || positionals.length === 0) {
    showHelp();
    process.exit(values.help ? 0 : 1);
  }

  const headerPaths = positionals.map(path => resolve(path as string));

  if (!values.name) {
    console.error('Error: Package name is required (use -n or --name)');
    process.exit(1);
  }

  const options: GeneratorOptions = {
    outputDir: resolve(values.output as string),
    packageName: values.name as string,
    packageVersion: values.version as string,
    libraryName: values.library as string,
    includePaths: (values.include as string[]) || [],
    libraryPaths: (values['library-path'] as string[]) || [],
    libraries: values.library ? [values.library as string] : [],
    frameworks: (values.framework as string[]) || [],
    frameworkPaths: (values['framework-path'] as string[]) || [],
    defines: (values.define as string[]) || [],
    headerIncludePath: values['header-include'] as string,
  };

  try {
    await generateBindings(headerPaths, options);
  } catch (error) {
    console.error('Error generating bindings:', error);
    process.exit(1);
  }
}

main().catch(console.error);
