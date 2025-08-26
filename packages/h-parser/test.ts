import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import { parseHeader, parseHeaderToJSON } from './index.ts';
import type { HeaderAST } from './types.ts';

const __dirname = dirname(fileURLToPath(import.meta.url));

describe('h-parser', () => {
  it('should parse a simple C header file', () => {
    const headerPath = join(__dirname, 'testdata', 'sample.h');
    const ast = parseHeader(headerPath, {
      language: 'c',
      includeDocumentation: true
    });

    assert.ok(ast);
    assert.equal(ast.header, 'sample.h');

    // Check enums
    assert.ok(ast.enums.length > 0);
    const statusEnum = ast.enums.find(e => e.name === '');  // Anonymous enum with typedef
    if (statusEnum) {
      assert.ok(statusEnum.constants.some(c => c.name === 'STATUS_SUCCESS' && c.value === 0));
      assert.ok(statusEnum.constants.some(c => c.name === 'STATUS_ERROR' && c.value === -1));
      assert.ok(statusEnum.constants.some(c => c.name === 'STATUS_TIMEOUT' && c.value === -2));
      assert.ok(statusEnum.constants.some(c => c.name === 'STATUS_NOT_FOUND' && c.value === -3));
    }

    // Check structs
    assert.ok(ast.structs.length > 0);
    
    // Check for Point struct (anonymous struct with typedef)
    const pointStruct = ast.structs.find(s => s.name === '');
    if (pointStruct) {
      assert.ok(pointStruct.fields.some(f => f.name === 'x' && f.type === 'float'));
      assert.ok(pointStruct.fields.some(f => f.name === 'y' && f.type === 'float'));
    }

    // Check for Rectangle struct (named struct)
    const rectangleStruct = ast.structs.find(s => s.name === 'Rectangle');
    assert.ok(rectangleStruct);
    assert.ok(rectangleStruct.fields.some(f => f.name === 'top_left'));
    assert.ok(rectangleStruct.fields.some(f => f.name === 'bottom_right'));
    assert.ok(rectangleStruct.fields.some(f => f.name === 'area' && f.type === 'float'));

    // Check typedefs
    assert.ok(ast.typedefs.length > 0);
    assert.ok(ast.typedefs.some(t => t.name === 'Status'));
    assert.ok(ast.typedefs.some(t => t.name === 'Point'));
    assert.ok(ast.typedefs.some(t => t.name === 'Rectangle'));
    assert.ok(ast.typedefs.some(t => t.name === 'Handle'));

    // Check functions
    assert.ok(ast.functions);
    assert.ok(ast.functions.length > 0);

    // Check calculate_distance function
    const calcDistance = ast.functions.find(f => f.name === 'calculate_distance');
    assert.ok(calcDistance);
    assert.equal(calcDistance.return.spelling, 'float');
    assert.equal(calcDistance.params.length, 2);
    assert.equal(calcDistance.params[0].name, 'p1');
    assert.equal(calcDistance.params[1].name, 'p2');
    if (calcDistance.documentation) {
      assert.ok(calcDistance.documentation.includes('Calculate the distance'));
    }

    // Check initialize_system function
    const initSystem = ast.functions.find(f => f.name === 'initialize_system');
    assert.ok(initSystem);
    assert.equal(initSystem.params.length, 1);
    assert.equal(initSystem.params[0].name, 'config');
    assert.ok(initSystem.params[0].type.spelling.includes('const char'));

    // Check process_data function
    const processData = ast.functions.find(f => f.name === 'process_data');
    assert.ok(processData);
    assert.equal(processData.return.spelling, 'int');
    assert.equal(processData.params.length, 4);
    assert.equal(processData.params[0].name, 'buffer');
    assert.equal(processData.params[1].name, 'size');
    assert.equal(processData.params[2].name, 'output');
    assert.equal(processData.params[3].name, 'output_size');

    // Check cleanup function
    const cleanup = ast.functions.find(f => f.name === 'cleanup');
    assert.ok(cleanup);
    assert.equal(cleanup.return.spelling, 'void');
    assert.equal(cleanup.params.length, 0);
  });

  it('should export parseHeaderToJSON function', () => {
    const headerPath = join(__dirname, 'testdata', 'sample.h');
    const json = parseHeaderToJSON(headerPath, {
      language: 'c'
    });

    assert.ok(json);
    assert.ok(typeof json === 'string');
    
    const parsed = JSON.parse(json);
    assert.ok(parsed);
    assert.equal(parsed.header, 'sample.h');
    assert.ok(Array.isArray(parsed.enums));
    assert.ok(Array.isArray(parsed.structs));
    assert.ok(Array.isArray(parsed.typedefs));
    assert.ok(Array.isArray(parsed.functions));
  });

  it('should handle documentation extraction', () => {
    const headerPath = join(__dirname, 'testdata', 'sample.h');
    const ast = parseHeader(headerPath, {
      language: 'c',
      includeDocumentation: true
    });

    // Check that at least some functions have documentation
    const functionsWithDocs = ast.functions?.filter(f => f.documentation) || [];
    assert.ok(functionsWithDocs.length > 0, 'Should have extracted documentation for some functions');

    // Check specific documentation content
    const calcDistance = ast.functions?.find(f => f.name === 'calculate_distance');
    if (calcDistance?.documentation) {
      assert.ok(calcDistance.documentation.includes('distance between two points'));
      assert.ok(calcDistance.documentation.includes('@param p1'));
      assert.ok(calcDistance.documentation.includes('@return'));
    }
  });

  it('should handle parse options correctly', () => {
    const headerPath = join(__dirname, 'testdata', 'sample.h');
    
    // Test with different language options
    const cAst = parseHeader(headerPath, { language: 'c' });
    assert.ok(cAst);
    
    const cppAst = parseHeader(headerPath, { language: 'c++' });
    assert.ok(cppAst);

    // Both should successfully parse the header
    assert.equal(cAst.header, 'sample.h');
    assert.equal(cppAst.header, 'sample.h');
  });
});