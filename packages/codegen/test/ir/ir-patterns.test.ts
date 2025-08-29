import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import type { HeaderAST } from '../../lib/h-parser/types.ts';
import { buildIR } from '../../lib/ir/builder.ts';

describe('IR pattern detection (buffers, strings, callbacks, errors)', () => {
  it('detects buffer+length, string+length, out-params, callbacks, and error conventions', () => {
    const ast: HeaderAST = {
      header: 'patterns.h',
      interfaces: [],
      protocols: [],
      enums: [],
      structs: [],
      typedefs: [],
      functions: [
        { name: 'get_value', return: { spelling: 'int' }, params: [{ name: 'out_value', type: { spelling: 'int *' } }] },
        { name: 'write_data', return: { spelling: 'int' }, params: [{ name: 'buf', type: { spelling: 'void *' } }, { name: 'len', type: { spelling: 'size_t' } }] },
        { name: 'set_name', return: { spelling: 'int' }, params: [{ name: 'name', type: { spelling: 'const char *' } }, { name: 'length', type: { spelling: 'size_t' } }] },
        { name: 'set_callback', return: { spelling: 'void' }, params: [{ name: 'cb', type: { spelling: 'void (*)(int)' } }, { name: 'userdata', type: { spelling: 'void *' } }] },
        { name: 'create_handle', return: { spelling: 'Handle *' }, params: [] },
        { name: 'destroy_handle', return: { spelling: 'void' }, params: [{ name: 'h', type: { spelling: 'Handle *' } }] },
      ],
      classes: [],
      objc_interfaces: [],
      objc_protocols: [],
      objc_categories: [],
    };

    const mod = buildIR(ast, { moduleName: 'patterns' });

    const fGet = mod.functions.find(f => f.name === 'get_value')!;
    assert.ok(fGet.patterns?.some(p => p.kind === 'out-parameter' && p.paramIndex === 0));
    assert.equal(fGet.semantics?.errorConvention, 'nonzero-is-error');

    const fWrite = mod.functions.find(f => f.name === 'write_data')!;
    assert.ok(fWrite.patterns?.some(p => p.kind === 'buffer-with-length' && (p as any).bufferParam === 0));

    const fSetName = mod.functions.find(f => f.name === 'set_name')!;
    assert.ok(fSetName.patterns?.some(p => p.kind === 'string-with-length'));

    const fCb = mod.functions.find(f => f.name === 'set_callback')!;
    assert.ok(fCb.patterns?.some(p => p.kind === 'callback-parameter'));
    assert.ok(fCb.patterns?.some(p => p.kind === 'userdata-parameter'));

    const fCreate = mod.functions.find(f => f.name === 'create_handle')!;
    assert.ok(fCreate.semantics?.returnsOwned);
    assert.equal(fCreate.semantics?.freedBy, 'destroy_handle');
  });
});

