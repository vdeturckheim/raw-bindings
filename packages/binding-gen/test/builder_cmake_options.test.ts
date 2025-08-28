import { describe, it, beforeEach, type TestContext, type SuiteContext } from 'node:test';
import assert from 'node:assert/strict';
import { promises as fs } from 'node:fs';
import { tmpdir } from 'node:os';
import { join } from 'node:path';

import BindingBuilder from '../lib/builder.ts';
import enumTpl from '../lib/templates/core/enum.const.ts';

const outDirs = new WeakMap<TestContext | SuiteContext, string>();

beforeEach(async (t: TestContext | SuiteContext) => {
  const dir = await fs.mkdtemp(join(tmpdir(), 'cmake-test-'));
  outDirs.set(t, dir);
});

describe('builder cmake options', () => {
  it('injects include dirs, link libs, and definitions', async (t: TestContext) => {
    const outDir = outDirs.get(t)!;

    const builder = new BindingBuilder();
    await builder.generate({
      outDir,
      units: [{ template: enumTpl, bindings: { name: 'X', entries: [] } }],
      packageName: 'node-cmake-test',
      bindingIdent: 'node_cmake_test',
      cmake: {
        includeDirs: ['/opt/include', 'third_party/include'],
        linkLibraries: ['clang', 'z'],
        definitions: ['-DTEST_FLAG=1'],
        extraLines: ['# EXTRA LINE']
      }
    });

    const cmake = await fs.readFile(join(outDir, 'CMakeLists.txt'), 'utf8');
    assert.match(cmake, /include_directories\(\/opt\/include\)/);
    assert.match(cmake, /include_directories\(third_party\/include\)/);
    assert.match(cmake, /target_link_libraries\(\$\{PROJECT_NAME\} clang z\)/);
    assert.match(cmake, /add_definitions\(-DTEST_FLAG=1\)/);
    assert.match(cmake, /# EXTRA LINE/);
  });
});

