import { basename } from 'node:path';
import * as clang from 'node-clang-raw';
import type {
  Enum,
  EnumConstant,
  Function as FunctionDecl,
  HeaderAST,
  Interface,
  Method,
  MethodParam,
  ParseOptions,
  Property,
  Protocol,
  Struct,
  StructField,
  Typedef,
} from './types.ts';
import {
  buildCompilerArgs,
  buildParseOptions,
  cleanDocumentation,
} from './utils.ts';

// Helper to get string from CXString
function getStringFromCXString(cxString: any): string {
  if (!cxString || !cxString._ptr) return '';
  try {
    const str = clang.clang_getCString(cxString);
    clang.clang_disposeString(cxString);
    return str || '';
  } catch (e) {
    // Handle null string case
    return '';
  }
}

// Helper to get type info
function getTypeInfo(type: any): { spelling: string; nullability?: string } {
  const spelling = getStringFromCXString(clang.clang_getTypeSpelling(type));
  const info: any = { spelling };
  
  // Get nullability if available
  const nullability = clang.clang_Type_getNullability(type);
  const nullabilityMap: Record<number, string> = {
    0: 'nonnull',
    1: 'nullable',
    2: 'unspecified',
    3: 'nullable_result',
  };
  
  if (nullability in nullabilityMap) {
    info.nullability = nullabilityMap[nullability];
  }
  
  return info;
}

/**
 * Collect method information from a cursor
 */
function collectMethod(cursor: any): Method {
  const kind = clang.clang_getCursorKind(cursor);
  const isClassMethod = kind === clang.CXCursor_ObjCClassMethodDecl;

  const method: Method = {
    kind: isClassMethod ? 'class' : 'instance',
    selector: getStringFromCXString(clang.clang_getCursorSpelling(cursor)),
    return: getTypeInfo(clang.clang_getCursorResultType(cursor)),
    params: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    method.documentation = doc;
  }

  // Get method arguments
  const numArgs = clang.clang_Cursor_getNumArguments(cursor);
  for (let i = 0; i < numArgs; i++) {
    const argCursor = clang.clang_Cursor_getArgument(cursor, i);
    const param: MethodParam = {
      name: getStringFromCXString(clang.clang_getCursorSpelling(argCursor)) || '',
      type: getTypeInfo(clang.clang_getCursorType(argCursor)),
    };
    method.params.push(param);
  }

  return method;
}

/**
 * Collect property information from a cursor
 */
function collectProperty(cursor: any): Property {
  const prop: Property = {
    name: getStringFromCXString(clang.clang_getCursorSpelling(cursor)),
    type: getTypeInfo(clang.clang_getCursorType(cursor)),
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    prop.documentation = doc;
  }

  return prop;
}

/**
 * Collect Objective-C interface or C++ class information
 */
function collectInterface(cursor: any): Interface {
  const iface: Interface = {
    name: getStringFromCXString(clang.clang_getCursorSpelling(cursor)),
    methods: [],
    properties: [],
    protocols: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    iface.documentation = doc;
  }

  // Visit children to collect methods, properties, and protocol references
  clang.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = clang.clang_getCursorKind(child);

    if (
      kind === clang.CXCursor_ObjCInstanceMethodDecl ||
      kind === clang.CXCursor_ObjCClassMethodDecl ||
      kind === clang.CXCursor_CXXMethod
    ) {
      iface.methods.push(collectMethod(child));
    } else if (kind === clang.CXCursor_ObjCPropertyDecl) {
      iface.properties.push(collectProperty(child));
    } else if (kind === clang.CXCursor_ObjCProtocolRef) {
      iface.protocols.push(getStringFromCXString(clang.clang_getCursorSpelling(child)));
    }

    return clang.CXChildVisit_Continue;
  }, null);

  return iface;
}

/**
 * Collect protocol information
 */
