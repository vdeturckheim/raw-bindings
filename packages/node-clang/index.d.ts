declare module 'node-clang' {
  // Core classes
  export class ClangIndex {
    constructor(
      excludeDeclarationsFromPCH?: boolean,
      displayDiagnostics?: boolean,
    );
    dispose(): void;
  }

  export class ClangTranslationUnit {
    dispose(): void;
    getCursor(): Cursor;
  }

  // Cursor type
  export interface Cursor {
    _cursor: any;
    _tu?: any;
  }

  // Type type
  export interface Type {
    _type: any;
  }

  // Location type
  export interface Location {
    file: string | null;
    line: number;
    column: number;
    offset: number;
  }

  // Core functions
  export function createIndex(
    excludeDeclarationsFromPCH?: boolean,
    displayDiagnostics?: boolean,
  ): ClangIndex;
  export function parseTranslationUnit(
    index: ClangIndex,
    sourceFilename: string,
    commandLineArgs?: string[],
    options?: number,
  ): ClangTranslationUnit;

  // Cursor functions
  export function getCursorKind(cursor: Cursor): number;
  export function getCursorSpelling(cursor: Cursor): string;
  export function getCursorLocation(cursor: Cursor): Location;
  export function getCursorType(cursor: Cursor): Type;
  export function visitChildren(
    cursor: Cursor,
    visitor: VisitorCallback,
  ): number;

  // Type functions
  export function getTypeSpelling(type: Type): string;
  export function getTypeNullability(type: Type): number;
  export function getResultType(type: Type): Type;
  export function getNumArgTypes(type: Type): number;
  export function getArgType(type: Type, index: number): Type;

  // Documentation
  export function getCursorRawCommentText(cursor: Cursor): string | null;

  // Enum functions
  export function getEnumConstantDeclValue(cursor: Cursor): number;

  // Typedef functions
  export function getTypedefDeclUnderlyingType(cursor: Cursor): Type;

  // Visitor callback type
  export type VisitorCallback = (
    cursor: Cursor,
    parent: Cursor,
  ) => CXChildVisitResult;

  // Enums
  export enum CXChildVisitResult {
    Break = 0,
    Continue = 1,
    Recurse = 2,
  }

  export const CXChildVisit: {
    Break: number;
    Continue: number;
    Recurse: number;
  };

  export const CXCursorKind: {
    UnexposedDecl: number;
    StructDecl: number;
    UnionDecl: number;
    ClassDecl: number;
    EnumDecl: number;
    FieldDecl: number;
    FunctionDecl: number;
    VarDecl: number;
    ParmDecl: number;
    TypedefDecl: number;
    CXXMethod: number;
    EnumConstantDecl: number;
  };

  // Default export
  const nodeClang: {
    createIndex: typeof createIndex;
    parseTranslationUnit: typeof parseTranslationUnit;
    getCursorKind: typeof getCursorKind;
    getCursorSpelling: typeof getCursorSpelling;
    getCursorLocation: typeof getCursorLocation;
    getCursorType: typeof getCursorType;
    visitChildren: typeof visitChildren;
    getTypeSpelling: typeof getTypeSpelling;
    getTypeNullability: typeof getTypeNullability;
    getResultType: typeof getResultType;
    getNumArgTypes: typeof getNumArgTypes;
    getArgType: typeof getArgType;
    getCursorRawCommentText: typeof getCursorRawCommentText;
    getEnumConstantDeclValue: typeof getEnumConstantDeclValue;
    getTypedefDeclUnderlyingType: typeof getTypedefDeclUnderlyingType;
    CXChildVisit: typeof CXChildVisit;
    CXCursorKind: typeof CXCursorKind;
    ClangIndex: typeof ClangIndex;
    ClangTranslationUnit: typeof ClangTranslationUnit;
  };

  export default nodeClang;
}
