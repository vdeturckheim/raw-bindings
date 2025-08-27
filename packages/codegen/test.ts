import assert from 'node:assert/strict';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { rm, readFile } from 'node:fs/promises';
import { describe, it, beforeEach, afterEach } from 'node:test';
import { generateBindings } from './generator.ts';
import type { GeneratorOptions } from './types.ts';

describe('codegen', () => {
  let testOutputDir: string;

  beforeEach(async () => {
    // Create a unique temp directory for each test
    testOutputDir = join(tmpdir(), `codegen-test-${Date.now()}`);
  });

  afterEach(async () => {
    // Clean up temp directory
    try {
      await rm(testOutputDir, { recursive: true, force: true });
    } catch {
      // Ignore cleanup errors
    }
  });

  it('should generate bindings for a subset of libclang', async (t) => {
    // Skip if libclang headers are not available
    try {
      await readFile('/opt/homebrew/opt/llvm/include/clang-c/CXString.h');
    } catch {
      t.skip('libclang headers not found');
      return;
    }

    // Create a simplified header that includes just basic clang types
    const testHeader = '/opt/homebrew/opt/llvm/include/clang-c/CXString.h';
    
    const options: GeneratorOptions = {
      outputDir: testOutputDir,
      packageName: 'clang-cxstring-test',
      packageVersion: '0.0.1',
      description: 'Test bindings for clang CXString',
      libraryName: 'clang',
      includePaths: ['/opt/homebrew/opt/llvm/include'],
      libraryPaths: ['/opt/homebrew/opt/llvm/lib'],
      libraries: ['clang']
    };

    await generateBindings(testHeader, options);

    // Verify files were generated
    const packageJson = await readFile(join(testOutputDir, 'package.json'), 'utf-8');
    assert.ok(packageJson.includes('clang-cxstring-test'));

    const cppBinding = await readFile(join(testOutputDir, 'src/binding.cpp'), 'utf-8');
    assert.ok(cppBinding.includes('#include <napi.h>'));
    assert.ok(cppBinding.includes('#include <CXString.h>'));

    const tsBinding = await readFile(join(testOutputDir, 'index.ts'), 'utf-8');
    assert.ok(tsBinding.includes('node-gyp-build'));

    const cmakeLists = await readFile(join(testOutputDir, 'CMakeLists.txt'), 'utf-8');
    assert.ok(cmakeLists.includes('project(clang-cxstring-test)'));
    assert.ok(cmakeLists.includes('find_library(CLANG_LIB clang'));

    const testFile = await readFile(join(testOutputDir, 'test.ts'), 'utf-8');
    assert.ok(testFile.includes('clang-cxstring-test bindings'));
  });

  it('should generate bindings with enum constants', async (t) => {
    // Skip if libclang headers are not available
    try {
      await readFile('/opt/homebrew/opt/llvm/include/clang-c/Index.h');
    } catch {
      t.skip('libclang headers not found');
      return;
    }

    // Use the main libclang header for a more complete test
    const testHeader = '/opt/homebrew/opt/llvm/include/clang-c/Index.h';
    
    const options: GeneratorOptions = {
      outputDir: testOutputDir,
      packageName: 'clang-bindings-full',
      packageVersion: '0.0.1',
      description: 'Full test bindings for libclang',
      libraryName: 'clang',
      includePaths: ['/opt/homebrew/opt/llvm/include'],
      libraryPaths: ['/opt/homebrew/opt/llvm/lib'],
      libraries: ['clang']
    };

    // This test will parse the full libclang header and generate bindings
    // It demonstrates that our codegen can handle complex real-world headers
    await generateBindings(testHeader, options);

    // Verify the generated TypeScript includes enums
    const tsBinding = await readFile(join(testOutputDir, 'index.ts'), 'utf-8');
    
    // Check for enum exports (libclang has many enums like CXCursorKind)
    assert.ok(tsBinding.includes('export enum'));
    
    // Check for function exports
    assert.ok(tsBinding.includes('export function'));

    // Verify C++ binding includes enum constants
    const cppBinding = await readFile(join(testOutputDir, 'src/binding.cpp'), 'utf-8');
    assert.ok(cppBinding.includes('// Enum constants'));
  });
});

describe('codegen type mapper', () => {
  it('should handle basic C types', async () => {
    const { TypeMapper } = await import('./type-mapper.ts');
    
    assert.equal(TypeMapper.getTsType('int'), 'number');
    assert.equal(TypeMapper.getTsType('void'), 'void');
    assert.equal(TypeMapper.getTsType('const char *'), 'string');
    assert.equal(TypeMapper.getTsType('bool'), 'boolean');
    assert.equal(TypeMapper.getTsType('double'), 'number');
    assert.equal(TypeMapper.getTsType('unsigned long long'), 'bigint');
  });

  it('should detect pointer types', async () => {
    const { TypeMapper } = await import('./type-mapper.ts');
    
    assert.equal(TypeMapper.isPointerType('void *'), true);
    assert.equal(TypeMapper.isPointerType('int *'), true);
    assert.equal(TypeMapper.isPointerType('const char *'), false); // String type
    assert.equal(TypeMapper.isPointerType('int'), false);
  });

  it('should detect string types', async () => {
    const { TypeMapper } = await import('./type-mapper.ts');
    
    assert.equal(TypeMapper.isStringType('const char *'), true);
    assert.equal(TypeMapper.isStringType('char *'), true);
    assert.equal(TypeMapper.isStringType('void *'), false);
    assert.equal(TypeMapper.isStringType('int'), false);
  });

  it('should sanitize identifiers', async () => {
    const { TypeMapper } = await import('./type-mapper.ts');
    
    assert.equal(TypeMapper.sanitizeIdentifier('valid_name'), 'valid_name');
    assert.equal(TypeMapper.sanitizeIdentifier('invalid-name'), 'invalid_name');
    assert.equal(TypeMapper.sanitizeIdentifier('123invalid'), '123invalid');
    assert.equal(TypeMapper.sanitizeIdentifier('has spaces'), 'has_spaces');
  });
});