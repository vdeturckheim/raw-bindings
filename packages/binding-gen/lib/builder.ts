import { promises as fs } from 'node:fs';
import { dirname, join } from 'node:path';
import type { Template } from './templates/schema.ts';
import { render, TS_IMPORTS } from './templates/schema.ts';

export interface GenerateUnit {
  template: Template;
  bindings: Record<string, string>;
}

export interface GenerateOptions {
  outDir: string;
  units: GenerateUnit[]; // one or more template+bindings units
  packageName?: string;
  bindingIdent?: string; // used in NODE_API_MODULE
}

async function ensureDir(p: string): Promise<void> {
  await fs.mkdir(p, { recursive: true });
}

async function writeFileEnsuring(path: string, content: string): Promise<void> {
  await ensureDir(dirname(path));
  await fs.writeFile(path, content);
}

function deriveBindingIdent(packageName: string): string {
  const base = packageName.replace(/[^A-Za-z0-9_]/g, '_');
  return base.length ? base : 'addon';
}

export class BindingBuilder {
  async generate(opts: GenerateOptions): Promise<void> {
    const {
      outDir,
      units,
      packageName = 'node-opaque-test',
      bindingIdent = deriveBindingIdent('node_opaque_test')
    } = opts;

    // Aggregate from all units
    const headerSet = new Set<string>();
    const helperSet = new Set<string>();
    const declSet = new Set<string>();
    const defsArr: string[] = [];
    const moduleInitArr: string[] = [];
    const tsParts: string[] = [];

    for (const { template, bindings } of units) {
      for (const h of template.cxx.headers.map((x) => render(x, bindings))) headerSet.add(h);
      for (const h of template.cxx.helpers.map((x) => render(x, bindings))) helperSet.add(h);
      if (template.cxx.declarations?.length) {
        for (const d of template.cxx.declarations.map((x) => render(x, bindings))) declSet.add(d);
      }
      defsArr.push(...template.cxx.definitions.map((x) => render(x, bindings)));
      moduleInitArr.push(...template.cxx.moduleInit.map((x) => render(x, bindings)));
      tsParts.push(...template.ts.parts.map((x) => render(x, bindings)));
    }

    const headers = Array.from(headerSet).sort().join('\n');
    const helpers = Array.from(helperSet).join('\n');
    const declarations = Array.from(declSet).join('\n');
    const definitions = defsArr.join('\n');
    const moduleInitBody = moduleInitArr.join('\n  ');
    const bindingCpp = `${headers}\n\n${helpers}\n${declarations}\n${definitions}\nstatic Napi::Object Init(Napi::Env env, Napi::Object exports) {\n  ${moduleInitBody}\n  return exports;\n}\n\nNODE_API_MODULE(${bindingIdent}, Init)\n`;

    // CMake configuration
    const cmakeLines: string[] = [];
    cmakeLines.push('cmake_minimum_required(VERSION 3.15)');
    cmakeLines.push(`project(${packageName})`);
    cmakeLines.push('add_definitions(-DNAPI_VERSION=9)');
    cmakeLines.push('set(CMAKE_CXX_STANDARD 17)');
    cmakeLines.push('set(CMAKE_CXX_STANDARD_REQUIRED ON)');
    cmakeLines.push('include_directories(${CMAKE_JS_INC})');
    cmakeLines.push('include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)');
    cmakeLines.push('file(GLOB SOURCE_FILES "src/*.cpp" "src/*.cc")');
    cmakeLines.push('add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})');
    cmakeLines.push('target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)');
    cmakeLines.push('set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")');
    cmakeLines.push('target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})');
    cmakeLines.push(`execute_process(COMMAND node -p "require('node-addon-api').include_dir" WORKING_DIRECTORY \${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE NODE_ADDON_API_DIR)`);
    cmakeLines.push('string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})');
    cmakeLines.push('target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})');
    cmakeLines.push('add_definitions(-DNAPI_DISABLE_CPP_EXCEPTIONS)');

    // package.json
    const pkg = {
      name: packageName,
      version: '0.0.1',
      type: 'module',
      main: 'index.ts',
      types: 'index.ts',
      scripts: {
        install: 'cmake-js compile',
        build: 'cmake-js build',
        rebuild: 'cmake-js rebuild',
        clean: 'cmake-js clean'
      },
      dependencies: {
        'cmake-js': '^7.3.1',
        'node-addon-api': '^8.5.0',
        'node-gyp-build': '^4.8.4'
      }
    } as any;

    // index.ts assembled from common imports + all TS parts
    const tsIndex = [TS_IMPORTS, ...tsParts].join('\n\n');

    // Write files
    await ensureDir(join(outDir, 'src'));
    await writeFileEnsuring(join(outDir, 'src/binding.cpp'), bindingCpp);
    await writeFileEnsuring(join(outDir, 'CMakeLists.txt'), cmakeLines.join('\n'));
    await writeFileEnsuring(join(outDir, 'package.json'), JSON.stringify(pkg, null, 2));
    await writeFileEnsuring(join(outDir, 'index.ts'), tsIndex);

  }
}

export default BindingBuilder;
