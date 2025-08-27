// AST structure types matching the Python script output

export interface TypeInfo {
  spelling: string;
  nullability?: 'nonnull' | 'nullable' | 'unspecified' | 'nullable_result';
}

export interface MethodParam {
  name: string;
  type: TypeInfo;
}

export interface Method {
  kind: 'class' | 'instance';
  selector: string;
  return: TypeInfo;
  params: MethodParam[];
  documentation?: string;
}

export interface Property {
  name: string;
  type: TypeInfo;
  documentation?: string;
}

export interface Interface {
  name: string;
  methods: Method[];
  properties: Property[];
  protocols: string[];
  documentation?: string;
}

export interface Protocol {
  name: string;
  documentation?: string;
  methods?: Method[];
  properties?: Property[];
}

export interface EnumConstant {
  name: string;
  value: number | null;
  documentation?: string;
}

export interface Enum {
  name: string;
  constants: EnumConstant[];
  documentation?: string;
}

export interface StructField {
  name: string;
  type: string;
}

export interface Struct {
  name: string;
  fields: StructField[];
  documentation?: string;
}

export interface Typedef {
  name: string;
  underlying: string;
  documentation?: string;
}

export interface Function {
  name: string;
  return: TypeInfo;
  params: MethodParam[];
  documentation?: string;
}

export interface HeaderAST {
  header: string;
  interfaces: Interface[];
  protocols: Protocol[];
  enums: Enum[];
  structs: Struct[];
  typedefs: Typedef[];
  functions?: Function[];
  classes?: Interface[]; // For C++ classes
  // Convenience aliases for better API
  objc_interfaces: Interface[];
  objc_protocols: Protocol[];
  objc_categories: any[];
}

export interface ParseOptions {
  // Compiler arguments
  language?: 'c' | 'c++' | 'objective-c' | 'objective-c++';
  systemRoot?: string;
  includePaths?: string[];
  additionalIncludePaths?: string[];  // Alias for includePaths for convenience
  frameworkPaths?: string[];
  frameworks?: string[];
  defines?: string[];
  additionalArgs?: string[];
  
  // Parse options
  includeDocumentation?: boolean;
  detailedProcessing?: boolean;
}