import nodeGypBuild from 'node-gyp-build';

// Type definitions
export interface Cursor {
  _cursor: unknown;
  _tu?: unknown;
}

export interface Type {
  _type: unknown;
}

export interface Location {
  file: string | null;
  line: number;
  column: number;
  offset: number;
}

export type VisitorCallback = (cursor: Cursor, parent: Cursor) => number;

// Class interfaces
export interface IClangIndex {
  dispose(): void;
}

export interface IClangTranslationUnit {
  dispose(): void;
  getCursor(): Cursor;
}

// Load the native addon
const addon = nodeGypBuild(import.meta.dirname) as {
  // Core functions
  createIndex(
    excludeDeclarationsFromPCH?: boolean,
    displayDiagnostics?: boolean,
  ): IClangIndex;
  parseTranslationUnit(
    index: IClangIndex,
    sourceFilename: string,
    commandLineArgs?: string[],
    options?: number,
  ): IClangTranslationUnit;

  // Cursor functions
  getCursorKind(cursor: Cursor): number;
  getCursorSpelling(cursor: Cursor): string;
  getCursorLocation(cursor: Cursor): Location;
  getCursorType(cursor: Cursor): Type;
  visitChildren(cursor: Cursor, visitor: VisitorCallback): number;

  // Type functions
  getTypeSpelling(type: Type): string;
  getTypeNullability(type: Type): number;
  getResultType(type: Type): Type;
  getNumArgTypes(type: Type): number;
  getArgType(type: Type, index: number): Type;

  // Documentation
  getCursorRawCommentText(cursor: Cursor): string | null;

  // Enum functions
  getEnumConstantDeclValue(cursor: Cursor): number;

  // Typedef functions
  getTypedefDeclUnderlyingType(cursor: Cursor): Type;

  // Constants
  CXChildVisit_Break: number;
  CXChildVisit_Continue: number;
  CXChildVisit_Recurse: number;

  CXCursor_UnexposedDecl: number;
  CXCursor_StructDecl: number;
  CXCursor_UnionDecl: number;
  CXCursor_ClassDecl: number;
  CXCursor_EnumDecl: number;
  CXCursor_FieldDecl: number;
  CXCursor_FunctionDecl: number;
  CXCursor_VarDecl: number;
  CXCursor_ParmDecl: number;
  CXCursor_TypedefDecl: number;
  CXCursor_CXXMethod: number;
  CXCursor_EnumConstantDecl: number;

  // Classes
  ClangIndex: new (
    excludeDeclarationsFromPCH?: boolean,
    displayDiagnostics?: boolean,
  ) => IClangIndex;
  ClangTranslationUnit: new () => IClangTranslationUnit;
};

// Core functions
export const createIndex = addon.createIndex;
export const parseTranslationUnit = addon.parseTranslationUnit;

// Cursor functions
export const getCursorKind = addon.getCursorKind;
export const getCursorSpelling = addon.getCursorSpelling;
export const getCursorLocation = addon.getCursorLocation;
export const getCursorType = addon.getCursorType;
export const visitChildren = addon.visitChildren;

// Type functions
export const getTypeSpelling = addon.getTypeSpelling;
export const getTypeNullability = addon.getTypeNullability;
export const getResultType = addon.getResultType;
export const getNumArgTypes = addon.getNumArgTypes;
export const getArgType = addon.getArgType;

// Documentation
export const getCursorRawCommentText = addon.getCursorRawCommentText;

// Enum functions
export const getEnumConstantDeclValue = addon.getEnumConstantDeclValue;

// Typedef functions
export const getTypedefDeclUnderlyingType = addon.getTypedefDeclUnderlyingType;

// Constants
export const CXChildVisit = {
  Break: addon.CXChildVisit_Break,
  Continue: addon.CXChildVisit_Continue,
  Recurse: addon.CXChildVisit_Recurse,
} as const;

export const CXCursorKind = {
  UnexposedDecl: addon.CXCursor_UnexposedDecl,
  StructDecl: addon.CXCursor_StructDecl,
  UnionDecl: addon.CXCursor_UnionDecl,
  ClassDecl: addon.CXCursor_ClassDecl,
  EnumDecl: addon.CXCursor_EnumDecl,
  FieldDecl: addon.CXCursor_FieldDecl,
  FunctionDecl: addon.CXCursor_FunctionDecl,
  VarDecl: addon.CXCursor_VarDecl,
  ParmDecl: addon.CXCursor_ParmDecl,
  TypedefDecl: addon.CXCursor_TypedefDecl,
  CXXMethod: addon.CXCursor_CXXMethod,
  EnumConstantDecl: addon.CXCursor_EnumConstantDecl,
} as const;

// Re-export classes from addon
export const ClangIndex = addon.ClangIndex;
export const ClangTranslationUnit = addon.ClangTranslationUnit;

// Default export for convenience
export default {
  createIndex,
  parseTranslationUnit,
  getCursorKind,
  getCursorSpelling,
  getCursorLocation,
  getCursorType,
  visitChildren,
  getTypeSpelling,
  getTypeNullability,
  getResultType,
  getNumArgTypes,
  getArgType,
  getCursorRawCommentText,
  getEnumConstantDeclValue,
  getTypedefDeclUnderlyingType,
  CXChildVisit,
  CXCursorKind,
  ClangIndex,
  ClangTranslationUnit,
};
