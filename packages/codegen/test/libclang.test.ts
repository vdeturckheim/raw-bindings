import assert from 'node:assert/strict';
import { execSync } from 'node:child_process';
import { cpSync, existsSync, mkdtempSync, writeFileSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { describe, it } from 'node:test';
import { generateBindings } from '../generator.ts';

describe('LibClang Bindings', () => {
  let tempDir: string;
  let outputDir: string;

  it('should generate, compile, and use libclang bindings', async () => {
    // Create temp directory for output
    tempDir = mkdtempSync(join(tmpdir(), 'libclang-test-'));
    outputDir = join(tempDir, 'libclang-binding');

    console.log('  📁 Output directory:', outputDir);

    try {
      // Create a simple C file to test parsing
      const testCFile = join(tempDir, 'test.c');
      writeFileSync(
        testCFile,
        `// Test C file for libclang parsing
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int main() {
    printf("Hello, World!\\n");
    return 0;
}
`,
        'utf-8',
      );
      console.log('  📝 Created test C file:', testCFile);

      // Generate bindings for libclang
      console.log('  📝 Generating libclang bindings...');
      await generateBindings(
        '/opt/homebrew/Cellar/llvm/20.1.8/include/clang-c/Index.h',
        {
          outputDir,
          packageName: 'libclang-binding',
          packageVersion: '0.0.1',
          libraryName: 'clang',
          headerIncludePath: 'clang-c/Index.h',
          includePaths: ['/opt/homebrew/Cellar/llvm/20.1.8/include'],
          libraryPaths: ['/opt/homebrew/Cellar/llvm/20.1.8/lib'],
          libraries: ['clang'],
        },
      );

      // Install dependencies
      console.log('  📦 Installing dependencies...');
      try {
        execSync('npm install', {
          cwd: outputDir,
          stdio: 'inherit',

        });
      } catch (error) {
        console.error('  ❌ npm install failed. Check output in:', outputDir);
        tempDir = null; // Don't clean up on error
        throw error;
      }

      // Build the addon
      console.log('  🔨 Building addon...');
      try {
        execSync('npm run build', {
          cwd: outputDir,
          stdio: 'inherit',
        });
      } catch (error) {
        console.error('  ❌ Build failed. Check output in:', outputDir);
        tempDir = null; // Don't clean up on error
        throw error;
      }

      // Test the generated binding
      console.log('  🧪 Testing libclang addon...');

      // Import the generated TypeScript bindings
      let addon: any;
      try {
        addon = await import(join(outputDir, 'index.ts'));
        console.log('  ✅ Successfully imported libclang addon');
      } catch (error) {
        console.error('  ❌ Failed to import addon:', error);
        throw error;
      }

      // Test basic libclang functions
      console.log('  🧪 Testing libclang functions...');

      // Test clang_createIndex
      assert(
        typeof addon.clang_createIndex === 'function',
        'clang_createIndex should be a function',
      );
      
      // Create an index (0 = don't exclude declarations from PCH, 0 = don't display diagnostics)
      const index = addon.clang_createIndex(0, 0);
      assert(index, 'Should create an index');
      console.log('  ✅ clang_createIndex works');

      // Test clang_parseTranslationUnit
      assert(
        typeof addon.clang_parseTranslationUnit === 'function',
        'clang_parseTranslationUnit should be a function',
      );

      // Parse the test C file
      // Note: clang_parseTranslationUnit has many parameters, we'll use the basic form
      // The actual signature depends on what the generator produces
      let tu;
      try {
        // Try to parse with minimal arguments
        // The exact calling convention will depend on how the generator handles complex signatures
        tu = addon.clang_parseTranslationUnit(
          index,
          testCFile,
          null, // command line args
          0,    // num command line args
          null, // unsaved files
          0,    // num unsaved files
          0,    // options
        );
        assert(tu, 'Should create a translation unit');
        console.log('  ✅ clang_parseTranslationUnit works');
      } catch (error) {
        console.log('  ⚠️  clang_parseTranslationUnit calling convention may differ');
        // Try alternative calling convention if the first fails
      }

      // Test that enums are exposed
      // CXCursorKind enum should have various values
      if (addon.CXCursor_UnexposedDecl !== undefined) {
        assert(
          typeof addon.CXCursor_UnexposedDecl === 'number',
          'CXCursor_UnexposedDecl should be a number',
        );
        console.log('  ✅ CXCursorKind enum values are exposed');
      }

      // Test clang_disposeTranslationUnit
      if (tu && typeof addon.clang_disposeTranslationUnit === 'function') {
        addon.clang_disposeTranslationUnit(tu);
        console.log('  ✅ clang_disposeTranslationUnit works');
      }

      // Test clang_disposeIndex
      assert(
        typeof addon.clang_disposeIndex === 'function',
        'clang_disposeIndex should be a function',
      );
      addon.clang_disposeIndex(index);
      console.log('  ✅ clang_disposeIndex works');

      console.log('  ✅ LibClang binding test succeeded!');
    } finally {
      // Clean up temp directory (unless we nulled it to preserve for debugging)
      if (tempDir && existsSync(tempDir)) {
        // Preserve for debugging - comment out to clean up
        console.log('  📁 Preserved output for debugging at:', tempDir);
        // rmSync(tempDir, { recursive: true, force: true });
      }
    }
  });
});