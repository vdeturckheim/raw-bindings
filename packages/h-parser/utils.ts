import { execSync } from 'node:child_process';
import * as clang from 'node-clang-raw';
import type { TypeInfo } from './types.ts';

/**
 * Get the macOS SDK path using xcrun
 */
export function getSDKPath(): string | null {
  try {
    const sdkPath = execSync('xcrun --show-sdk-path', {
      encoding: 'utf-8',
    }).trim();
    return sdkPath || null;
  } catch {
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
export function getTypeInfo(type: any): TypeInfo {
  // Helper to get string from CXString
  function getStringFromCXString(cxString: any): string {
    if (!cxString || !cxString._ptr) return '';
    const str = clang.clang_getCString(cxString);
    clang.clang_disposeString(cxString);
    return str || '';
  }

  const info: TypeInfo = {
    spelling: getStringFromCXString(clang.clang_getTypeSpelling(type)),
  };

  // Get nullability if available
  const nullability = clang.clang_Type_getNullability(type);
  const nullabilityMap: Record<number, TypeInfo['nullability'] | undefined> = {
    0: 'nonnull',
    1: 'nullable',
    2: 'unspecified',
    3: 'nullable_result',
  };

  if (nullability in nullabilityMap) {
    const value = nullabilityMap[nullability];
    if (value !== undefined) {
      info.nullability = value;
    }
  }

  return info;
}

/**
 * Get cursor kind name as a string
 */
export function getCursorKindName(cursor: any): string {
  const kind = clang.clang_getCursorKind(cursor);

  // Map numeric kinds to string names
  const kindMap: Record<number, string> = {
    [clang.CXCursor_UnexposedDecl]: 'UNEXPOSED_DECL',
    [clang.CXCursor_StructDecl]: 'STRUCT_DECL',
    [clang.CXCursor_UnionDecl]: 'UNION_DECL',
    [clang.CXCursor_ClassDecl]: 'CLASS_DECL',
    [clang.CXCursor_EnumDecl]: 'ENUM_DECL',
    [clang.CXCursor_FieldDecl]: 'FIELD_DECL',
    [clang.CXCursor_FunctionDecl]: 'FUNCTION_DECL',
    [clang.CXCursor_VarDecl]: 'VAR_DECL',
    [clang.CXCursor_ParmDecl]: 'PARM_DECL',
    [clang.CXCursor_TypedefDecl]: 'TYPEDEF_DECL',
    [clang.CXCursor_CXXMethod]: 'CXX_METHOD',
    [clang.CXCursor_EnumConstantDecl]: 'ENUM_CONSTANT_DECL',
    [clang.CXCursor_ObjCInterfaceDecl]: 'OBJC_INTERFACE_DECL',
    [clang.CXCursor_ObjCCategoryDecl]: 'OBJC_CATEGORY_DECL',
    [clang.CXCursor_ObjCProtocolDecl]: 'OBJC_PROTOCOL_DECL',
    [clang.CXCursor_ObjCPropertyDecl]: 'OBJC_PROPERTY_DECL',
    [clang.CXCursor_ObjCInstanceMethodDecl]: 'OBJC_INSTANCE_METHOD_DECL',
    [clang.CXCursor_ObjCClassMethodDecl]: 'OBJC_CLASS_METHOD_DECL',
    [clang.CXCursor_ObjCProtocolRef]: 'OBJC_PROTOCOL_REF',
  };

  return kindMap[kind] || `UNKNOWN_${kind}`;
}

/**
 * Check if cursor is from a specific file
 */
export function isCursorFromFile(
  cursor: any,
  filePath: string,
): boolean {
  // TODO: Implement once output parameters are supported in bindings
  // For now, return true to allow parsing
  return true;
}

/**
 * Build compiler arguments based on parse options
 */
export function buildCompilerArgs(
  options: import('./types.ts').ParseOptions = {},
): string[] {
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
export function buildParseOptions(
  options: import('./types.ts').ParseOptions = {},
): number {
  let flags = clang.CXTranslationUnit_None || 0;

  if (options.detailedProcessing !== false) {
    flags |= clang.CXTranslationUnit_DetailedPreprocessingRecord || 0x01;
  }

  if (options.includeDocumentation !== false) {
    flags |= clang.CXTranslationUnit_IncludeBriefCommentsInCodeCompletion || 0x40;
  }

  return flags;
}
