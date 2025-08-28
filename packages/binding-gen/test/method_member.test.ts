import { describe, it, beforeEach, type TestContext, type SuiteContext } from 'node:test';
import assert from 'node:assert/strict';
import { promises as fs } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { spawn } from 'node:child_process';

import BindingBuilder from '../lib/builder.ts';
import opaqueTpl from '../lib/templates/core/resource.opaque.ts';
import methodTpl from '../lib/templates/core/method.member.ts';

function run(cmd: string, args: string[], cwd: string): Promise<void> {
  return new Promise((resolve, reject) => {
    const p = spawn(cmd, args, { cwd, stdio: 'inherit' });
    p.on('exit', (code) => (code === 0 ? resolve() : reject(new Error(`${cmd} ${args.join(' ')} -> ${code}`))));
    p.on('error', reject);
  });
}

const outDirs = new WeakMap<TestContext | SuiteContext, string>();

beforeEach(async (t: TestContext | SuiteContext) => {
  const dir = await fs.mkdtemp(join(tmpdir(), 'method-test-'));
  outDirs.set(t, dir);
});

describe('method.member template', () => {
  it('attaches a numeric-returning method to an opaque class', async (t: TestContext) => {
    const outDir = outDirs.get(t)!;

    const myHeader = `#pragma once
struct MyCtx { int a; };
MyCtx* my_create(int a);
void my_destroy(MyCtx* ctx);
int my_get_a(MyCtx* ctx);
`;
    const mySource = `#include "mylib.h"
MyCtx* my_create(int a){ auto* p = new MyCtx(); p->a=a; return p; }
void my_destroy(MyCtx* ctx){ delete ctx; }
int my_get_a(MyCtx* ctx){ return ctx->a; }
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
      methodName: 'getA',
      methodFn: 'my_get_a',
      methodCall: 'my_get_a(handle)'
    } as Record<string, string>;

    const builder = new BindingBuilder();
    await builder.generate({
      outDir,
      units: [
        { template: opaqueTpl, bindings: opaque },
        { template: methodTpl, bindings: method }
      ],
      packageName: 'node-method-test',
      bindingIdent: 'node_method_test'
    });

    await fs.writeFile(join(outDir, 'src/mylib.h'), myHeader);
    await fs.writeFile(join(outDir, 'src/mylib.cpp'), mySource);

    await run('npm', ['install'], outDir);
    await run('npm', ['run', 'build'], outDir);

    const mod = await import(join(outDir, 'index.ts')) as any;
    const ctx = mod.Ctx.create(42);
    assert.equal(ctx.getA(), 42);
    // @ts-ignore
    ctx[Symbol.dispose]();
  });
});

