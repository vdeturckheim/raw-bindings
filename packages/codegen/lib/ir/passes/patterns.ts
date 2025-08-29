import type { IRFunction, IRModule, IRPointer, IRPrimitive, IRType } from '../schema.ts';

function isPrimitiveIntLike(t: IRType): boolean {
  if (t.kind !== 'primitive') return false;
  const n = (t as IRPrimitive).name;
  return (
    n === 'char' ||
    n === 'signed char' ||
    n === 'unsigned char' ||
    n === 'short' ||
    n === 'unsigned short' ||
    n === 'int' ||
    n === 'unsigned int' ||
    n === 'long' ||
    n === 'unsigned long' ||
    n === 'long long' ||
    n === 'unsigned long long' ||
    n === 'size_t' ||
    n === 'ssize_t'
  );
}

function isVoidPointer(t: IRType): boolean {
  return t.kind === 'pointer' && (t as IRPointer).element.kind === 'primitive' && (t as IRPointer).element.cType === 'void';
}

export function detectCreateDestroyPairs(mod: IRModule): void {
  const resourceCreators: Array<{ fn: IRFunction; resource: string }> = [];
  for (const fn of mod.functions) {
    const name = fn.name.toLowerCase();
    const rt = fn.returnType;
    const returnsPointer = rt.kind === 'pointer' || rt.kind === 'struct-ref' || rt.kind === 'opaque-ref' || rt.kind === 'typedef-ref';
    if (returnsPointer && (name.includes('create') || name.includes('new') || name.includes('alloc') || name.startsWith('init'))) {
      const resourceType = rt.cType.replace(/\s*\*+\s*$/, '').trim();
      resourceCreators.push({ fn, resource: resourceType });
      fn.patterns ||= [];
      fn.patterns.push({ kind: 'create-function', resourceType: resourceType });
      fn.semantics ||= {};
      fn.semantics.returnsOwned = true;
    }
  }

  for (const fn of mod.functions) {
    const name = fn.name.toLowerCase();
    if (!(name.includes('destroy') || name.includes('free') || name.includes('release') || name.includes('dispose'))) continue;
    fn.patterns ||= [];
    for (let i = 0; i < fn.params.length; i++) {
      const p = fn.params[i]!;
      if (p.type.kind === 'pointer') {
        const pointee = (p.type as IRPointer).element;
        const resourceType = pointee.cType;
        fn.patterns.push({ kind: 'destroy-function', resourceType, paramIndex: i });
        fn.semantics ||= {};
        fn.semantics.takesOwnershipOf ||= [];
        fn.semantics.takesOwnershipOf.push(i);
      }
    }
  }

  for (const { fn, resource } of resourceCreators) {
    for (const d of mod.functions) {
      if (!d.patterns) continue;
      if (d.patterns.some((p: any) => p.kind === 'destroy-function' && p.resourceType === resource)) {
        fn.semantics ||= {};
        fn.semantics.freedBy = d.name;
      }
    }
  }
}

export function detectOutParams(mod: IRModule): void {
  for (const fn of mod.functions) {
    fn.patterns ||= [];
    for (let i = 0; i < fn.params.length; i++) {
      const p = fn.params[i]!;
      if (p.type.kind === 'pointer' && !(p.type as IRPointer).isConst) {
        const n = p.name.toLowerCase();
        const isLikelyOut = n.startsWith('out') || n.includes('result') || n.includes('ret') || i === fn.params.length - 1;
        if (isLikelyOut) fn.patterns.push({ kind: 'out-parameter', paramIndex: i });
      }
    }
  }
}

export function detectBufferWithLength(mod: IRModule): void {
  for (const fn of mod.functions) {
    fn.patterns ||= [];
    for (let i = 0; i < fn.params.length; i++) {
      const p = fn.params[i]!;
      if (p.type.kind !== 'pointer') continue;
      const elem = (p.type as IRPointer).element;
      const pointerToPrimitive = elem.kind === 'primitive' || isVoidPointer(p.type);
      if (!pointerToPrimitive) continue;
      for (let j = 0; j < fn.params.length; j++) {
        if (j === i) continue;
        const q = fn.params[j]!;
        if (!isPrimitiveIntLike(q.type)) continue;
        const qn = q.name.toLowerCase();
        if (qn.includes('len') || qn.includes('length') || qn.includes('size') || qn.includes('count')) {
          const mode: 'in' | 'out' | 'inout' = (p.direction === 'out' ? 'out' : p.direction === 'inout' ? 'inout' : 'in');
          fn.patterns.push({ kind: 'buffer-with-length', bufferParam: i, lengthParam: j, mode });
          break;
        }
      }
    }
  }
}

export function detectStringWithLength(mod: IRModule): void {
  for (const fn of mod.functions) {
    fn.patterns ||= [];
    for (let i = 0; i < fn.params.length; i++) {
      const p = fn.params[i]!;
      const isCString = p.type.kind === 'cstring' || (p.type.kind === 'pointer' && (p.type as IRPointer).element.kind === 'primitive' && (p.type as IRPointer).element.cType === 'char');
      if (!isCString) continue;
      for (let j = 0; j < fn.params.length; j++) {
        if (j === i) continue;
        const q = fn.params[j]!;
        if (!isPrimitiveIntLike(q.type)) continue;
        const qn = q.name.toLowerCase();
        if (qn.includes('len') || qn.includes('length') || qn.includes('size')) {
          fn.patterns.push({ kind: 'string-with-length', stringParam: i, lengthParam: j });
          break;
        }
      }
    }
  }
}

export function detectCallbacks(mod: IRModule): void {
  for (const fn of mod.functions) {
    fn.patterns ||= [];
    for (let i = 0; i < fn.params.length; i++) {
      const p = fn.params[i]!;
      if (p.type.kind === 'function-pointer') {
        fn.patterns.push({ kind: 'callback-parameter', paramIndex: i });
      }
      if (p.name.toLowerCase().includes('userdata') || p.name.toLowerCase().includes('context')) {
        fn.patterns.push({ kind: 'userdata-parameter', paramIndex: i });
      }
    }
  }
}

export function detectErrorConventions(mod: IRModule): void {
  for (const fn of mod.functions) {
    const rt = fn.returnType;
    if (rt.kind === 'primitive') {
      const n = (rt as IRPrimitive).name;
      if (n === 'int' || n === 'long' || n === 'ssize_t') {
        const name = fn.name.toLowerCase();
        const hasOut = (fn.patterns || []).some((p: any) => p.kind === 'out-parameter');
        fn.semantics ||= {};
        if (name.includes('error') || name.includes('status') || hasOut) {
          fn.semantics.errorConvention = 'nonzero-is-error';
        } else {
          fn.semantics.errorConvention = 'neg-is-error';
        }
      }
    }
    if (rt.kind === 'pointer') {
      fn.semantics ||= {};
      fn.semantics.errorConvention ||= 'null-is-error';
    }
  }
}

export function runPatternPasses(mod: IRModule): void {
  detectCreateDestroyPairs(mod);
  detectOutParams(mod);
  detectBufferWithLength(mod);
  detectStringWithLength(mod);
  detectCallbacks(mod);
  detectErrorConventions(mod);
}
