import type { HeaderAST, MethodParam } from '../h-parser/types.ts';
import type {
  IRArray,
  IRCString,
  IRDoc,
  IREnum,
  IREnumRef,
  IRFunction,
  IRFunctionPointer,
  IRModule,
  IRParameterType,
  IRPointer,
  IRPrimitive,
  IRPrimitiveName,
  IRStruct,
  IRStructField,
  IRStructRef,
  IRType,
  IRTypedef,
  IRTypedefRef,
  IRUnknown,
} from './schema.ts';
import { runPatternPasses } from './passes/patterns.ts';

function toDoc(s?: string): IRDoc | undefined {
  if (!s) return undefined;
  const trimmed = s.trim();
  return trimmed ? { summary: trimmed } : undefined;
}

function normalizeWhitespace(s: string): string {
  return s.replace(/\s+/g, ' ').trim();
}

function parsePrimitiveName(cType: string): IRPrimitiveName | null {
  const t = normalizeWhitespace(cType);
  const table: IRPrimitiveName[] = [
    'void',
    'bool',
    'char',
    'signed char',
    'unsigned char',
    'short',
    'unsigned short',
    'int',
    'unsigned int',
    'long',
    'unsigned long',
    'long long',
    'unsigned long long',
    'float',
    'double',
    'size_t',
    'ssize_t',
  ];
  for (const n of table) if (t === n) return n;
  return null;
}

function isConstQualified(cType: string): boolean {
  return /(^|\s)const(\s|$)/.test(cType);
}

function stripConst(cType: string): string {
  return normalizeWhitespace(cType.replace(/\bconst\b/g, ''));
}

function parseTypeFromSpelling(spelling: string): IRType {
  const raw = normalizeWhitespace(spelling);

  if (raw === 'char *' || raw === 'const char *') {
    const t: IRCString = { kind: 'cstring', cType: raw };
    return t;
  }

  if (raw.includes('[') && raw.includes(']')) {
    const base = raw.substring(0, raw.indexOf('[')).trim();
    const el = parseTypeFromSpelling(base);
    const t: IRArray = { kind: 'array', element: el, length: null, cType: raw };
    return t;
  }

  if (raw.includes('(*') && raw.includes(')')) {
    const t: IRFunctionPointer = { kind: 'function-pointer', returnType: { kind: 'unknown', cType: raw } as IRUnknown, params: [], cType: raw };
    return t;
  }

  if (raw.endsWith('*')) {
    const isConst = isConstQualified(raw);
    const base = stripConst(raw).replace(/\*+$/, '').trim();
    const element = parseTypeFromSpelling(base);
    const t: IRPointer = { kind: 'pointer', element, isConst, cType: raw };
    return t;
  }

  if (raw.startsWith('enum ')) {
    const name = raw.replace(/^enum\s+/, '').trim();
    const t: IREnumRef = { kind: 'enum-ref', name, cType: raw };
    return t;
  }

  if (raw.startsWith('struct ') || raw.startsWith('class ')) {
    const name = raw.replace(/^(struct|class)\s+/, '').trim();
    const t: IRStructRef = { kind: 'struct-ref', name, cType: raw };
    return t;
  }

  const prim = parsePrimitiveName(raw);
  if (prim) {
    const t: IRPrimitive = { kind: 'primitive', name: prim, cType: raw };
    return t;
  }

  if (/^[A-Za-z_][A-Za-z0-9_]*$/.test(raw)) {
    const t: IRTypedefRef = { kind: 'typedef-ref', name: raw, cType: raw };
    return t;
  }

  const t: IRUnknown = { kind: 'unknown', cType: raw };
  return t;
}

function toIRParam(p: MethodParam): IRParameterType {
  const cType = p.type?.spelling || 'void';
  const type = parseTypeFromSpelling(cType);
  let direction: IRParameterType['direction'] = 'in';
  if (type.kind === 'pointer') {
    const pt = type as IRPointer;
    if (pt.isConst) direction = 'in';
    else direction = 'inout';
    const n = (p.name || '').toLowerCase();
    if (n.startsWith('out') || n.includes('result') || n.includes('ret')) direction = 'out';
  }
  const nullability = p.type?.nullability as any;
  return { name: p.name || '', type, direction, nullability };
}

function toIRFunction(astFn: NonNullable<HeaderAST['functions']>[number]): IRFunction {
  const returnType = parseTypeFromSpelling(astFn.return?.spelling || 'void');
  const params = astFn.params.map(toIRParam);
  const documentation = toDoc(astFn.documentation || undefined);
  const fn: IRFunction = { name: astFn.name, returnType, params };
  if (documentation) fn.documentation = documentation;
  return fn;
}

function toIRStruct(astStruct: HeaderAST['structs'][number]): IRStruct {
  const fields: IRStructField[] = astStruct.fields.map((f) => ({
    name: f.name,
    type: parseTypeFromSpelling(f.type),
  }));
  const s: IRStruct = { name: astStruct.name, fields };
  const doc = toDoc(astStruct.documentation);
  if (doc) s.documentation = doc;
  return s;
}

function toIREnum(astEnum: HeaderAST['enums'][number]): IREnum {
  const e: IREnum = {
    name: astEnum.name,
    constants: astEnum.constants.map((c) => {
      const ec: any = { name: c.name, value: c.value };
      const d = toDoc(c.documentation);
      if (d) ec.documentation = d;
      return ec;
    }),
  } as IREnum;
  const d = toDoc(astEnum.documentation);
  if (d) e.documentation = d;
  return e;
}

function toIRTypedef(astTypedef: HeaderAST['typedefs'][number]): IRTypedef {
  const t: IRTypedef = { name: astTypedef.name, targetCType: astTypedef.underlying } as IRTypedef;
  const d = toDoc(astTypedef.documentation);
  if (d) (t as any).documentation = d;
  return t;
}

export interface IRBuildOptions {
  moduleName?: string;
}

export function buildIR(ast: HeaderAST, options: IRBuildOptions = {}): IRModule {
  const mod: IRModule = {
    name: options.moduleName || ast.header || 'module',
    functions: (ast.functions || []).map(toIRFunction),
    structs: ast.structs.map(toIRStruct),
    enums: ast.enums.map(toIREnum),
    typedefs: ast.typedefs.map(toIRTypedef),
    classes: (ast.classes || []).map((c) => {
      const cls: any = { name: c.name, methods: [] };
      const d = toDoc(c.documentation);
      if (d) cls.documentation = d;
      return cls;
    }),
    metadata: {},
  };

  runPatternPasses(mod);
  return mod;
}