function collectProtocol(cursor: any): Protocol {
  const protocol: Protocol = {
    name: getStringFromCXString(clang.clang_getCursorSpelling(cursor)),
    methods: [],
    properties: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    protocol.documentation = doc;
  }

  // Visit children to collect methods and properties
  clang.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = clang.clang_getCursorKind(child);

    if (
      kind === clang.CXCursor_ObjCInstanceMethodDecl ||
      kind === clang.CXCursor_ObjCClassMethodDecl
    ) {
      if (!protocol.methods) protocol.methods = [];
      protocol.methods.push(collectMethod(child));
    } else if (kind === clang.CXCursor_ObjCPropertyDecl) {
      if (!protocol.properties) protocol.properties = [];
      protocol.properties.push(collectProperty(child));
    }

    return clang.CXChildVisit_Continue;
  }, null);

  return protocol;
}

/**
 * Collect enum information
 */
function collectEnum(cursor: any): Enum {
  const enumDecl: Enum = {
    name: getStringFromCXString(clang.clang_getCursorSpelling(cursor)) || '',
    constants: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    enumDecl.documentation = doc;
  }

  // Visit children to collect enum constants
  clang.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = clang.clang_getCursorKind(child);

    if (kind === clang.CXCursor_EnumConstantDecl) {
      const constant: EnumConstant = {
        name: getStringFromCXString(clang.clang_getCursorSpelling(child)),
        value: null,
      };

      try {
        const val = clang.clang_getEnumConstantDeclValue(child);
        // Convert BigInt to number or string for JSON serialization
        if (typeof val === 'bigint') {
          // Convert to number if it fits, otherwise to string
          if (val >= Number.MIN_SAFE_INTEGER && val <= Number.MAX_SAFE_INTEGER) {
            constant.value = Number(val);
          } else {
            constant.value = val.toString();
          }
        } else {
          constant.value = val;
        }
      } catch {
        // Value might not be available
      }

      // Add documentation for constant if available
      const constDoc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(child)));
      if (constDoc) {
        constant.documentation = constDoc;
      }

      enumDecl.constants.push(constant);
    }

    return clang.CXChildVisit_Continue;
  }, null);

  return enumDecl;
}

/**
 * Collect struct information
 */
function collectStruct(cursor: any): Struct {
  const struct: Struct = {
    name: getStringFromCXString(clang.clang_getCursorSpelling(cursor)),
    fields: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    struct.documentation = doc;
  }

  // Visit children to collect fields
  clang.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = clang.clang_getCursorKind(child);

    if (kind === clang.CXCursor_FieldDecl) {
      const field: StructField = {
        name: getStringFromCXString(clang.clang_getCursorSpelling(child)),
        type: getStringFromCXString(clang.clang_getTypeSpelling(clang.clang_getCursorType(child))),
      };
      struct.fields.push(field);
    }

    return clang.CXChildVisit_Continue;
  }, null);

  return struct;
}

/**
 * Collect typedef information
 */
function collectTypedef(cursor: any): Typedef {
  const typedef: Typedef = {
    name: getStringFromCXString(clang.clang_getCursorSpelling(cursor)),
    underlying: getStringFromCXString(clang.clang_getTypeSpelling(clang.clang_getTypedefDeclUnderlyingType(cursor))),
    spelling: getStringFromCXString(clang.clang_getTypeSpelling(clang.clang_getCursorType(cursor))),
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    typedef.documentation = doc;
  }

  return typedef;
}

/**
 * Collect function information
 */
