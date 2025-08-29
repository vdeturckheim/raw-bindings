import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import { mkdtempSync, mkdirSync } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { cpSync, writeFileSync, existsSync } from 'node:fs';
import { execSync } from 'node:child_process';
import { parseHeader } from '../../lib/h-parser/index.ts';
import { buildIR } from '../../lib/ir/builder.ts';
import { renderTsIndexFromIR, renderCppBindingFromIR, renderCMakeLists, renderPackageJson } from '../../lib/template-gen.ts';

describe('templates: e2e generate + build mathlib (template-based)', () => {
  it('generates and builds addon using templates (minimal patterns)', async () => {
    // Prepare temp workspace
    const dir = mkdtempSync(join(tmpdir(), 'tmpl-mathlib-'));
    const outDir = join(dir, 'binding');
    const srcDir = join(outDir, 'src');
    mkdirSync(srcDir, { recursive: true });
    const testdataDir = join(import.meta.dirname, '..', 'testdata');

    // Parse + IR
    const headerPath = join(testdataDir, 'mathlib.h');
    const ast = parseHeader(headerPath, { language: 'c', includeDocumentation: true, detailedProcessing: true });
    const ir = buildIR(ast, { moduleName: 'mathlib' });

    // Render files
    const indexTs = renderTsIndexFromIR(ir);
    const bindingCpp = renderCppBindingFromIR(ir, ['mathlib.h']);
    const cmake = renderCMakeLists({ packageName: 'mathlib-binding' });
    const pkg = renderPackageJson({ packageName: 'mathlib-binding', packageVersion: '0.0.1', libraryName: 'mathlib' });

    // Write files
    writeFileSync(join(outDir, 'index.ts'), indexTs);
    writeFileSync(join(outDir, 'CMakeLists.txt'), cmake);
    writeFileSync(join(outDir, 'package.json'), pkg);
    // src
    writeFileSync(join(srcDir, 'binding.cpp'), bindingCpp);
    // copy mathlib sources
    cpSync(join(testdataDir, 'mathlib.h'), join(srcDir, 'mathlib.h'));
    cpSync(join(testdataDir, 'mathlib.cpp'), join(srcDir, 'mathlib.cpp'));

    // Install and build
    execSync('npm install', { cwd: outDir, stdio: 'inherit' });
    execSync('npm run build', { cwd: outDir, stdio: 'inherit' });

    // Import TS wrapper and test simple functions
    const addon: any = await import(join(outDir, 'index.ts'));
    assert.equal(addon.add(2, 3), 5);
    assert.equal(addon.factorial(5), 120);

    // Structs
    const p1 = new addon.Point2D({ x: 3.0, y: 4.0 });
    const p2 = new addon.Point2D({ x: 0.0, y: 0.0 });
    const dist = addon.distance(p1, p2);
    assert.equal(dist, 5);

    const p3 = addon.add_points(p1, p2);
    const obj = p3.toObject();
    assert.equal(obj.x, 3);
    assert.equal(obj.y, 4);

    // Enum usage (MathOperation)
    if (typeof addon.calculate === 'function') {
      const sum = addon.calculate(10, 5, 0 /* OP_ADD */);
      assert.equal(sum, 15);
    }
  });
});
