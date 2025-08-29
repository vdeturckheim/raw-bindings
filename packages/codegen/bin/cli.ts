#!/usr/bin/env node

import { resolve, basename } from 'node:path';
import { parseArgs } from 'node:util';
import { parseHeader } from '../lib/h-parser/index.ts';
import { buildIR } from '../lib/ir/builder.ts';
import { generateProjectFromIR } from '../lib/newgen/generate.ts';

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
Usage: codegen [options] <header-file>

Generate Node.js bindings from C/C++ header files (template-based)

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

  # Generate bindings for a simple C library
  codegen -n mylib -o ./mylib-binding /usr/include/mylib.h
`);
}

async function main() {
  if (values.help || positionals.length === 0) {
    showHelp();
    process.exit(values.help ? 0 : 1);
  }

  const headerPath = resolve(positionals[0] as string);

  if (!values.name) {
    console.error('Error: Package name is required (use -n or --name)');
    process.exit(1);
  }

  const outputDir = resolve(values.output as string);
  const includePaths = (values.include as string[]) || [];
  const headerInclude = (values['header-include'] as string) || basename(headerPath);

  const ast = parseHeader(headerPath, {
    language: 'c',
    includeDocumentation: true,
    detailedProcessing: true,
    includePaths,
  });
  const ir = buildIR(ast, { moduleName: values.name as string });
  await generateProjectFromIR(ir, {
    outputDir,
    packageName: values.name as string,
    packageVersion: (values.version as string) || '0.0.1',
    libraryName: (values.library as string) || (values.name as string),
    includes: [headerInclude],
  });
}

main().catch(console.error);
