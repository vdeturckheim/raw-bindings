import Handlebars from 'handlebars';

export type TemplateKind =
  | 'resource'
  | 'method'
  | 'view'
  | 'enum'
  | 'struct'
  | 'callback'
  | 'error'
  | 'ownership'
  | 'objc';

export interface Template {
  id: string;              // unique identifier
  kind: TemplateKind;      // category
  description?: string;    // human hint

  // Native side (C/C++ N-API). Always present, even if minimal.
  cxx: {
    // Headers to include (each string is a line). Emitter will unique+sort.
    headers: string[];     // e.g., ['#include <napi.h>', '#include <clang-c/Index.h>']
    // Helper functions (wrapPointer, unwrapPointer, trampolines, etc.)
    helpers: string[];     // free-form code fragments
    // Declarations (optional forward decls)
    declarations?: string[];
    // Definitions (wrapper funcs/classes)
    definitions: string[];
    // Module init export lines (e.g., exports.Set(...))
    moduleInit: string[];
  };

  // JS side (TypeScript). Always present, even if minimal.
  ts: {
    parts: string[];       // main emitted TS code fragments
  };
}

// Render templates using Handlebars
export function render(template: string, bindings: Record<string, unknown>): string {
  const compiled = Handlebars.compile(template, { noEscape: true });
  return compiled(bindings);
}

// Common TS import prologue shared by all templates
export const TS_IMPORTS = `import nodeGypBuild from 'node-gyp-build';
const addon = nodeGypBuild(import.meta.dirname) as any;`;
// Hidden symbol used by templates to access native handle from resource classes
export const kGetPtrExport = `export const kGetPtr: unique symbol = Symbol.for('binding-gen.getPtr') as any;`;
