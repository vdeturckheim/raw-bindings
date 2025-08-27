import * as clang from 'node-clang';
import { basename } from 'node:path';
import {
  cleanDocumentation,
  getTypeInfo,
  getCursorKindName,
  isCursorFromFile,
  buildCompilerArgs,
  buildParseOptions
} from './utils.ts';
import type {
  HeaderAST,
  Interface,
  Protocol,
  Enum,
  EnumConstant,
  Struct,
  StructField,
  Typedef,
  Method,
  Property,
  Function,
  MethodParam,
  ParseOptions
} from './types.ts';

/**
 * Collect method information from a cursor
 */
function collectMethod(cursor: clang.Cursor): Method {
  const kind = clang.getCursorKind(cursor);
  const isClassMethod = kind === clang.CXCursorKind.ObjCClassMethodDecl;
  
  const method: Method = {
    kind: isClassMethod ? 'class' : 'instance',
    selector: clang.getCursorSpelling(cursor),
    return: getTypeInfo(clang.getCursorResultType(cursor)),
    params: []
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    method.documentation = doc;
  }

  // Get method arguments
  const numArgs = clang.getNumCursorArguments(cursor);
  for (let i = 0; i < numArgs; i++) {
    const argCursor = clang.getCursorArgument(cursor, i);
    const param: MethodParam = {
      name: clang.getCursorSpelling(argCursor) || '',
      type: getTypeInfo(clang.getCursorType(argCursor))
    };
    method.params.push(param);
  }

  return method;
}

/**
 * Collect property information from a cursor
 */
function collectProperty(cursor: clang.Cursor): Property {
  const prop: Property = {
    name: clang.getCursorSpelling(cursor),
    type: getTypeInfo(clang.getCursorType(cursor))
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    prop.documentation = doc;
  }

  return prop;
}

/**
 * Collect Objective-C interface or C++ class information
 */
function collectInterface(cursor: clang.Cursor): Interface {
  const iface: Interface = {
    name: clang.getCursorSpelling(cursor),
    methods: [],
    properties: [],
    protocols: []
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    iface.documentation = doc;
  }

  // Visit children to collect methods, properties, and protocol references
  clang.visitChildren(cursor, (child: clang.Cursor) => {
    const kind = clang.getCursorKind(child);
    
    if (kind === clang.CXCursorKind.ObjCInstanceMethodDecl ||
        kind === clang.CXCursorKind.ObjCClassMethodDecl ||
        kind === clang.CXCursorKind.CXXMethod) {
      iface.methods.push(collectMethod(child));
    } else if (kind === clang.CXCursorKind.ObjCPropertyDecl) {
      iface.properties.push(collectProperty(child));
    } else if (kind === clang.CXCursorKind.ObjCProtocolRef) {
      iface.protocols.push(clang.getCursorSpelling(child));
    }
    
    return clang.CXChildVisit.Continue;
  });

  return iface;
}

/**
 * Collect protocol information
 */
function collectProtocol(cursor: clang.Cursor): Protocol {
  const protocol: Protocol = {
    name: clang.getCursorSpelling(cursor),
    methods: [],
    properties: []
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    protocol.documentation = doc;
  }

  // Visit children to collect methods and properties
  clang.visitChildren(cursor, (child: clang.Cursor) => {
    const kind = clang.getCursorKind(child);
    
    if (kind === clang.CXCursorKind.ObjCInstanceMethodDecl ||
        kind === clang.CXCursorKind.ObjCClassMethodDecl) {
      if (!protocol.methods) protocol.methods = [];
      protocol.methods.push(collectMethod(child));
    } else if (kind === clang.CXCursorKind.ObjCPropertyDecl) {
      if (!protocol.properties) protocol.properties = [];
      protocol.properties.push(collectProperty(child));
    }
    
    return clang.CXChildVisit.Continue;
  });

  return protocol;
}

/**
 * Collect enum information
 */
function collectEnum(cursor: clang.Cursor): Enum {
  const enumDecl: Enum = {
    name: clang.getCursorSpelling(cursor) || '',
    constants: []
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    enumDecl.documentation = doc;
  }

  // Visit children to collect enum constants
  clang.visitChildren(cursor, (child: clang.Cursor) => {
    const kind = clang.getCursorKind(child);
    
    if (kind === clang.CXCursorKind.EnumConstantDecl) {
      const constant: EnumConstant = {
        name: clang.getCursorSpelling(child),
        value: null
      };
      
      try {
        constant.value = clang.getEnumConstantDeclValue(child);
      } catch {
        // Value might not be available
      }
      
      // Add documentation for constant if available
      const constDoc = cleanDocumentation(clang.getCursorRawCommentText(child));
      if (constDoc) {
        constant.documentation = constDoc;
      }
      
      enumDecl.constants.push(constant);
    }
    
    return clang.CXChildVisit.Continue;
  });

  return enumDecl;
}

