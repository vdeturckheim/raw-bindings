import assert from 'node:assert/strict';
import fs from 'node:fs/promises';
import { tmpdir } from 'node:os';
import { join } from 'node:path';
import { beforeEach, describe, it } from 'node:test';
import * as clang from './index.js';

const kTempDir = Symbol('kTempDir');

describe('node-clang bindings', () => {
  beforeEach(async (t) => {
    const baseDir = tmpdir();
    const prefix = 'node-clang-test-';
    t[kTempDir] = await fs.mkdtemp(join(baseDir, prefix));
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
    const index = clang.createIndex();

    // Create a test file
    const testFile = join(t[kTempDir], 'test_clang.c');
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
    await fs.writeFile(index, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      assert.ok(tu);
      assert.ok(tu instanceof clang.ClangTranslationUnit);

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get translation unit cursor and visit children', async () => {
    const index = clang.createIndex();

    const testFile = join(tmpdir(), 'test_visitor.c');
    const testCode = `
            int globalVar = 42;
            
            void testFunction() {
                int localVar = 10;
            }
        `;
    await fs.writeFile(index, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      assert.ok(cursor);
      assert.ok(cursor._cursor);

      const declarations = [];
      clang.visitChildren(cursor, (child, parent) => {
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

  it('should get cursor location information', async () => {
    const index = clang.createIndex();

    const testFile = join(tmpdir(), 'test_location.c');
    const testCode = `int myFunction() { return 0; }`;
    await fs.writeFile(index, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      let functionCursor = null;
      clang.visitChildren(cursor, (child, parent) => {
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
      assert.ok(location.file.includes('test_location.c'));

      tu.dispose();
    } finally {
      index.dispose();
    }
  });

  it('should get type information', () => {
    const index = clang.createIndex();

    const testFile = join(tmpdir(), 'test_types.c');
    const testCode = `
            float calculate(double x, int y) {
                return x + y;
            }
        `;
    writeFileSync(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      let functionCursor = null;
      clang.visitChildren(cursor, (child, parent) => {
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
      unlinkSync(testFile);
    }
  });

  it('should get enum constant values', () => {
    const index = clang.createIndex();

    const testFile = join(tmpdir(), 'test_enum.c');
    const testCode = `
            enum Status {
                SUCCESS = 0,
                ERROR = -1,
                PENDING = 42
            };
        `;
    writeFileSync(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      const enumValues = {};
      clang.visitChildren(cursor, (child, parent) => {
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
      unlinkSync(testFile);
    }
  });

  it('should get typedef underlying type', () => {
    const index = clang.createIndex();

    const testFile = join(tmpdir(), 'test_typedef.c');
    const testCode = `
            typedef unsigned long size_t;
            typedef int* IntPtr;
        `;
    writeFileSync(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      const typedefs = {};
      clang.visitChildren(cursor, (child, parent) => {
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
      unlinkSync(testFile);
    }
  });

  it('should get raw comment text', () => {
    const index = clang.createIndex();

    const testFile = join(tmpdir(), 'test_comments.c');
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
    writeFileSync(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      let comment = null;
      clang.visitChildren(cursor, (child, parent) => {
        if (clang.getCursorSpelling(child) === 'add') {
          comment = clang.getCursorRawCommentText(child);
          return clang.CXChildVisit.Break;
        }
        return clang.CXChildVisit.Continue;
      });

      assert.ok(comment);
      assert.ok(comment.includes('This function adds two numbers'));
      assert.ok(comment.includes('@param a First number'));
      assert.ok(comment.includes('@return The sum of a and b'));

      tu.dispose();
    } finally {
      index.dispose();
      unlinkSync(testFile);
    }
  });

  it('should handle visitor break correctly', () => {
    const index = clang.createIndex();

    const testFile = join(tmpdir(), 'test_break.c');
    const testCode = `
            int a;
            int b;
            int c;
        `;
    writeFileSync(testFile, testCode);

    try {
      const tu = clang.parseTranslationUnit(index, testFile, []);
      const cursor = tu.getCursor();

      const found = [];
      clang.visitChildren(cursor, (child, parent) => {
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
      unlinkSync(testFile);
    }
  });
});
