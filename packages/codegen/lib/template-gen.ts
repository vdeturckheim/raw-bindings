import { join, dirname } from 'node:path';
import { fileURLToPath } from 'node:url';
import { HbsEngine } from './templates/handlebars-engine.ts';
import type { IRModule, IRStruct, IRType, IRFunction, IRPointer } from './ir/schema.ts';
import { TypeMapper } from '../type-mapper.ts';

const __dirname = dirname(fileURLToPath(import.meta.url));

function tsFromIR(type: any, structNames?: Set<string>): string {
  // Accept IRType or string cType
  if (typeof type === 'string') return TypeMapper.getTsType(type);
  const t = type as IRType;
  switch (t.kind) {
    case 'cstring':
      return 'string';
    case 'primitive':
      return TypeMapper.getTsType(t.cType);
    case 'enum-ref':
      return 'number';
    case 'struct-ref':
      return t.name;
    case 'typedef-ref':
      if (structNames && structNames.has(t.name)) return t.name;
      return TypeMapper.getTsType(t.name);
    case 'pointer': {
      const el = t.element as any;
      if (el && el.kind === 'primitive' && el.cType === 'void') return 'unknown';
      if (el && el.kind === 'struct-ref') return `${el.name} | AddonHandle | null`;
      return 'unknown';
    }
    case 'array': {
      const el = t.element as any;
      if (el && el.kind === 'primitive' && el.cType === 'char') return 'string';
      const elTs = tsFromIR(el, structNames);
      return `${elTs}[]`;
    }
    default:
      return 'unknown';
  }
}

export function renderTsIndexFromIR(ir: IRModule): string {
  const baseDir = join(__dirname, 'templates', 'default', 'ts');
  const engine = new HbsEngine({ baseDir });

  // Helpers
  const structNamesSet = new Set((ir.structs || []).map(s => s.name));
  engine.registerHelper('tsType', (irType: any) => tsFromIR(irType, structNames));
  engine.registerHelper('sanitize', (name: any) => TypeMapper.sanitizeIdentifier(String(name)));

  // Partials
  engine.registerPartialsFrom(baseDir);

  // Build view model: struct list with only well-named structs
  const byName = new Map<string, any>();
  for (const s of ir.structs || []) {
    if (!s.name || !s.fields || s.fields.length === 0) continue;
    if (s.name.includes('(unnamed at ') || s.name.includes('/') || s.name.includes('__')) continue;
    if (byName.has(s.name)) continue; // dedupe
    byName.set(s.name, {
      name: s.name,
      documentation: s.documentation,
      fields: s.fields.map(f => ({ name: f.name, type: f.type }))
    });
  }
  const structs = Array.from(byName.values()).sort((a, b) => a.name.localeCompare(b.name));

  // Build function model for TS
  const structNames = new Set((ir.structs || []).map(s => s.name));
  const functions = (ir.functions || []).map(fn => {
    const params = fn.params.map((p, i) => {
      const t = p.type as IRType;
      let isStruct = false;
      let structName: string | undefined;
      if (t.kind === 'struct-ref') { isStruct = true; structName = t.name; }
      if (t.kind === 'typedef-ref' && structNamesSet.has(t.name)) { isStruct = true; structName = t.name; }
      return { name: p.name || `arg${i}`, type: t, isStruct, structName };
    });
    const ret = fn.returnType as IRType;
    const returnsStruct = ret.kind === 'struct-ref' || (ret.kind === 'typedef-ref' && structNamesSet.has((ret as any).name));
    const returnStructName = returnsStruct ? (ret.kind === 'struct-ref' ? ret.name : (ret as any).name) : undefined;
    return { name: fn.name, params, returnType: ret, returnsStruct, returnStructName };
  });

  const model = { structs, functions };

  const tplPath = join(baseDir, 'index.hbs');
  return engine.render(tplPath, model);
}

