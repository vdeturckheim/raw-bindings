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
import { buildCompilerArgs, buildParseOptions, cleanDocumentation, getTypeInfo } from './utils.ts';

// Use an untyped handle for the native API to avoid over-constrained typings
const C: any = clang as any;

// Helper to get string from CXString
function getStringFromCXString(cxString: any): string {
  if (!cxString || !cxString._ptr) return '';
  try {
    const str = C.clang_getCString(cxString);
    C.clang_disposeString(cxString);
    return str || '';
  } catch (e) {
    // Handle null string case
    return '';
  }
}

/**
 * Collect method information from a cursor
 */
function collectMethod(cursor: any): Method {
  const kind = C.clang_getCursorKind(cursor);
  const isClassMethod = kind === C.CXCursor_ObjCClassMethodDecl;

  const method: Method = {
    kind: isClassMethod ? 'class' : 'instance',
    selector: getStringFromCXString(C.clang_getCursorSpelling(cursor)),
    return: getTypeInfo(C.clang_getCursorResultType(cursor)),
    params: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    method.documentation = doc;
  }

  // Get method arguments
  const numArgs = C.clang_Cursor_getNumArguments(cursor);
  for (let i = 0; i < numArgs; i++) {
    const argCursor = C.clang_Cursor_getArgument(cursor, i);
    const param: MethodParam = {
      name: getStringFromCXString(C.clang_getCursorSpelling(argCursor)) || '',
      type: getTypeInfo(C.clang_getCursorType(argCursor)),
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
    name: getStringFromCXString(C.clang_getCursorSpelling(cursor)),
    type: getTypeInfo(C.clang_getCursorType(cursor)),
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
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
    name: getStringFromCXString(C.clang_getCursorSpelling(cursor)),
    methods: [],
    properties: [],
    protocols: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    iface.documentation = doc;
  }

  // Visit children to collect methods, properties, and protocol references
  C.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = C.clang_getCursorKind(child);

    if (
      kind === clang.CXCursor_ObjCInstanceMethodDecl ||
      kind === clang.CXCursor_ObjCClassMethodDecl ||
      kind === clang.CXCursor_CXXMethod
    ) {
      iface.methods.push(collectMethod(child));
    } else if (kind === clang.CXCursor_ObjCPropertyDecl) {
      iface.properties.push(collectProperty(child));
    } else if (kind === clang.CXCursor_ObjCProtocolRef) {
      iface.protocols.push(getStringFromCXString(C.clang_getCursorSpelling(child)));
    }

    return C.CXChildVisit_Continue;
  }, null);

  return iface;
}

/**
 * Collect protocol information
 */
function collectProtocol(cursor: any): Protocol {
  const protocol: Protocol = {
    name: getStringFromCXString(C.clang_getCursorSpelling(cursor)),
    methods: [],
    properties: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    protocol.documentation = doc;
  }

  // Visit children to collect methods and properties
  C.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = C.clang_getCursorKind(child);

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

    return C.CXChildVisit_Continue;
  }, null);

  return protocol;
}

/**
 * Collect enum information
 */
function collectEnum(cursor: any): Enum {
  const enumDecl: Enum = {
    name: getStringFromCXString(C.clang_getCursorSpelling(cursor)) || '',
    constants: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    enumDecl.documentation = doc;
  }

  // Visit children to collect enum constants
  C.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = C.clang_getCursorKind(child);

    if (kind === clang.CXCursor_EnumConstantDecl) {
      const constant: EnumConstant = {
        name: getStringFromCXString(C.clang_getCursorSpelling(child)),
        value: null,
      };

      try {
        const val = C.clang_getEnumConstantDeclValue(child);
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
      const constDoc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(child)));
      if (constDoc) {
        constant.documentation = constDoc;
      }

      enumDecl.constants.push(constant);
    }

    return C.CXChildVisit_Continue;
  }, null);

  return enumDecl;
}

/**
 * Collect struct information
 */
