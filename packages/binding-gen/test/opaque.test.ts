import { describe, it, beforeEach, type TestContext, type SuiteContext } from 'node:test';
import assert from 'node:assert/strict';
import { promises as fs } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { spawn } from 'node:child_process';
import opaqueTpl from '../lib/templates/core/resource.opaque.ts';
import BindingBuilder from '../lib/builder.ts';

function run(cmd: string, args: string[], cwd: string): Promise<void> {
  return new Promise((resolve, reject) => {
    const p = spawn(cmd, args, { cwd, stdio: 'inherit' });
    p.on('exit', (code) => (code === 0 ? resolve() : reject(new Error(`${cmd} ${args.join(' ')} -> ${code}`))));
    p.on('error', reject);
  });
}

const outDirs = new WeakMap<TestContext | SuiteContext, string>();

beforeEach(async (t: TestContext | SuiteContext) => {
  const dir = await fs.mkdtemp(join(tmpdir(), 'opaque-test-'));
  outDirs.set(t, dir);
});

describe('opaque template (local lib)', () => {
  it('generates, compiles, and creates/disposes opaque resource (using + manual)', async (t: TestContext) => {
    const outDir = outDirs.get(t)!;

    // Render C++ addon
    const myHeader = `#pragma once
struct MyCtx { int a; int b; };
MyCtx* my_create(int a, int b);
void my_destroy(MyCtx* ctx);
`;
    const mySource = `#include "mylib.h"
MyCtx* my_create(int a, int b){ auto* p = new MyCtx(); p->a=a; p->b=b; return p; }
void my_destroy(MyCtx* ctx){ delete ctx; }
`;

    const bindings = {
      headerInclude: 'mylib.h',
      handleTypedef: 'MyCtx*',
      handleCxxType: 'MyCtx*',
      className: 'Ctx',
      createFn: 'my_create',
      createCall: 'my_create(info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value())',
      destroyFn: 'my_destroy'
    } as Record<string, string>;

    const builder = new BindingBuilder();
    await builder.generate({
      outDir,
      units: [{ template: opaqueTpl, bindings }],
      packageName: 'node-opaque-test',
      bindingIdent: 'node_opaque_test'
    });

    await fs.writeFile(join(outDir, 'src/mylib.h'), myHeader);
    await fs.writeFile(join(outDir, 'src/mylib.cpp'), mySource);

    await run('npm', ['install'], outDir);
    await run('npm', ['run', 'build'], outDir);

    const mod = await import(join(outDir, 'index.ts')) as any;
    assert.ok(mod.Ctx, 'Ctx class should be exported');

    // Validate using declaration disposes automatically (Node >= 24.3 guaranteed)
    {
      // @ts-ignore
      using ctx = mod.Ctx.create(1, 2);
      if (ctx.disposed !== false) throw new Error('ctx should not be disposed inside using');
    }

    // Manual disposal path
    const ctx2 = mod.Ctx.create(3, 4);
    assert.equal(ctx2.disposed, false);
    // @ts-ignore
    ctx2[Symbol.dispose]();
    assert.equal(ctx2.disposed, true);
  });
  
  it('generates a project with two opaques and both work', async (t: TestContext) => {
    const outDir = outDirs.get(t)!;

    const myHeader = `#pragma once
struct MyCtxA { int x; };
struct MyCtxB { int y; };
MyCtxA* myA_create(int a);
void myA_destroy(MyCtxA* ctx);
MyCtxB* myB_create(int b);
void myB_destroy(MyCtxB* ctx);
`;
    const mySource = `#include "mylib.h"
MyCtxA* myA_create(int a){ auto* p = new MyCtxA(); p->x=a; return p; }
void myA_destroy(MyCtxA* ctx){ delete ctx; }
MyCtxB* myB_create(int b){ auto* p = new MyCtxB(); p->y=b; return p; }
void myB_destroy(MyCtxB* ctx){ delete ctx; }
`;

    const a = {
      headerInclude: 'mylib.h',
      handleTypedef: 'MyCtxA*',
      handleCxxType: 'MyCtxA*',
      className: 'CtxA',
      createFn: 'myA_create',
      createCall: 'myA_create(info[0].As<Napi::Number>().Int32Value())',
      destroyFn: 'myA_destroy'
    } as Record<string, string>;

    const b = {
      headerInclude: 'mylib.h',
      handleTypedef: 'MyCtxB*',
      handleCxxType: 'MyCtxB*',
      className: 'CtxB',
      createFn: 'myB_create',
      createCall: 'myB_create(info[0].As<Napi::Number>().Int32Value())',
      destroyFn: 'myB_destroy'
    } as Record<string, string>;

    const builder = new BindingBuilder();
    await builder.generate({
      outDir,
      units: [
        { template: opaqueTpl, bindings: a },
        { template: opaqueTpl, bindings: b }
      ],
      packageName: 'node-opaque-two',
      bindingIdent: 'node_opaque_two'
    });

    await fs.writeFile(join(outDir, 'src/mylib.h'), myHeader);
    await fs.writeFile(join(outDir, 'src/mylib.cpp'), mySource);

    await run('npm', ['install'], outDir);
    await run('npm', ['run', 'build'], outDir);

    const mod = await import(join(outDir, 'index.ts')) as any;
    assert.ok(mod.CtxA, 'CtxA class should be exported');
    assert.ok(mod.CtxB, 'CtxB class should be exported');

    // using validation for both
    {
      // @ts-ignore
      using ctxA = mod.CtxA.create(10);
      if (ctxA.disposed !== false) throw new Error('ctxA should not be disposed inside using');
    }
    {
      // @ts-ignore
      using ctxB = mod.CtxB.create(20);
      if (ctxB.disposed !== false) throw new Error('ctxB should not be disposed inside using');
    }

    // Manual disposal for both
    const a2 = mod.CtxA.create(1);
    const b2 = mod.CtxB.create(2);
    assert.equal(a2.disposed, false);
    assert.equal(b2.disposed, false);
    // @ts-ignore
    a2[Symbol.dispose]();
    // @ts-ignore
    b2[Symbol.dispose]();
    assert.equal(a2.disposed, true);
    assert.equal(b2.disposed, true);
  });
});
