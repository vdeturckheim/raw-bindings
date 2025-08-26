import nodeGypBuild from 'node-gyp-build';

const addon = nodeGypBuild(import.meta.dirname); // looks for build/Release/*.node (or prebuilds/)

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
};

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
};

// Classes
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
