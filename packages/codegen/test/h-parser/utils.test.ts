import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import * as utils from '../../lib/h-parser/utils.ts';

describe('h-parser utils', () => {
  it('buildCompilerArgs assembles flags', async () => {
    const args = utils.buildCompilerArgs({
      language: 'c',
      includePaths: ['/usr/include', '/opt/include'],
      defines: ['DEBUG', 'NDEBUG=1'],
      additionalArgs: ['-Wall'],
    });
    assert.ok(args.includes('-x'));
    assert.ok(args.includes('c'));
    assert.ok(args.includes('-I'));
    assert.ok(args.includes('/usr/include'));
    assert.ok(args.includes('/opt/include'));
    assert.ok(args.includes('-D'));
    assert.ok(args.includes('DEBUG'));
    assert.ok(args.includes('NDEBUG=1'));
    assert.ok(args.includes('-Wall'));
  });
});
