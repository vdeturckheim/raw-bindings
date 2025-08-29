import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import type { HeaderAST } from '../../lib/h-parser/types.ts';
import { buildIR } from '../../lib/ir/builder.ts';

describe('IR Builder and pattern detection', () => {
  it('builds IR from a minimal synthetic AST and detects patterns', () => {
    const ast: HeaderAST = {
      header: 'test.h',
      interfaces: [],
      protocols: [],
      enums: [],
      structs: [],
      typedefs: [],
      functions: [
        {
          name: 'create_context',
          return: { spelling: 'OpaqueContext *' },
          params: [],
        },
        {
          name: 'destroy_context',
          return: { spelling: 'void' },
          params: [
            { name: 'ctx', type: { spelling: 'OpaqueContext *' } },
          ],
        },
        {
          name: 'integer_add',
          return: { spelling: 'int' },
          params: [
            { name: 'a', type: { spelling: 'int' } },
            { name: 'b', type: { spelling: 'int' } },
          ],
        },
      ],
      classes: [],
      objc_interfaces: [],
      objc_protocols: [],
      objc_categories: [],
    };

    const mod = buildIR(ast, { moduleName: 'synthetic' });

    const createFn = mod.functions.find(f => f.name === 'create_context');
    assert.ok(createFn, 'create_context function present');
    assert.ok(createFn!.patterns?.some(p => p.kind === 'create-function' && p.resourceType === 'OpaqueContext'));
    assert.equal(createFn!.semantics?.returnsOwned, true);
    assert.equal(createFn!.semantics?.freedBy, 'destroy_context');

    const destroyFn = mod.functions.find(f => f.name === 'destroy_context');
    assert.ok(destroyFn, 'destroy_context function present');
    assert.ok(destroyFn!.patterns?.some(p => p.kind === 'destroy-function' && p.resourceType === 'OpaqueContext'));

    const addFn = mod.functions.find(f => f.name === 'integer_add');
    assert.ok(addFn, 'integer_add function present');
    assert.ok(addFn!.semantics?.errorConvention, 'error convention inferred for int return');
  });
});

