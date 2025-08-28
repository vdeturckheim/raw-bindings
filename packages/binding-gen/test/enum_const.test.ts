import { describe, it, beforeEach, type TestContext, type SuiteContext } from 'node:test';
import assert from 'node:assert/strict';
import { promises as fs } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { spawn } from 'node:child_process';

import BindingBuilder from '../lib/builder.ts';
import enumTpl from '../lib/templates/core/enum.const.ts';

function run(cmd: string, args: string[], cwd: string): Promise<void> {
  return new Promise((resolve, reject) => {
    const p = spawn(cmd, args, { cwd, stdio: 'inherit' });
    p.on('exit', (code) => (code === 0 ? resolve() : reject(new Error(`${cmd} ${args.join(' ')} -> ${code}`))));
    p.on('error', reject);
  });
}

const outDirs = new WeakMap<TestContext | SuiteContext, string>();

beforeEach(async (t: TestContext | SuiteContext) => {
  const dir = await fs.mkdtemp(join(tmpdir(), 'enum-test-'));
  outDirs.set(t, dir);
});

describe('enum.const template', () => {
  it('emits a const object and type', async (t: TestContext) => {
    const outDir = outDirs.get(t)!;

    const bindings = {
      name: 'TUFlags',
      entries: [
        { name: 'None', value: 0 },
        { name: 'DetailedPreprocessingRecord', value: 1 },
        { name: 'Incomplete', value: 2 }
      ]
    } as any;

    const builder = new BindingBuilder();
    await builder.generate({
      outDir,
      units: [{ template: enumTpl, bindings }],
      packageName: 'node-enum-test',
      bindingIdent: 'node_enum_test'
    });

    await run('npm', ['install'], outDir);
    await run('npm', ['run', 'build'], outDir);

    const mod = await import(join(outDir, 'index.ts')) as any;
    assert.deepEqual(mod.TUFlags, { None: 0, DetailedPreprocessingRecord: 1, Incomplete: 2 });
  });
});

