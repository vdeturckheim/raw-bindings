import { describe, it } from 'node:test';
import assert from 'node:assert/strict';
import { generateBindings } from '../generator.ts';
import { execSync } from 'node:child_process';
import { mkdtempSync, rmSync, cpSync, existsSync } from 'node:fs';
import { join } from 'node:path';
import { tmpdir } from 'node:os';

describe('MathLib Bindings', () => {
  let tempDir: string;
  let outputDir: string;
  
  it('should generate, compile, and use mathlib bindings', async () => {
    // Create temp directory for output
    tempDir = mkdtempSync(join(tmpdir(), 'mathlib-test-'));
    outputDir = join(tempDir, 'mathlib-binding');
    
    console.log('  📁 Output directory:', outputDir);
    
    try {
      // Copy mathlib source files to temp dir
      const fixturesDir = join(import.meta.dirname, 'fixtures');
      cpSync(join(fixturesDir, 'mathlib.h'), join(tempDir, 'mathlib.h'));
      cpSync(join(fixturesDir, 'mathlib.cpp'), join(tempDir, 'mathlib.cpp'));
      
      // Generate bindings
      console.log('  📝 Generating bindings...');
      await generateBindings(join(tempDir, 'mathlib.h'), {
        outputDir,
        packageName: 'mathlib-binding',
        packageVersion: '0.0.1',
        libraryName: 'mathlib',
        headerIncludePath: 'mathlib.h',
      });
      
      // Copy the mathlib source to the generated binding directory
      cpSync(join(tempDir, 'mathlib.cpp'), join(outputDir, 'src', 'mathlib.cpp'));
      cpSync(join(tempDir, 'mathlib.h'), join(outputDir, 'src', 'mathlib.h'));
      
      // Update CMakeLists.txt to include mathlib.cpp
      const cmakeFile = join(outputDir, 'CMakeLists.txt');
      let cmakeContent = await import('node:fs').then(fs => 
        fs.readFileSync(cmakeFile, 'utf-8')
      );
      
      // Add mathlib.h include directory
      cmakeContent = cmakeContent.replace(
        'file(GLOB SOURCE_FILES "src/*.cpp" "src/*.cc")',
        'file(GLOB SOURCE_FILES "src/*.cpp" "src/*.cc")\ninclude_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)'
      );
      
      await import('node:fs').then(fs => 
        fs.writeFileSync(cmakeFile, cmakeContent)
      );
      
      // Install dependencies
      console.log('  📦 Installing dependencies...');
      try {
        execSync('npm install', { 
          cwd: outputDir, 
          stdio: 'inherit'  // Show output to see errors
        });
      } catch (error) {
        console.error('  ❌ Build failed. Check output in:', outputDir);
        // Don't clean up on error so we can debug
        tempDir = null;
        throw error;
      }
      
      // Build the addon
      console.log('  🔨 Building addon...');
      execSync('npm run build', { 
        cwd: outputDir, 
        stdio: 'inherit'  // Show output
      });
      
      // Test the generated binding
      console.log('  🧪 Testing addon...');
      
      // Import the generated TypeScript bindings
      let addon: any;
      try {
        addon = await import(join(outputDir, 'index.ts'));
        console.log('  ✅ Successfully imported addon');
      } catch (error) {
        console.error('  ❌ Failed to import addon:', error);
        throw error;
      }
      
      // Test basic arithmetic
      try {
        assert.strictEqual(addon.add(2, 3), 5, 'add should work');
        console.log('  ✅ add works');
      } catch (error) {
        console.error('  ❌ add failed:', error);
        throw error;
      }
      assert.strictEqual(addon.subtract(10, 4), 6, 'subtract should work');
      assert.strictEqual(addon.multiply(3, 4), 12, 'multiply should work');
      assert.strictEqual(addon.divide(10, 2), 5, 'divide should work');
      console.log('  ✅ Basic arithmetic works');
      
      // Test factorial
      assert.strictEqual(addon.factorial(5), 120, 'factorial should work');
      console.log('  ✅ Factorial works');
      
      // Test power
      assert.strictEqual(addon.power(2, 3), 8, 'power should work');
      console.log('  ✅ Power works');
      
      // Test enum
      assert.strictEqual(addon.OP_ADD, 0, 'enum OP_ADD should be 0');
      assert.strictEqual(addon.OP_MULTIPLY, 2, 'enum OP_MULTIPLY should be 2');
      
      // Test calculate with enum
      assert.strictEqual(addon.calculate(10, 5, addon.OP_ADD), 15, 'calculate with OP_ADD');
      assert.strictEqual(addon.calculate(10, 5, addon.OP_SUBTRACT), 5, 'calculate with OP_SUBTRACT');
      
      // Test Point2D struct
      const p1 = addon.create_Point2D({ x: 3.0, y: 4.0 });
      const p2 = addon.create_Point2D({ x: 0.0, y: 0.0 });
      
      // Test create_point function
      const p3 = addon.create_point(1.0, 2.0);
      
      // Test distance function (uses struct parameters)
      const dist = addon.distance(p1, p2);
      assert.strictEqual(dist, 5, 'distance should work');
      
      // Test add_points function (returns struct)
      const p4 = addon.add_points(p1, p2);
      assert.strictEqual(addon.get_Point2D_field(p4, 'x'), 3.0, 'add_points x coordinate');
      assert.strictEqual(addon.get_Point2D_field(p4, 'y'), 4.0, 'add_points y coordinate');
      
      console.log('  ✅ MathLib binding test succeeded!');
      
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