/**
 * Collect struct information
 */
function collectStruct(cursor: clang.Cursor): Struct {
  const struct: Struct = {
    name: clang.getCursorSpelling(cursor),
    fields: []
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    struct.documentation = doc;
  }

  // Visit children to collect fields
  clang.visitChildren(cursor, (child: clang.Cursor) => {
    const kind = clang.getCursorKind(child);
    
    if (kind === clang.CXCursorKind.FieldDecl) {
      const field: StructField = {
        name: clang.getCursorSpelling(child),
        type: clang.getTypeSpelling(clang.getCursorType(child))
      };
      struct.fields.push(field);
    }
    
    return clang.CXChildVisit.Continue;
  });

  return struct;
}

/**
 * Collect typedef information
 */
function collectTypedef(cursor: clang.Cursor): Typedef {
  const typedef: Typedef = {
    name: clang.getCursorSpelling(cursor),
    underlying: clang.getTypeSpelling(clang.getTypedefDeclUnderlyingType(cursor))
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    typedef.documentation = doc;
  }

  return typedef;
}

/**
 * Collect function information
 */
function collectFunction(cursor: clang.Cursor): Function {
  const func: Function = {
    name: clang.getCursorSpelling(cursor),
    return: getTypeInfo(clang.getCursorResultType(cursor)),
    params: []
  };

  // Add documentation if available
  const doc = cleanDocumentation(clang.getCursorRawCommentText(cursor));
  if (doc) {
    func.documentation = doc;
  }

  // Get function arguments
  const numArgs = clang.getNumCursorArguments(cursor);
  for (let i = 0; i < numArgs; i++) {
    const argCursor = clang.getCursorArgument(cursor, i);
    const param: MethodParam = {
      name: clang.getCursorSpelling(argCursor) || '',
      type: getTypeInfo(clang.getCursorType(argCursor))
    };
    func.params.push(param);
  }

  return func;
}

/**
 * Parse a header file and return structured AST
 */
export function parseHeader(headerPath: string, options: ParseOptions = {}): HeaderAST {
  // Create index
  const index = clang.createIndex();
  
  try {
    // Build compiler arguments
    const args = buildCompilerArgs(options);
    
    // Build parse options
    const parseOpts = buildParseOptions(options);
    
    // Parse the header
    const tu = clang.parseTranslationUnit(index, headerPath, args, parseOpts);
    
    if (!tu) {
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
        objc_categories: []
      };
      
      // Get root cursor
      const cursor = tu.getCursor();
      
      // Recursively visit nodes
      const visitNode = (node: clang.Cursor, parentInHeader: boolean = false) => {
        // Check if this cursor is in our target header
        const location = clang.getCursorLocation(node);
        const inThisHeader = location.file === headerPath;
        
        // Process if in target header or if parent was in header (for nested decls)
        if (inThisHeader || parentInHeader) {
          const kind = clang.getCursorKind(node);
          const spelling = clang.getCursorSpelling(node);
          
          // Process based on cursor kind
          switch (kind) {
            case clang.CXCursorKind.ObjCInterfaceDecl:
              ast.interfaces.push(collectInterface(node));
              break;
            
            case clang.CXCursorKind.ObjCProtocolDecl:
              ast.protocols.push(collectProtocol(node));
              break;
            
            case clang.CXCursorKind.ClassDecl:
              if (ast.classes) {
                ast.classes.push(collectInterface(node));
              }
              break;
            
            case clang.CXCursorKind.EnumDecl:
              // Parse both named and anonymous enums
              ast.enums.push(collectEnum(node));
              break;
            
            case clang.CXCursorKind.StructDecl:
              // Parse both named and anonymous structs
              ast.structs.push(collectStruct(node));
              break;
            
            case clang.CXCursorKind.TypedefDecl:
              ast.typedefs.push(collectTypedef(node));
              break;
            
            case clang.CXCursorKind.FunctionDecl:
              if (ast.functions) {
                ast.functions.push(collectFunction(node));
              }
              break;
          }
        }
        
        // Recurse into children
        clang.visitChildren(node, (child: clang.Cursor) => {
          visitNode(child, inThisHeader);
          return clang.CXChildVisit.Continue;
        });
      };
      
      // Start visiting from root
      visitNode(cursor);
      
      // Update convenience aliases
      ast.objc_interfaces = ast.interfaces;
      ast.objc_protocols = ast.protocols;
      
      return ast;
      
    } finally {
      tu.dispose();
    }
  } finally {
    index.dispose();
  }
}

/**
 * Parse a header file and return JSON string
 */
export function parseHeaderToJSON(headerPath: string, options: ParseOptions = {}): string {
  const ast = parseHeader(headerPath, options);
  return JSON.stringify(ast, null, 2);
}

// Export utilities and types
export * from './types.ts';
export * from './utils.ts';