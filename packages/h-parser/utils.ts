import * as clang from 'node-clang';
import { execSync } from 'child_process';
import type { TypeInfo } from './types.ts';

/**
 * Get the macOS SDK path using xcrun
 */
export function getSDKPath(): string | null {
  try {
    const sdkPath = execSync('xcrun --show-sdk-path', { encoding: 'utf-8' }).trim();
    return sdkPath || null;
  } catch (error) {
    return null;
  }
}

/**
 * Clean documentation comment by removing comment markers
 */
export function cleanDocumentation(comment: string | null): string | undefined {
  if (!comment) return undefined;

  const lines = comment.split('\n');
  const cleanedLines: string[] = [];

  for (let line of lines) {
    line = line.trim();
    
    // Remove /// markers
    if (line.startsWith('///')) {
      cleanedLines.push(line.slice(3).trim());
    }
    // Remove /** markers
    else if (line.startsWith('/**')) {
      line = line.slice(3).trim();
      if (line.endsWith('*/')) {
        line = line.slice(0, -2).trim();
      }
      if (line) {
        cleanedLines.push(line);
      }
    }
    // Remove * markers (continuation of block comment)
    else if (line.startsWith('*')) {
      line = line.slice(1).trim();
      if (line.endsWith('*/')) {
        line = line.slice(0, -2).trim();
      }
      if (line !== '/') {
        cleanedLines.push(line);
      }
    }
    // Remove */ at end
    else if (line.endsWith('*/')) {
      line = line.slice(0, -2).trim();
      if (line) {
        cleanedLines.push(line);
      }
    }
  }

  const doc = cleanedLines.join('\n').trim();
  return doc || undefined;
}

/**
 * Extract type information from a libclang type
 */
export function getTypeInfo(type: clang.Type): TypeInfo {
  const info: TypeInfo = {
    spelling: clang.getTypeSpelling(type)
  };

  // Get nullability if available
  const nullability = clang.getTypeNullability(type);
  const nullabilityMap: Record<number, TypeInfo['nullability']> = {
    0: 'nonnull',
    1: 'nullable',
    2: 'unspecified',
    3: 'nullable_result'
  };

  if (nullability in nullabilityMap) {
    info.nullability = nullabilityMap[nullability];
  }

  return info;
}

/**
 * Get cursor kind name as a string
 */
export function getCursorKindName(cursor: clang.Cursor): string {
  const kind = clang.getCursorKind(cursor);
  
  // Map numeric kinds to string names
  const kindMap: Record<number, string> = {
    [clang.CXCursorKind.UnexposedDecl]: 'UNEXPOSED_DECL',
    [clang.CXCursorKind.StructDecl]: 'STRUCT_DECL',
    [clang.CXCursorKind.UnionDecl]: 'UNION_DECL',
    [clang.CXCursorKind.ClassDecl]: 'CLASS_DECL',
    [clang.CXCursorKind.EnumDecl]: 'ENUM_DECL',
    [clang.CXCursorKind.FieldDecl]: 'FIELD_DECL',
    [clang.CXCursorKind.FunctionDecl]: 'FUNCTION_DECL',
    [clang.CXCursorKind.VarDecl]: 'VAR_DECL',
    [clang.CXCursorKind.ParmDecl]: 'PARM_DECL',
    [clang.CXCursorKind.TypedefDecl]: 'TYPEDEF_DECL',
    [clang.CXCursorKind.CXXMethod]: 'CXX_METHOD',
    [clang.CXCursorKind.EnumConstantDecl]: 'ENUM_CONSTANT_DECL',
    [clang.CXCursorKind.ObjCInterfaceDecl]: 'OBJC_INTERFACE_DECL',
    [clang.CXCursorKind.ObjCCategoryDecl]: 'OBJC_CATEGORY_DECL',
    [clang.CXCursorKind.ObjCProtocolDecl]: 'OBJC_PROTOCOL_DECL',
    [clang.CXCursorKind.ObjCPropertyDecl]: 'OBJC_PROPERTY_DECL',
    [clang.CXCursorKind.ObjCInstanceMethodDecl]: 'OBJC_INSTANCE_METHOD_DECL',
    [clang.CXCursorKind.ObjCClassMethodDecl]: 'OBJC_CLASS_METHOD_DECL',
    [clang.CXCursorKind.ObjCProtocolRef]: 'OBJC_PROTOCOL_REF'
  };

  return kindMap[kind] || `UNKNOWN_${kind}`;
}

/**
 * Check if cursor is from a specific file
 */
export function isCursorFromFile(cursor: clang.Cursor, filePath: string): boolean {
  const location = clang.getCursorLocation(cursor);
  return location.file === filePath;
}

/**
 * Build compiler arguments based on parse options
 */
export function buildCompilerArgs(options: import('./types.ts').ParseOptions = {}): string[] {
  const args: string[] = [];

  // Language specification
  const lang = options.language || 'objective-c';
  args.push('-x', lang);

  // Objective-C specific flags
  if (lang.includes('objective-c')) {
    args.push('-fobjc-arc', '-fblocks');
  }

  // System root
  if (options.systemRoot) {
    args.push('-isysroot', options.systemRoot);
  }

  // Include paths
  if (options.includePaths) {
    for (const path of options.includePaths) {
      args.push('-I', path);
    }
  }
  
  // Additional include paths (alias for includePaths)
  if (options.additionalIncludePaths) {
    for (const path of options.additionalIncludePaths) {
      args.push('-I', path);
    }
  }

  // Framework paths
  if (options.frameworkPaths) {
    for (const path of options.frameworkPaths) {
      args.push('-F', path);
    }
  }

  // Frameworks
  if (options.frameworks) {
    for (const framework of options.frameworks) {
      args.push('-framework', framework);
    }
  }

  // Defines
  if (options.defines) {
    for (const define of options.defines) {
      args.push('-D', define);
    }
  }

  // Additional arguments
  if (options.additionalArgs) {
    args.push(...options.additionalArgs);
  }

  return args;
}

/**
 * Build parse options flags for libclang
 */
export function buildParseOptions(options: import('./types.ts').ParseOptions = {}): number {
  let flags = clang.CXTranslationUnit.None;

  if (options.detailedProcessing !== false) {
    flags |= clang.CXTranslationUnit.DetailedPreprocessingRecord;
  }

  if (options.includeDocumentation !== false) {
    flags |= clang.CXTranslationUnit.IncludeBriefCommentsInCodeCompletion;
  }

  return flags;
}