import type { GeneratorOptions } from '../types.ts';

export class BuildGenerator {
  private options: GeneratorOptions;

  constructor(options: GeneratorOptions) {
    this.options = options;
  }

  generateCMakeLists(): string {
    const lines: string[] = [];

    lines.push('cmake_minimum_required(VERSION 3.15)');
    lines.push('cmake_policy(SET CMP0091 NEW)');
    lines.push('cmake_policy(SET CMP0042 NEW)');
    lines.push('');
    lines.push(`project(${this.options.packageName})`);
    lines.push('');
    lines.push('add_definitions(-DNAPI_VERSION=9)');
    lines.push('');
    lines.push('include_directories(${CMAKE_JS_INC})');
    lines.push('');

    // Add include directories
    if (this.options.includePaths?.length) {
      lines.push('# Additional include directories');
      for (const path of this.options.includePaths) {
        lines.push(`include_directories(${path})`);
      }
      lines.push('');
    }

    // Add source files
    lines.push('file(GLOB SOURCE_FILES "src/*.cpp" "src/*.cc")');
    lines.push('');
    lines.push(`add_library(\${PROJECT_NAME} SHARED \${SOURCE_FILES} \${CMAKE_JS_SRC})`);
    lines.push('');
    lines.push('set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")');
    lines.push('');

    // Link libraries
    if (this.options.libraries?.length || this.options.libraryPaths?.length) {
      lines.push('# Link libraries');
      
      // Add library paths
      if (this.options.libraryPaths?.length) {
        for (const path of this.options.libraryPaths) {
          lines.push(`link_directories(${path})`);
        }
      }

      // Find and link libraries
      if (this.options.libraries?.length) {
        for (const lib of this.options.libraries) {
          lines.push(`find_library(${lib.toUpperCase()}_LIB ${lib} PATHS ${this.options.libraryPaths?.join(' ') || ''} REQUIRED)`);
        }
        
        const libVars = this.options.libraries.map(lib => `\${${lib.toUpperCase()}_LIB}`).join(' ');
        lines.push(`target_link_libraries(\${PROJECT_NAME} \${CMAKE_JS_LIB} ${libVars})`);
      } else {
        lines.push('target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})');
      }
    } else {
      lines.push('target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})');
    }
    lines.push('');

    // Framework support for macOS
    if (this.options.frameworks?.length) {
      lines.push('# Frameworks (macOS)');
      lines.push('if(APPLE)');
      for (const framework of this.options.frameworks) {
        lines.push(`  target_link_libraries(\${PROJECT_NAME} "-framework ${framework}")`);
      }
      if (this.options.frameworkPaths?.length) {
        for (const path of this.options.frameworkPaths) {
          lines.push(`  target_compile_options(\${PROJECT_NAME} PRIVATE -F${path})`);
          lines.push(`  target_link_options(\${PROJECT_NAME} PRIVATE -F${path})`);
        }
      }
      lines.push('endif()');
      lines.push('');
    }

    // MSVC specific
    lines.push('if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)');
    lines.push('  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})');
    lines.push('endif()');
    lines.push('');

    // Include Node-API wrappers
    lines.push('# Include Node-API wrappers');
    lines.push('execute_process(COMMAND node -p "require(\'node-addon-api\').include"');
    lines.push('        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}');
    lines.push('        OUTPUT_VARIABLE NODE_ADDON_API_DIR');
    lines.push('        )');
    lines.push('string(REPLACE "\\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})');
    lines.push('string(REPLACE "\\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})');
    lines.push('target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})');
    lines.push('');
    lines.push('# define NAPI_CPP_EXCEPTIONS to disable exceptions');
    lines.push('add_definitions(-DNAPI_DISABLE_CPP_EXCEPTIONS)');

    // Add compiler definitions
    if (this.options.defines?.length) {
      lines.push('');
      lines.push('# Custom definitions');
      for (const define of this.options.defines) {
        lines.push(`add_definitions(-D${define})`);
      }
    }

    return lines.join('\n');
  }

  generatePackageJson(): string {
    const pkg = {
      name: this.options.packageName,
      version: this.options.packageVersion || '0.0.1',
      description: this.options.description || `Node.js bindings for ${this.options.libraryName || this.options.packageName}`,
      type: 'module',
      main: 'index.ts',
      types: 'index.ts',
      scripts: {
        install: 'cmake-js compile',
        build: 'cmake-js build',
        'build:debug': 'cmake-js build --debug',
        rebuild: 'cmake-js rebuild',
        clean: 'cmake-js clean',
        test: 'node --test'
      },
      keywords: [
        'native',
        'addon',
        'bindings',
        this.options.libraryName || this.options.packageName
      ],
      author: this.options.author || '',
      license: this.options.license || 'MIT',
      dependencies: {
        'bindings': '^1.5.0',
        'cmake-js': '^7.3.1',
        'node-addon-api': '^8.5.0',
        'node-gyp-build': '^4.8.4'
      },
      devDependencies: {
        '@types/node': '^22.0.0'
      }
    };

    return JSON.stringify(pkg, null, 2);
  }

  generateTestFile(): string {
    const lines: string[] = [];

    lines.push(`import assert from 'node:assert/strict';`);
    lines.push(`import { describe, it } from 'node:test';`);
    lines.push(`import * as binding from './index.ts';`);
    lines.push(``);
    lines.push(`describe('${this.options.packageName} bindings', () => {`);
    lines.push(`  it('should load the module', () => {`);
    lines.push(`    assert.ok(binding);`);
    lines.push(`  });`);
    lines.push(``);
    lines.push(`  // Add more specific tests here based on the exported functions`);
    lines.push(`  // Example:`);
    lines.push(`  // it('should call a function', () => {`);
    lines.push(`  //   const result = binding.someFunction();`);
    lines.push(`  //   assert.ok(result);`);
    lines.push(`  // });`);
    lines.push(`});`);

    return lines.join('\n');
  }
}