function cTypeFromIR(t: IRType): string {
  switch (t.kind) {
    case 'primitive': return t.cType;
    case 'cstring': return 'const char *';
    case 'enum-ref': return `int`;
    case 'struct-ref': return t.cType || `struct ${t.name}`;
    case 'typedef-ref': return t.name;
    case 'pointer': return `${cTypeFromIR(t.element)} *`;
    case 'array': return `${cTypeFromIR(t.element)}[]`;
    default: return 'void *';
  }
}

function buildStructContext(s: IRStruct) {
  const cType = s.name; // assume typedef or bare name usable
  const fields = s.fields.map(f => {
    const type = f.type as IRType;
    // Set lines to assign and get
    let fieldSetLine = '';
    if (type.kind === 'cstring' || (type.kind === 'array' && (type.element as any)?.cType === 'char')) {
      fieldSetLine = `std::string tmp = obj.Get("${f.name}").As<Napi::String>().Utf8Value(); std::strncpy(ptr->${f.name}, tmp.c_str(), sizeof(ptr->${f.name}) - 1); ptr->${f.name}[sizeof(ptr->${f.name})-1] = '\\0';`;
    } else if (type.kind === 'primitive') {
      if (type.cType.includes('double') || type.cType.includes('float')) {
        fieldSetLine = `ptr->${f.name} = obj.Get("${f.name}").As<Napi::Number>().DoubleValue();`;
      } else {
        fieldSetLine = `ptr->${f.name} = obj.Get("${f.name}").As<Napi::Number>().Int32Value();`;
      }
    } else if (type.kind === 'struct-ref') {
      fieldSetLine = `auto fobj = obj.Get("${f.name}").As<Napi::Object>(); auto fptr = static_cast<${type.cType || type.name}*>(unwrapPointer(fobj)); if (fptr) ptr->${f.name} = *fptr;`;
    } else {
      fieldSetLine = `/* unsupported field type */`;
    }

    let fieldGetLine = '';
    if (type.kind === 'cstring') {
      fieldGetLine = `Napi::String::New(env, ptr->${f.name} ? ptr->${f.name} : "")`;
    } else if (type.kind === 'array' && (type.element as any)?.cType === 'char') {
      fieldGetLine = `Napi::String::New(env, ptr->${f.name})`;
    } else if (type.kind === 'primitive') {
      if (type.cType.includes('double') || type.cType.includes('float')) fieldGetLine = `Napi::Number::New(env, ptr->${f.name})`; else fieldGetLine = `Napi::Number::New(env, ptr->${f.name})`;
    } else if (type.kind === 'struct-ref') {
      fieldGetLine = `wrapOwnedPointer(env, new ${type.cType || type.name}(ptr->${f.name}), "${type.name}")`;
    } else {
      fieldGetLine = `env.Undefined()`;
    }

    return { name: f.name, fieldSetLine, fieldGetLine };
  });
  return { name: s.name, cType, fields };
}

