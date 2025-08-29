import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import { join } from 'node:path';
import { parseHeader } from '../../lib/h-parser/index.ts';
import { buildIR } from '../../lib/ir/builder.ts';
import { renderTsIndexFromIR } from '../../lib/template-gen.ts';

describe('templates: struct classes (TS)', () => {
  it('renders classes and init interfaces for mathlib structs', () => {
    const headerPath = join(import.meta.dirname, '..', 'testdata', 'mathlib.h');
    const ast = parseHeader(headerPath, { language: 'c', includeDocumentation: true, detailedProcessing: true });
    const ir = buildIR(ast, { moduleName: 'mathlib' });
    const output = renderTsIndexFromIR(ir);

    // Basic includes
    assert.match(output, /node-gyp-build/);
    assert.match(output, /const addon = nodeGypBuild\(import.meta.dirname\)/);

    // Point2D class
    assert.match(output, /export interface Point2DInit\s*\{/);
    assert.match(output, /export class Point2D\s*\{/);
    assert.match(output, /constructor\(init\?: Point2DInit\)/);
    assert.match(output, /this\._handle = addon\.create_Point2D\(init\)/);
    assert.match(output, /get x\(\): number \{/);
    assert.match(output, /return addon\.get_Point2D_field\(this\._handle, 'x'\)/);
    assert.match(output, /get y\(\): number \{/);

    // Circle class and nested getters
    assert.match(output, /export interface CircleInit\s*\{/);
    assert.match(output, /export class Circle\s*\{/);
    assert.match(output, /get radius\(\): number/);

    // DataArray string array and numeric array should map types
    assert.match(output, /export interface DataArrayInit/);
    assert.match(output, /name\?: string;/);
    assert.match(output, /(values\?: unknown\[\];|values\?: number\[\];)/);

    // Opaque context shouldn't produce a struct class, but typedef to pointer won't be in structs
  });
});

