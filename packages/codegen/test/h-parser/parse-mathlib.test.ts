import assert from 'node:assert/strict';
import { join } from 'node:path';
import { describe, it } from 'node:test';
import { parseHeader } from '../../lib/h-parser/index.ts';

describe('h-parser: parseHeader basic C header', () => {
  it('parses mathlib.h with libclang', async () => {
    const headerPath = join(import.meta.dirname, '..', 'testdata', 'mathlib.h');

    const ast = parseHeader(headerPath, {
      language: 'c',
      includeDocumentation: true,
      detailedProcessing: true,
    });

    assert.ok(ast, 'AST returned');
    assert.ok(Array.isArray(ast.functions), 'functions array present');
    const fnNames = new Set((ast.functions || []).map((f) => f.name));
    assert.ok(fnNames.has('add'), 'contains function add');
    assert.ok(
      fnNames.has('create_context'),
      'contains function create_context',
    );
  });
});
