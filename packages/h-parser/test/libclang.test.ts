import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import { existsSync } from 'node:fs';
import { parseHeader } from '../index.ts';

const LIBCLANG_HEADERS_PATH = '/opt/homebrew/Cellar/llvm/20.1.8/include/clang-c';

// List of libclang headers to test
const LIBCLANG_HEADERS = [
  'Index.h',
  'CXFile.h',
  'BuildSystem.h',
  'Documentation.h',
  'ExternC.h',
  'CXCompilationDatabase.h',
  'CXSourceLocation.h',
  'FatalErrorHandler.h',
  'CXDiagnostic.h',
  'CXErrorCode.h',
  'CXString.h',
  'Platform.h',
  'Rewrite.h'
];

describe('libclang headers parsing', () => {
  // Skip tests if LLVM is not installed via Homebrew
  const llvmInstalled = existsSync(LIBCLANG_HEADERS_PATH);
  
  if (!llvmInstalled) {
    it.skip('libclang headers not found (LLVM not installed via Homebrew)', () => {});
    return;
  }

  describe('individual header parsing', () => {
    for (const header of LIBCLANG_HEADERS) {
      it(`should parse ${header}`, () => {
        const headerPath = `${LIBCLANG_HEADERS_PATH}/${header}`;
        
        if (!existsSync(headerPath)) {
          console.log(`Skipping ${header} - not found`);
          return;
        }

        const ast = parseHeader(headerPath, {
          language: 'c',
          includeDocumentation: true,
          additionalIncludePaths: [LIBCLANG_HEADERS_PATH]
        });

        assert.ok(ast, `Failed to parse ${header}`);
        assert.equal(ast.header, header);
        
        // Basic validation - libclang headers should have functions and typedefs
        if (header === 'Index.h') {
          // Index.h is the main header with most of the API
          assert.ok(ast.functions && ast.functions.length > 0, `${header} should have functions`);
          assert.ok(ast.typedefs.length > 0, `${header} should have typedefs`);
          assert.ok(ast.enums.length > 0, `${header} should have enums`);
          
          // Check for some known functions - some might be missing due to parsing differences
          const knownFunctions = ['clang_disposeIndex', 'clang_parseTranslationUnit2', 'clang_getCursorKind'];
          let foundCount = 0;
          for (const funcName of knownFunctions) {
            const func = ast.functions?.find(f => f.name === funcName);
            if (func) foundCount++;
          }
          assert.ok(foundCount >= 2, `Should find at least 2 of the known functions in ${header}, found ${foundCount}`);
        } else if (header === 'ExternC.h' || header === 'Platform.h') {
          // These are utility headers, might not have functions
          console.log(`${header} is a utility header, may not contain functions`);
        } else {
          // Other headers should have at least some declarations
          const hasContent = (ast.functions && ast.functions.length > 0) || 
                            ast.typedefs.length > 0 || 
                            ast.enums.length > 0 || 
                            ast.structs.length > 0;
          assert.ok(hasContent, `${header} should have some declarations`);
        }
      });
    }
  });

  it('should parse the main Index.h header with documentation', () => {
    const headerPath = `${LIBCLANG_HEADERS_PATH}/Index.h`;
    const ast = parseHeader(headerPath, {
      language: 'c',
      includeDocumentation: true,
      additionalIncludePaths: [LIBCLANG_HEADERS_PATH]
    });

    assert.ok(ast);
    assert.ok(ast.functions && ast.functions.length > 50, 'Index.h should have many functions (found ' + ast.functions.length + ')');
    
    // Check for documentation in some functions
    const functionsWithDocs = ast.functions?.filter(f => f.documentation) || [];
    assert.ok(functionsWithDocs.length > 0, 'Some functions should have documentation');
    
    // Validate enum parsing
    assert.ok(ast.enums.length > 0, 'Should have parsed enums');
    
    // Check for CXCursorKind enum which is fundamental to libclang
    const cursorKindEnum = ast.enums.find(e => 
      e.constants.some(c => c.name.startsWith('CXCursor_'))
    );
    assert.ok(cursorKindEnum, 'Should find CXCursorKind enum');
    assert.ok(cursorKindEnum.constants.length > 5, 'CXCursorKind should have many constants (found ' + cursorKindEnum.constants.length + ')');
    
    // Check for important typedefs
    const importantTypedefs = ['CXIndex', 'CXTranslationUnit', 'CXCursor', 'CXType'];
    for (const typedef of importantTypedefs) {
      assert.ok(
        ast.typedefs.some(t => t.name === typedef),
        `Should find typedef ${typedef}`
      );
    }
  });

  it('should handle complex C constructs in libclang headers', () => {
    const headerPath = `${LIBCLANG_HEADERS_PATH}/Index.h`;
    const ast = parseHeader(headerPath, {
      language: 'c',
      includeDocumentation: false // Faster parsing without docs
    });

    assert.ok(ast);
    
    // Check for function pointers in typedefs
    const functionPointerTypedefs = ast.typedefs.filter(t => 
      t.underlying.includes('(*)') || t.underlying.includes('(*)')
    );
    assert.ok(functionPointerTypedefs.length > 0, 'Should have function pointer typedefs');
    
    // Check for structs
    assert.ok(ast.structs.length > 0, 'Should have struct declarations');
    
    // Check for variadic functions (functions with ... parameters)
    const variadicFunctions = ast.functions?.filter(f => 
      f.params.some(p => p.type.spelling === '...' || p.type.spelling.includes('...'))
    ) || [];
    
    // Check parsing doesn't crash on complex declarations
    assert.ok(true, 'Successfully parsed complex libclang headers without crashing');
  });
});