import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import type { HeaderAST } from '../../lib/h-parser/types.ts';
import { buildIR } from '../../lib/ir/builder.ts';

describe('IR type parsing', () => {
  it('parses a variety of C types into IR types', () => {
    const ast: HeaderAST = {
      header: 'types.h',
      interfaces: [],
      protocols: [],
      enums: [{ name: 'MyEnum', constants: [] }],
      structs: [{ name: 'MyStruct', fields: [{ name: 'values', type: 'double [10]' }] }],
      typedefs: [{ name: 'Baz', underlying: 'unsigned long' }],
      functions: [
        {
          name: 'f',
          return: { spelling: 'int' },
          params: [
            { name: 'a', type: { spelling: 'int' } },
            { name: 's', type: { spelling: 'const char *' } },
            { name: 'p', type: { spelling: 'double *' } },
            { name: 'v', type: { spelling: 'void *' } },
            { name: 'm', type: { spelling: 'MyStruct *' } },
            { name: 'e', type: { spelling: 'enum MyEnum' } },
            { name: 't', type: { spelling: 'Baz' } },
            { name: 'cb', type: { spelling: 'int (*)(int)' } },
          ],
        },
      ],
      classes: [],
      objc_interfaces: [],
      objc_protocols: [],
      objc_categories: [],
    };

    const mod = buildIR(ast, { moduleName: 'types' });
    const f = mod.functions[0]!;
    assert.equal(f.params[0]!.type.kind, 'primitive');
    assert.equal(f.params[1]!.type.kind, 'cstring');
    assert.equal(f.params[2]!.type.kind, 'pointer');
    assert.equal(f.params[3]!.type.kind, 'pointer');
    assert.equal(f.params[4]!.type.kind, 'pointer');
    assert.equal(f.params[5]!.type.kind, 'enum-ref');
    assert.equal(f.params[6]!.type.kind, 'typedef-ref');
    assert.equal(f.params[7]!.type.kind, 'function-pointer');

    const s = mod.structs.find(s => s.name === 'MyStruct')!;
    assert.equal(s.fields[0]!.type.kind, 'array');
  });
});
