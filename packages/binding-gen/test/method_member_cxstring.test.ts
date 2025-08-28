import { describe, it, beforeEach, type TestContext, type SuiteContext } from 'node:test';
import assert from 'node:assert/strict';
import { promises as fs } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { spawn } from 'node:child_process';

import BindingBuilder from '../lib/builder.ts';
import opaqueTpl from '../lib/templates/core/resource.opaque.ts';
import methodStrTpl from '../lib/templates/core/method.member.cxstring.ts';

function run(cmd: string, args: string[], cwd: string): Promise<void> {
  return new Promise((resolve, reject) => {
    const p = spawn(cmd, args, { cwd, stdio: 'inherit' });
    p.on('exit', (code) => (code === 0 ? resolve() : reject(new Error(`${cmd} ${args.join(' ')} -> ${code}`))));
    p.on('error', reject);
  });
}

const outDirs = new WeakMap<TestContext | SuiteContext, string>();

beforeEach(async (t: TestContext | SuiteContext) => {
  const dir = await fs.mkdtemp(join(tmpdir(), 'cxstr-test-'));
  outDirs.set(t, dir);
});

describe('method.member.cxstring template', () => {
  it('returns a string and disposes native CXString-like result', async (t: TestContext) => {
    const outDir = outDirs.get(t)!;

    const myHeader = `#pragma once
struct MyCtx { int a; };
struct MyStr { const char* s; };
MyCtx* my_create(int a);
void my_destroy(MyCtx* ctx);
MyStr my_name(MyCtx* ctx);
const char* my_get_cstr(MyStr s);
void my_dispose_str(MyStr s);
`;
    const mySource = `#include "mylib.h"
MyCtx* my_create(int a){ auto* p = new MyCtx(); p->a=a; return p; }
void my_destroy(MyCtx* ctx){ delete ctx; }
MyStr my_name(MyCtx* ctx){ (void)ctx; MyStr r; r.s = "hello"; return r; }
const char* my_get_cstr(MyStr s){ return s.s; }
void my_dispose_str(MyStr s){ (void)s; /* no-op */ }
`;

    const opaque = {
      headerInclude: 'mylib.h',
      handleTypedef: 'MyCtx*',
      handleCxxType: 'MyCtx*',
      className: 'Ctx',
      createFn: 'my_create',
      createCall: 'my_create(info[0].As<Napi::Number>().Int32Value())',
      destroyFn: 'my_destroy'
    } as Record<string, string>;

    const method = {
      handleCxxType: 'MyCtx*',
      className: 'Ctx',
      methodName: 'name',
      methodFn: 'my_name',
      methodCall: 'my_name(handle)',
      getCString: 'my_get_cstr',
      dispose: 'my_dispose_str'
    } as Record<string, string>;

    const builder = new BindingBuilder();
    await builder.generate({
      outDir,
      units: [
        { template: opaqueTpl, bindings: opaque },
        { template: methodStrTpl, bindings: method }
      ],
      packageName: 'node-cxstr-test',
      bindingIdent: 'node_cxstr_test'
    });

    await fs.writeFile(join(outDir, 'src/mylib.h'), myHeader);
    await fs.writeFile(join(outDir, 'src/mylib.cpp'), mySource);

    await run('npm', ['install'], outDir);
    await run('npm', ['run', 'build'], outDir);

    const mod = await import(join(outDir, 'index.ts')) as any;
    const ctx = mod.Ctx.create(1);
    assert.equal(ctx.name(), 'hello');
    // @ts-ignore
    ctx[Symbol.dispose]();
  });
});

