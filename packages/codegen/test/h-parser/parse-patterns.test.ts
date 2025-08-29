import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import { join } from 'node:path';
import { parseHeader } from '../../lib/h-parser/index.ts';

 

describe('h-parser: parseHeader pattern-heavy header', () => {
  it('parses patterns.h with libclang', async () => {
    const headerPath = join(import.meta.dirname, '..', 'testdata', 'patterns.h');

    const ast = parseHeader(headerPath, {
      language: 'c',
      includeDocumentation: true,
      detailedProcessing: true,
    });

    assert.ok(ast, 'AST returned');
    const names = new Set((ast.functions || []).map(f => f.name));
    for (const fn of ['get_value', 'write_data', 'set_name', 'set_callback', 'create_handle', 'destroy_handle']) {
      assert.ok(names.has(fn), `contains function ${fn}`);
    }
  });
});
