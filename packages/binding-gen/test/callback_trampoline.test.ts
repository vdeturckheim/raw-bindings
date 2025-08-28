import { describe, it, beforeEach, type TestContext, type SuiteContext } from 'node:test';
import assert from 'node:assert/strict';
import { promises as fs } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { spawn } from 'node:child_process';

import BindingBuilder from '../lib/builder.ts';
import cbTpl from '../lib/templates/core/callback.promise.once.ts';

function run(cmd: string, args: string[], cwd: string): Promise<void> {
  return new Promise((resolve, reject) => {
    const p = spawn(cmd, args, { cwd, stdio: 'inherit' });
    p.on('exit', (code) => (code === 0 ? resolve() : reject(new Error(`${cmd} ${args.join(' ')} -> ${code}`))));
    p.on('error', reject);
  });
}

const outDirs = new WeakMap<TestContext | SuiteContext, string>();

beforeEach(async (t: TestContext | SuiteContext) => {
  const dir = await fs.mkdtemp(join(tmpdir(), 'cb-test-'));
  outDirs.set(t, dir);
});

describe('callback.trampoline template', () => {
  it('returns a Promise resolved via ThreadSafeFunction', async (t: TestContext) => {
    const outDir = outDirs.get(t)!;

    const bindings = {
      fn: 'invoke_cb_once',
      jsName: 'invokeCbOnce'
    } as any;

    const builder = new BindingBuilder();
    await builder.generate({
      outDir,
      units: [{ template: cbTpl, bindings }],
      packageName: 'node-cb-test',
      bindingIdent: 'node_cb_test'
    });

    await run('npm', ['install'], outDir);
    await run('npm', ['run', 'build'], outDir);

    const mod = await import(join(outDir, 'index.ts')) as any;
    const n = await mod.invokeCbOnce(123);
    assert.equal(n, 123);
  });
});