function collectFunction(cursor: any): FunctionDecl {
  const func: FunctionDecl = {
    name: getStringFromCXString(clang.clang_getCursorSpelling(cursor)),
    return: getTypeInfo(clang.clang_getCursorResultType(cursor)),
    params: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(clang.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    func.documentation = doc;
  }

  // Get function arguments
  const numArgs = clang.clang_Cursor_getNumArguments(cursor);
  for (let i = 0; i < numArgs; i++) {
    const argCursor = clang.clang_Cursor_getArgument(cursor, i);
    const param: MethodParam = {
      name: getStringFromCXString(clang.clang_getCursorSpelling(argCursor)) || '',
      type: getTypeInfo(clang.clang_getCursorType(argCursor)),
    };
    func.params.push(param);
  }

  return func;
}

/**
 * Parse a header file and return structured AST
 */
export function parseHeader(
  headerPath: string,
  options: ParseOptions = {},
): HeaderAST {
  // Create index
  const index = clang.clang_createIndex(
    options.excludeDeclarationsFromPCH ? 1 : 0,
    options.displayDiagnostics ? 1 : 0
  );

  try {
    // Build compiler arguments
    const args = buildCompilerArgs(options);

    // Build parse options
    const parseOpts = buildParseOptions(options);

    // Parse the header
    const tu = clang.clang_parseTranslationUnit(
      index,
      headerPath,
      args,
      args.length,
      null,
      0,
      parseOpts
    );

    if (!tu || !tu._ptr) {
      throw new Error(`Failed to parse header: ${headerPath}`);
    }

    try {
      // Initialize AST structure
      const ast: HeaderAST = {
        header: basename(headerPath),
        interfaces: [],
        protocols: [],
        enums: [],
        structs: [],
        typedefs: [],
        functions: [],
        classes: [],
        // Convenience aliases
        objc_interfaces: [],
        objc_protocols: [],
        objc_categories: [],
      };

      // Get root cursor
      const cursor = clang.clang_getTranslationUnitCursor(tu);

      // Recursively visit nodes
      const visitNode = (
        node: any,
        parentInHeader: boolean = false,
      ) => {
        // For now, assume all nodes from the parsed file are in the target header
        // TODO: Properly check source location once output parameters are supported
        const inThisHeader = true;

        // Process if in target header or if parent was in header (for nested decls)
        if (inThisHeader || parentInHeader) {
          const kind = clang.clang_getCursorKind(node);

          // Process based on cursor kind
          switch (kind) {
            case clang.CXCursor_ObjCInterfaceDecl:
              ast.interfaces.push(collectInterface(node));
              break;

            case clang.CXCursor_ObjCProtocolDecl:
              ast.protocols.push(collectProtocol(node));
              break;

            case clang.CXCursor_ClassDecl:
              if (ast.classes) {
                ast.classes.push(collectInterface(node));
              }
              break;

            case clang.CXCursor_EnumDecl:
              // Parse both named and anonymous enums
              ast.enums.push(collectEnum(node));
              break;

            case clang.CXCursor_StructDecl:
              // Parse both named and anonymous structs
              ast.structs.push(collectStruct(node));
              break;

            case clang.CXCursor_TypedefDecl:
              ast.typedefs.push(collectTypedef(node));
              break;

            case clang.CXCursor_FunctionDecl:
              if (ast.functions) {
                ast.functions.push(collectFunction(node));
              }
              break;
          }
        }

        // Recurse into children
        clang.clang_visitChildren(node, (child: any, _parent: any) => {
          visitNode(child, inThisHeader);
          return clang.CXChildVisit_Continue;
        }, null);
      };

      // Start visiting from root
      visitNode(cursor);

      // Update convenience aliases
      ast.objc_interfaces = ast.interfaces;
      ast.objc_protocols = ast.protocols;

      return ast;
    } finally {
      clang.clang_disposeTranslationUnit(tu);
    }
  } finally {
    clang.clang_disposeIndex(index);
  }
}

/**
 * Parse a header file and return JSON string
 */
export function parseHeaderToJSON(
  headerPath: string,
  options: ParseOptions = {},
): string {
  const ast = parseHeader(headerPath, options);
  return JSON.stringify(ast, null, 2);
}

// Export utilities and types
export * from './types.ts';
export * from './utils.ts';