function collectStruct(cursor: any): Struct {
  const struct: Struct = {
    name: getStringFromCXString(C.clang_getCursorSpelling(cursor)),
    fields: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    struct.documentation = doc;
  }

  // Visit children to collect fields
  C.clang_visitChildren(cursor, (child: any, _parent: any) => {
    const kind = C.clang_getCursorKind(child);

    if (kind === clang.CXCursor_FieldDecl) {
      const field: StructField = {
        name: getStringFromCXString(C.clang_getCursorSpelling(child)),
        type: getStringFromCXString(C.clang_getTypeSpelling(C.clang_getCursorType(child))),
      };
      struct.fields.push(field);
    }

    return C.CXChildVisit_Continue;
  }, null);

  return struct;
}

/**
 * Collect typedef information
 */
function collectTypedef(cursor: any): Typedef {
  const typedef: Typedef = {
    name: getStringFromCXString(C.clang_getCursorSpelling(cursor)),
    underlying: getStringFromCXString(C.clang_getTypeSpelling(C.clang_getTypedefDeclUnderlyingType(cursor))),
    spelling: getStringFromCXString(C.clang_getTypeSpelling(C.clang_getCursorType(cursor))),
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
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
    name: getStringFromCXString(C.clang_getCursorSpelling(cursor)),
    return: getTypeInfo(C.clang_getCursorResultType(cursor)),
    params: [],
  };

  // Add documentation if available
  const doc = cleanDocumentation(getStringFromCXString(C.clang_Cursor_getRawCommentText(cursor)));
  if (doc) {
    func.documentation = doc;
  }

  // Get function arguments
  const numArgs = C.clang_Cursor_getNumArguments(cursor);
  for (let i = 0; i < numArgs; i++) {
    const argCursor = C.clang_Cursor_getArgument(cursor, i);
    const param: MethodParam = {
      name: getStringFromCXString(C.clang_getCursorSpelling(argCursor)) || '',
      type: getTypeInfo(C.clang_getCursorType(argCursor)),
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
  const index = C.clang_createIndex(
    options.excludeDeclarationsFromPCH ? 1 : 0,
    options.displayDiagnostics ? 1 : 0
  );

  try {
    // Build compiler arguments
    const args = buildCompilerArgs(options);

    // Build parse options
    const parseOpts = buildParseOptions(options);

    // Parse the header
    const tu = C.clang_parseTranslationUnit(
      index,
      headerPath,
      args,
      args.length,
      null,
      0,
      parseOpts
    );

    if (!tu || !(tu as any)._ptr) {
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
      const cursor = C.clang_getTranslationUnitCursor(tu);

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
          const kind = C.clang_getCursorKind(node);

          // Process based on cursor kind
          switch (kind) {
            case C.CXCursor_ObjCInterfaceDecl:
              ast.interfaces.push(collectInterface(node));
              break;

            case C.CXCursor_ObjCProtocolDecl:
              ast.protocols.push(collectProtocol(node));
              break;

            case C.CXCursor_ClassDecl:
              if (ast.classes) {
                ast.classes.push(collectInterface(node));
              }
              break;

            case C.CXCursor_EnumDecl:
              // Parse both named and anonymous enums
              ast.enums.push(collectEnum(node));
              break;

            case C.CXCursor_StructDecl:
              // Parse both named and anonymous structs
              ast.structs.push(collectStruct(node));
              break;

            case C.CXCursor_TypedefDecl:
              ast.typedefs.push(collectTypedef(node));
              break;

            case C.CXCursor_FunctionDecl:
              if (ast.functions) {
                ast.functions.push(collectFunction(node));
              }
              break;
          }
        }

        // Recurse into children
        C.clang_visitChildren(node, (child: any, _parent: any) => {
          visitNode(child, inThisHeader);
          return C.CXChildVisit_Continue;
        }, null);
      };

      // Start visiting from root
      visitNode(cursor);

      // Update convenience aliases
      ast.objc_interfaces = ast.interfaces;
      ast.objc_protocols = ast.protocols;

      return ast;
    } finally {
      C.clang_disposeTranslationUnit(tu);
    }
  } finally {
    C.clang_disposeIndex(index);
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
