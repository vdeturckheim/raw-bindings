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
  getTypeKind(type: Type): number;
  getPointeeType(type: Type): Type;
  getCanonicalType(type: Type): Type;
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

  // Cursor argument functions
  getCursorResultType(cursor: Cursor): Type;
  getNumCursorArguments(cursor: Cursor): number;
  getCursorArgument(cursor: Cursor, index: number): Cursor;

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

  // Objective-C cursor kinds
  CXCursor_ObjCInterfaceDecl: number;
  CXCursor_ObjCCategoryDecl: number;
  CXCursor_ObjCProtocolDecl: number;
  CXCursor_ObjCPropertyDecl: number;
  CXCursor_ObjCInstanceMethodDecl: number;
  CXCursor_ObjCClassMethodDecl: number;
  CXCursor_ObjCProtocolRef: number;

  // Parse options
  CXTranslationUnit_None: number;
  CXTranslationUnit_DetailedPreprocessingRecord: number;
  CXTranslationUnit_IncludeBriefCommentsInCodeCompletion: number;
  
  // Type kind constants
  CXType_Invalid: number;
  CXType_Unexposed: number;
  CXType_Void: number;
  CXType_Bool: number;
  CXType_Char_U: number;
  CXType_UChar: number;
  CXType_UShort: number;
  CXType_UInt: number;
  CXType_ULong: number;
  CXType_ULongLong: number;
  CXType_Char_S: number;
  CXType_SChar: number;
  CXType_Short: number;
  CXType_Int: number;
  CXType_Long: number;
  CXType_LongLong: number;
  CXType_Float: number;
  CXType_Double: number;
  CXType_LongDouble: number;
  CXType_Pointer: number;
  CXType_BlockPointer: number;
  CXType_LValueReference: number;
  CXType_RValueReference: number;
  CXType_Record: number;
  CXType_Enum: number;
  CXType_Typedef: number;
  CXType_FunctionProto: number;
  CXType_FunctionNoProto: number;
  CXType_ConstantArray: number;
  CXType_IncompleteArray: number;
  CXType_VariableArray: number;
  CXType_DependentSizedArray: number;
  CXType_Elaborated: number;

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
export const getTypeKind = addon.getTypeKind;
export const getPointeeType = addon.getPointeeType;
export const getCanonicalType = addon.getCanonicalType;
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

// Cursor argument functions
export const getCursorResultType = addon.getCursorResultType;
export const getNumCursorArguments = addon.getNumCursorArguments;
export const getCursorArgument = addon.getCursorArgument;

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
  // Objective-C kinds
  ObjCInterfaceDecl: addon.CXCursor_ObjCInterfaceDecl,
  ObjCCategoryDecl: addon.CXCursor_ObjCCategoryDecl,
  ObjCProtocolDecl: addon.CXCursor_ObjCProtocolDecl,
  ObjCPropertyDecl: addon.CXCursor_ObjCPropertyDecl,
  ObjCInstanceMethodDecl: addon.CXCursor_ObjCInstanceMethodDecl,
  ObjCClassMethodDecl: addon.CXCursor_ObjCClassMethodDecl,
  ObjCProtocolRef: addon.CXCursor_ObjCProtocolRef,
} as const;

// Parse options
export const CXTranslationUnit = {
  None: addon.CXTranslationUnit_None,
  DetailedPreprocessingRecord:
    addon.CXTranslationUnit_DetailedPreprocessingRecord,
  IncludeBriefCommentsInCodeCompletion:
    addon.CXTranslationUnit_IncludeBriefCommentsInCodeCompletion,
} as const;

// Type kinds
export const CXTypeKind = {
  Invalid: addon.CXType_Invalid,
  Unexposed: addon.CXType_Unexposed,
  Void: addon.CXType_Void,
  Bool: addon.CXType_Bool,
  Char_U: addon.CXType_Char_U,
  UChar: addon.CXType_UChar,
  UShort: addon.CXType_UShort,
  UInt: addon.CXType_UInt,
  ULong: addon.CXType_ULong,
  ULongLong: addon.CXType_ULongLong,
  Char_S: addon.CXType_Char_S,
  SChar: addon.CXType_SChar,
  Short: addon.CXType_Short,
  Int: addon.CXType_Int,
  Long: addon.CXType_Long,
  LongLong: addon.CXType_LongLong,
  Float: addon.CXType_Float,
  Double: addon.CXType_Double,
  LongDouble: addon.CXType_LongDouble,
  Pointer: addon.CXType_Pointer,
  BlockPointer: addon.CXType_BlockPointer,
  LValueReference: addon.CXType_LValueReference,
  RValueReference: addon.CXType_RValueReference,
  Record: addon.CXType_Record,
  Enum: addon.CXType_Enum,
  Typedef: addon.CXType_Typedef,
  FunctionProto: addon.CXType_FunctionProto,
  FunctionNoProto: addon.CXType_FunctionNoProto,
  ConstantArray: addon.CXType_ConstantArray,
  IncompleteArray: addon.CXType_IncompleteArray,
  VariableArray: addon.CXType_VariableArray,
  DependentSizedArray: addon.CXType_DependentSizedArray,
  Elaborated: addon.CXType_Elaborated,
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
  getTypeKind,
  getPointeeType,
  getCanonicalType,
  getTypeNullability,
  getResultType,
  getNumArgTypes,
  getArgType,
  getCursorRawCommentText,
  getEnumConstantDeclValue,
  getTypedefDeclUnderlyingType,
  getCursorResultType,
  getNumCursorArguments,
  getCursorArgument,
  CXChildVisit,
  CXCursorKind,
  CXTypeKind,
  CXTranslationUnit,
  ClangIndex,
  ClangTranslationUnit,
};
