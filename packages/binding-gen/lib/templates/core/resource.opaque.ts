import type { Template } from '../../templates/schema.ts';

// Opaque resource mapped to a N-API addon (create/destroy wrappers) + TS class using [Symbol.dispose].
const tpl: Template = {
  id: 'core.resource.opaque',
  kind: 'resource',
  description: 'Opaque C handle mapped to a JS class with explicit lifetime (create/destroy).',
  cxx: {
    headers: [
      '#include <napi.h>',
      '#include <{{headerInclude}}>'
    ],
    helpers: [
      `static Napi::Object wrapPointer(Napi::Env env, void* ptr, const std::string& typeName) {\n  Napi::Object obj = Napi::Object::New(env);\n  obj.Set("_ptr", Napi::External<void>::New(env, ptr));\n  obj.Set("_type", Napi::String::New(env, typeName));\n  return obj;\n}\n`,
      `static void* unwrapPointer(Napi::Object obj) {\n  if (!obj.Has("_ptr")) return nullptr;\n  return obj.Get("_ptr").As<Napi::External<void>>().Data();\n}\n`
    ],
    declarations: [],
    definitions: [
      `static Napi::Value {{createFn}}_wrapper(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  auto handle = {{createCall}};
  return wrapPointer(env, handle, "{{handleTypedef}}");
}
`,
      `static Napi::Value {{destroyFn}}_wrapper(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsObject()) { Napi::TypeError::New(env, "Expected handle").ThrowAsJavaScriptException(); return env.Undefined(); }
  auto handle = ({{handleCxxType}})unwrapPointer(info[0].As<Napi::Object>());
  {{destroyFn}}(handle);
  return env.Undefined();
}
`
    ],
    moduleInit: [
      `exports.Set("{{createFn}}", Napi::Function::New(env, {{createFn}}_wrapper));`,
      `exports.Set("{{destroyFn}}", Napi::Function::New(env, {{destroyFn}}_wrapper));`
    ]
  },
  ts: {
    parts: [
      `export class {{className}} {
  #ptr: any;
  #disposed = false;

  static create(...args: any[]): {{className}} {
    const handle = addon.{{createFn}}(...args);
    return new {{className}}(handle);
  }

  private constructor(handle: any) { this.#ptr = handle; }
  get disposed() { return this.#disposed; }
  #ensure() { if (this.#disposed) throw new Error('{{className}} disposed'); }

  [kGetPtr](): any { this.#ensure(); return this.#ptr; }

  [Symbol.dispose](): void {
    if (!this.#disposed) {
      try { addon.{{destroyFn}}(this.#ptr); }
      finally { this.#disposed = true; this.#ptr = null; }
    }
  }

  /* @gen:methods */
}
`
    ]
  }
};

export default tpl;