function buildFunctionContext(fn: IRFunction, structNames: Set<string>) {
  // Only support primitives and struct-by-value parameters for now
  const paramLines: string[] = [];
  const callArgs: string[] = [];
  fn.params.forEach((p, i) => {
    const t = p.type as IRType;
    const pname = p.name || `arg${i}`;
    if (t.kind === 'primitive') {
      if (t.cType.includes('double') || t.cType.includes('float')) paramLines.push(`double ${pname} = info[${i}].As<Napi::Number>().DoubleValue();`);
      else paramLines.push(`int ${pname} = info[${i}].As<Napi::Number>().Int32Value();`);
      callArgs.push(pname);
    } else if (t.kind === 'struct-ref' || (t.kind === 'typedef-ref' && structNames.has((t as any).name))) {
      const ct = (t as any).cType || (t as any).name;
      paramLines.push(`${ct} ${pname} = *static_cast<${ct}*>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
      callArgs.push(pname);
    } else if (t.kind === 'enum-ref') {
      // Enums as numbers
      paramLines.push(`int ${pname}_num = info[${i}].As<Napi::Number>().Int32Value();`);
      const ct = cTypeFromIR(t);
      callArgs.push(`static_cast<${ct}>(${pname}_num)`);
    } else if (t.kind === 'typedef-ref') {
      // Assume numeric-like typedef (e.g., double, int)
      const ct = cTypeFromIR(t);
      paramLines.push(`double ${pname}_num = info[${i}].As<Napi::Number>().DoubleValue();`);
      callArgs.push(`static_cast<${ct}>(${pname}_num)`);
    } else {
      // unsupported param kind; use placeholder
      paramLines.push(`/* unsupported param ${pname} */`);
      callArgs.push(`/*unsupported*/0`);
    }
  });

  const ret = fn.returnType as IRType;
  let callLine = '';
  let returnLine = 'return env.Undefined();';
  const argList = callArgs.join(', ');
  if (ret.kind === 'primitive') {
    callLine = `auto result = ${fn.name}(${argList});`;
    returnLine = `return Napi::Number::New(env, result);`;
  } else if (ret.kind === 'struct-ref' || (ret.kind === 'typedef-ref' && structNames.has((ret as any).name))) {
    const ct = (ret as any).cType || (ret as any).name;
    callLine = `${ct} result = ${fn.name}(${argList});`;
    returnLine = `return wrapOwnedPointer(env, new ${ct}(result), "${(ret as any).name}");`;
  } else if (ret.kind === 'cstring') {
    callLine = `auto result = ${fn.name}(${argList});`;
    returnLine = `return result ? Napi::String::New(env, result) : env.Null();`;
  } else {
    callLine = `/* unsupported return */`;
    returnLine = `return env.Undefined();`;
  }

  return { name: fn.name, wrapperName: `${fn.name}_wrapper`, paramLines, callLine, returnLine };
}

export function renderCppBindingFromIR(ir: IRModule, includes: string[]): string {
  const baseDir = join(__dirname, 'templates', 'default', 'cpp');
  const engine = new HbsEngine({ baseDir });
  const structNames = new Set((ir.structs || []).map(s => s.name));
  const typedefMap = new Map<string, string>();
  for (const td of ir.typedefs || []) {
    typedefMap.set(td.name, td.targetCType);
  }
  const sByName = new Map<string, ReturnType<typeof buildStructContext>>();
  for (const s of ir.structs || []) {
    if (!s.name || !s.fields || s.fields.length === 0) continue;
    if (s.name.includes('(unnamed at ') || s.name.includes('/') || s.name.includes('__')) continue;
    if (sByName.has(s.name)) continue;
    sByName.set(s.name, buildStructContext(s));
  }
  const structs = Array.from(sByName.values());
  const functions = (ir.functions || [])
    .filter(f => f.params.every(p => {
      const t = p.type as IRType;
      if (t.kind === 'primitive' || t.kind === 'enum-ref' || t.kind === 'struct-ref') return true;
      if (t.kind === 'typedef-ref') {
        const name = (t as any).name as string;
        const underlying = typedefMap.get(name) || '';
        if (/^union\b/.test(underlying)) return false; // skip union by value
        if (underlying.includes('*')) return false;    // skip opaque pointers for now
        return true; // treat as numeric alias or struct alias by value
      }
      return false;
    }))
    .map(f => buildFunctionContext(f, structNames));
  const model = { includes, structs, functions };
  const tplPath = join(baseDir, 'binding.hbs');
  return engine.render(tplPath, model);
}

export function renderCMakeLists(opts: { packageName: string }): string {
  const baseDir = join(__dirname, 'templates', 'default', 'cmake');
  const engine = new HbsEngine({ baseDir });
  const tplPath = join(baseDir, 'CMakeLists.hbs');
  return engine.render(tplPath, { packageName: opts.packageName });
}

export function renderPackageJson(opts: { packageName: string; packageVersion: string; libraryName: string }): string {
  const baseDir = join(__dirname, 'templates', 'default', 'pkg');
  const engine = new HbsEngine({ baseDir });
  const tplPath = join(baseDir, 'package.json.hbs');
  return engine.render(tplPath, opts);
}
