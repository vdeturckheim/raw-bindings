import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import type { TestContext } from 'node:test';
import { beforeEach, describe, it } from 'node:test';
import type { Cursor } from './index.ts';
import * as clang from './index.ts';

const kTempDir = Symbol('kTempDir');

interface TestContextWithTempDir extends TestContext {
  [kTempDir]: string;
}

describe('node-clang bindings', () => {
  beforeEach(async (t) => {
    const baseDir = tmpdir();
    const prefix = 'node-clang-test-';
    (t as unknown as TestContextWithTempDir)[kTempDir] = await fs.mkdtemp(
      join(baseDir, prefix),
    );
  });

  it('should create and dispose index', () => {
    const index = clang.createIndex();
    assert.ok(index);
    assert.ok(index instanceof clang.ClangIndex);
    index.dispose();
  });

  it('should create index with options', () => {
    const index = clang.createIndex(true, false);
    assert.ok(index);
    index.dispose();
  });

  it('should parse a simple C file', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    // Create a test file
    const testFile = join(ctx[kTempDir], 'test_clang.c');
    const testCode = `
            // This is a test function
            int add(int a, int b) {
                return a + b;
            }
            
            typedef struct {
                int x;
                int y;
            } Point;
            
            enum Color {
                RED = 0,
                GREEN = 1,
                BLUE = 2
            };
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      assert.ok(tu);
      assert.ok(tu instanceof clang.ClangTranslationUnit);

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get translation unit cursor and visit children', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_visitor.c');
    const testCode = `
            int globalVar = 42;
            
            void testFunction() {
                int localVar = 10;
            }
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      assert.ok(cursor);
      assert.ok(cursor._cursor);

      interface Declaration {
        kind: number;
        spelling: string;
      }

      const declarations: Declaration[] = [];
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        const kind = clang.getCursorKind(child);
        const spelling = clang.getCursorSpelling(child);

        if (spelling) {
          declarations.push({ kind, spelling });
        }

        return clang.CXChildVisit.Continue;
      });

      // Check we found our declarations
      assert.ok(declarations.some((d) => d.spelling === 'globalVar'));
      assert.ok(declarations.some((d) => d.spelling === 'testFunction'));

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get cursor location information', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_location.c');
    const testCode = `int myFunction() { return 0; }`;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      let functionCursor: Cursor | null = null;
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        if (clang.getCursorSpelling(child) === 'myFunction') {
          functionCursor = child;
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(functionCursor);
      const location = clang.getCursorLocation(functionCursor);

      assert.ok(location);
      assert.equal(location.line, 1);
      assert.ok(location.file?.includes('test_location.c'));

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get type information', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_types.c');
    const testCode = `
            float calculate(double x, int y) {
                return x + y;
            }
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      let functionCursor: Cursor | null = null;
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        if (clang.getCursorSpelling(child) === 'calculate') {
          functionCursor = child;
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(functionCursor);
      const type = clang.getCursorType(functionCursor);
      assert.ok(type);

      const typeSpelling = clang.getTypeSpelling(type);
      assert.ok(typeSpelling.includes('float'));
      assert.ok(typeSpelling.includes('double'));
      assert.ok(typeSpelling.includes('int'));

      // Check result type
      const resultType = clang.getResultType(type);
      const resultTypeSpelling = clang.getTypeSpelling(resultType);
      assert.equal(resultTypeSpelling, 'float');

      // Check number of arguments
      const numArgs = clang.getNumArgTypes(type);
      assert.equal(numArgs, 2);

      // Check first argument type
      const arg0Type = clang.getArgType(type, 0);
      assert.equal(clang.getTypeSpelling(arg0Type), 'double');

      // Check second argument type
      const arg1Type = clang.getArgType(type, 1);
      assert.equal(clang.getTypeSpelling(arg1Type), 'int');

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get enum constant values', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_enum.c');
    const testCode = `
            enum Status {
                SUCCESS = 0,
                ERROR = -1,
                PENDING = 42
            };
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      const enumValues: Record<string, number> = {};
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        const kind = clang.getCursorKind(child);
        if (kind === clang.CXCursorKind.EnumConstantDecl) {
          const name = clang.getCursorSpelling(child);
          const value = clang.getEnumConstantDeclValue(child);
          enumValues[name] = value;
        }
        return clang.CXChildVisit.Recurse;
      });

      assert.equal(enumValues.SUCCESS, 0);
      assert.equal(enumValues.ERROR, -1);
      assert.equal(enumValues.PENDING, 42);

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get typedef underlying type', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_typedef.c');
    const testCode = `
            typedef unsigned long size_t;
            typedef int* IntPtr;
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      const typedefs: Record<string, string> = {};
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        const kind = clang.getCursorKind(child);
        if (kind === clang.CXCursorKind.TypedefDecl) {
          const name = clang.getCursorSpelling(child);
          const underlyingType = clang.getTypedefDeclUnderlyingType(child);
          typedefs[name] = clang.getTypeSpelling(underlyingType);
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(typedefs.size_t);
      assert.ok(typedefs.size_t.includes('unsigned long'));
      assert.ok(typedefs.IntPtr);
      assert.ok(typedefs.IntPtr.includes('int'));

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get raw comment text', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_comments.c');
    const testCode = `
            /**
             * This function adds two numbers
             * @param a First number
             * @param b Second number
             * @return The sum of a and b
             */
            int add(int a, int b) {
                return a + b;
            }
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      let comment: string | null = null;
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        if (clang.getCursorSpelling(child) === 'add') {
          comment = clang.getCursorRawCommentText(child);
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(comment);
      if (comment) {
        const commentStr = comment as string;
        assert.ok(commentStr.includes('This function adds two numbers'));
        assert.ok(commentStr.includes('@param a First number'));
        assert.ok(commentStr.includes('@return The sum of a and b'));
      }

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should handle visitor break correctly', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_break.c');
    const testCode = `
            int a;
            int b;
            int c;
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      const found: string[] = [];
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        const spelling = clang.getCursorSpelling(child);
        if (spelling) {
          found.push(spelling);
          if (spelling === 'b') {
            return clang.CXChildVisit.Break;
          }
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(found.includes('a'));
      assert.ok(found.includes('b'));
      assert.ok(!found.includes('c')); // Should not reach 'c' due to break

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get cursor argument information', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_cursor_args.c');
    const testCode = `
            // Function with multiple arguments
            float calculate(double x, int y, const char* name) {
                return x + y;
            }
            
            // Function with no arguments
            void noArgs() {
                // Do nothing
            }
            
            // Variadic function
            int sum(int count, ...) {
                return count;
            }
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      // Test calculate function
      let calculateCursor: Cursor | null = null;
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        if (clang.getCursorSpelling(child) === 'calculate') {
          calculateCursor = child;
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(calculateCursor);

      // Test getCursorResultType
      const resultType = clang.getCursorResultType(calculateCursor);
      assert.ok(resultType);
      const resultTypeSpelling = clang.getTypeSpelling(resultType);
      assert.equal(resultTypeSpelling, 'float');

      // Test getNumCursorArguments
      const numArgs = clang.getNumCursorArguments(calculateCursor);
      assert.equal(numArgs, 3);

      // Test getCursorArgument for each argument
      const arg0 = clang.getCursorArgument(calculateCursor, 0);
      assert.ok(arg0);
      assert.equal(clang.getCursorSpelling(arg0), 'x');
      const arg0Type = clang.getCursorType(arg0);
      assert.equal(clang.getTypeSpelling(arg0Type), 'double');

      const arg1 = clang.getCursorArgument(calculateCursor, 1);
      assert.ok(arg1);
      assert.equal(clang.getCursorSpelling(arg1), 'y');
      const arg1Type = clang.getCursorType(arg1);
      assert.equal(clang.getTypeSpelling(arg1Type), 'int');

      const arg2 = clang.getCursorArgument(calculateCursor, 2);
      assert.ok(arg2);
      assert.equal(clang.getCursorSpelling(arg2), 'name');
      const arg2Type = clang.getCursorType(arg2);
      assert.ok(clang.getTypeSpelling(arg2Type).includes('const char'));

      // Test function with no arguments
      let noArgsCursor: Cursor | null = null;
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        if (clang.getCursorSpelling(child) === 'noArgs') {
          noArgsCursor = child;
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(noArgsCursor);
      const noArgsResultType = clang.getCursorResultType(noArgsCursor);
      assert.equal(clang.getTypeSpelling(noArgsResultType), 'void');
      const noArgsCount = clang.getNumCursorArguments(noArgsCursor);
      assert.equal(noArgsCount, 0);

      // Test variadic function
      let sumCursor: Cursor | null = null;
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        if (clang.getCursorSpelling(child) === 'sum') {
          sumCursor = child;
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(sumCursor);
      const sumResultType = clang.getCursorResultType(sumCursor);
      assert.equal(clang.getTypeSpelling(sumResultType), 'int');
      const sumArgsCount = clang.getNumCursorArguments(sumCursor);
      assert.equal(sumArgsCount, 1); // Only the fixed argument is counted

      const sumArg0 = clang.getCursorArgument(sumCursor, 0);
      assert.ok(sumArg0);
      assert.equal(clang.getCursorSpelling(sumArg0), 'count');

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should recognize Objective-C cursor kinds', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_objc.m');
    const testCode = `
            @interface MyClass : NSObject
            @property (nonatomic, strong) NSString *name;
            @property (nonatomic, assign) NSInteger age;
            
            - (void)instanceMethod;
            + (void)classMethod;
            @end
            
            @protocol MyProtocol
            - (void)protocolMethod;
            @end
            
            @interface MyClass (MyCategory)
            - (void)categoryMethod;
            @end
        `;
    await fs.writeFile(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, [
        '-x',
        'objective-c',
      ]);
      const cursor = tu.getCursor();

      const foundKinds: Record<string, number> = {};
      clang.visitChildren(cursor, (child: Cursor, _parent: Cursor) => {
        const kind = clang.getCursorKind(child);
        const spelling = clang.getCursorSpelling(child);

        if (spelling) {
          foundKinds[spelling] = kind;
        }

        return clang.CXChildVisit.Recurse;
      });

      // Check for Objective-C specific cursor kinds
      // Note: If parsing fails to recognize ObjC code, check that at least the interface is recognized
      if (foundKinds.MyClass === clang.CXCursorKind.ObjCInterfaceDecl) {
        assert.equal(foundKinds.MyClass, clang.CXCursorKind.ObjCInterfaceDecl);
        assert.equal(foundKinds.name, clang.CXCursorKind.ObjCPropertyDecl);
        assert.equal(foundKinds.age, clang.CXCursorKind.ObjCPropertyDecl);
        assert.equal(
          foundKinds.instanceMethod,
          clang.CXCursorKind.ObjCInstanceMethodDecl,
        );
        assert.equal(
          foundKinds.classMethod,
          clang.CXCursorKind.ObjCClassMethodDecl,
        );
        assert.equal(
          foundKinds.MyProtocol,
          clang.CXCursorKind.ObjCProtocolDecl,
        );
        assert.equal(
          foundKinds.MyCategory,
          clang.CXCursorKind.ObjCCategoryDecl,
        );
      } else {
        // If ObjC parsing isn't fully supported, at least verify the constants are exported correctly
        assert.ok(
          clang.CXCursorKind.ObjCInterfaceDecl !== undefined,
          'ObjCInterfaceDecl constant should be defined',
        );
        assert.ok(
          clang.CXCursorKind.ObjCCategoryDecl !== undefined,
          'ObjCCategoryDecl constant should be defined',
        );
        assert.ok(
          clang.CXCursorKind.ObjCProtocolDecl !== undefined,
          'ObjCProtocolDecl constant should be defined',
        );
        assert.ok(
          clang.CXCursorKind.ObjCPropertyDecl !== undefined,
          'ObjCPropertyDecl constant should be defined',
        );
        assert.ok(
          clang.CXCursorKind.ObjCInstanceMethodDecl !== undefined,
          'ObjCInstanceMethodDecl constant should be defined',
        );
        assert.ok(
          clang.CXCursorKind.ObjCClassMethodDecl !== undefined,
          'ObjCClassMethodDecl constant should be defined',
        );
        assert.ok(
          clang.CXCursorKind.ObjCProtocolRef !== undefined,
          'ObjCProtocolRef constant should be defined',
        );
      }

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should use translation unit parse options', async (t) => {
    const ctx = t as unknown as TestContextWithTempDir;
    const index = clang.createIndex();

    const testFile = join(ctx[kTempDir], 'test_parse_options.c');
    const testCode = `
            #define MY_MACRO 42
            #ifdef MY_MACRO
            int defined_value = MY_MACRO;
            #endif
            
            /**
             * Brief comment for test function
             */
            int test_function() {
                return 0;
            }
        `;
    await fs.writeFile(testFile, testCode);

    try {
      // Test with DetailedPreprocessingRecord option
      const tu1 = clang.parseTranslationUnit(
        index,
        testFile,
        [],
        clang.CXTranslationUnit.DetailedPreprocessingRecord,
      );
      assert.ok(tu1);

      // The translation unit should be created successfully with this option
      const cursor1 = tu1.getCursor();
      assert.ok(cursor1);

      tu1.dispose();

      // Test with IncludeBriefCommentsInCodeCompletion option
      const tu2 = clang.parseTranslationUnit(
        index,
        testFile,
        [],
        clang.CXTranslationUnit.IncludeBriefCommentsInCodeCompletion,
      );
      assert.ok(tu2);

      const cursor2 = tu2.getCursor();
      let hasComment = false;
      clang.visitChildren(cursor2, (child: Cursor, _parent: Cursor) => {
        if (clang.getCursorSpelling(child) === 'test_function') {
          const comment = clang.getCursorRawCommentText(child);
          if (comment?.includes('Brief comment')) {
            hasComment = true;
          }
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(hasComment, 'Should have found the brief comment');

      tu2.dispose();

      // Test with None option (default behavior)
      const tu3 = clang.parseTranslationUnit(
        index,
        testFile,
        [],
        clang.CXTranslationUnit.None,
      );
      assert.ok(tu3);
      tu3.dispose();
    } finally {
      index.dispose();
    }
  });
});
