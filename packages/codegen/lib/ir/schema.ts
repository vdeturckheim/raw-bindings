export type IRNullability = 'nonnull' | 'nullable' | 'unspecified' | 'nullable_result' | undefined;

export interface IRDoc {
  summary?: string;
}

export type IRPrimitiveName =
  | 'void'
  | 'bool'
  | 'char'
  | 'signed char'
  | 'unsigned char'
  | 'short'
  | 'unsigned short'
  | 'int'
  | 'unsigned int'
  | 'long'
  | 'unsigned long'
  | 'long long'
  | 'unsigned long long'
  | 'float'
  | 'double'
  | 'size_t'
  | 'ssize_t';

export type IRType =
  | IRPrimitive
  | IRPointer
  | IRArray
  | IRFunctionPointer
  | IRStructRef
  | IREnumRef
  | IRTypedefRef
  | IROpaqueRef
  | IRCString
  | IRUnknown;

export interface IRPrimitive {
  kind: 'primitive';
  name: IRPrimitiveName;
  cType: string;
}

export interface IRPointer {
  kind: 'pointer';
  element: IRType;
  isConst?: boolean;
  cType: string;
}

export interface IRArray {
  kind: 'array';
  element: IRType;
  length?: number | null;
  cType: string;
}

export interface IRFunctionPointer {
  kind: 'function-pointer';
  returnType: IRType;
  params: IRParameterType[];
  cType: string;
}

export interface IRCString {
  kind: 'cstring';
  cType: string;
}

export interface IRStructRef {
  kind: 'struct-ref';
  name: string;
  cType: string;
}

export interface IREnumRef {
  kind: 'enum-ref';
  name: string;
  cType: string;
}

export interface IRTypedefRef {
  kind: 'typedef-ref';
  name: string;
  cType: string;
}

export interface IROpaqueRef {
  kind: 'opaque-ref';
  name: string;
  cType: string;
}

export interface IRUnknown {
  kind: 'unknown';
  cType: string;
}

export interface IRParameterType {
  name: string;
  type: IRType;
  direction: 'in' | 'out' | 'inout';
  optional?: boolean;
  nullability?: IRNullability;
}

export interface IRFunction {
  name: string;
  returnType: IRType;
  params: IRParameterType[];
  documentation?: IRDoc;
  patterns?: IRPattern[];
  semantics?: {
    errorConvention?: 'none' | 'errno' | 'neg-is-error' | 'nonzero-is-error' | 'null-is-error';
    returnsOwned?: boolean;
    takesOwnershipOf?: number[];
    freedBy?: string | null;
  };
}

export interface IRStructField {
  name: string;
  type: IRType;
  documentation?: IRDoc;
}

export interface IRStruct {
  name: string;
  fields: IRStructField[];
  documentation?: IRDoc;
}

export interface IREnumConst {
  name: string;
  value: number | string | null;
  documentation?: IRDoc;
}

export interface IREnum {
  name: string;
  constants: IREnumConst[];
  documentation?: IRDoc;
}

export interface IRTypedef {
  name: string;
  targetCType: string;
  documentation?: IRDoc;
}

export interface IRClass {
  name: string;
  methods: IRFunction[];
  documentation?: IRDoc;
}

export interface IRModule {
  name: string;
  functions: IRFunction[];
  structs: IRStruct[];
  enums: IREnum[];
  typedefs: IRTypedef[];
  classes: IRClass[];
  metadata?: Record<string, unknown>;
}

export type IRPattern =
  | { kind: 'create-function'; resourceType: string }
  | { kind: 'destroy-function'; resourceType: string; paramIndex: number }
  | { kind: 'getter'; field?: string }
  | { kind: 'setter'; field?: string }
  | { kind: 'buffer-with-length'; bufferParam: number; lengthParam: number; mode: 'in' | 'out' | 'inout' }
  | { kind: 'string-with-length'; stringParam: number; lengthParam: number }
  | { kind: 'out-parameter'; paramIndex: number }
  | { kind: 'callback-parameter'; paramIndex: number }
  | { kind: 'userdata-parameter'; paramIndex: number }
  | { kind: 'error-code-return' }
  | { kind: 'group-handle'; resourceType: string }
  | { kind: 'resource-type'; name: string };

