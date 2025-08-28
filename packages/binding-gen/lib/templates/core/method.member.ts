import type { Template } from '../../templates/schema.ts';

// Instance method for an opaque resource. Assumes unwrapPointer helper is available (deduped).
// Returns a number result for now (common for many C APIs). Extend as needed.
const tpl: Template = {
  id: 'core.method.member',
  kind: 'method',
  description: 'Bind a C function as an instance method on an opaque class (number return).',
  cxx: {
    headers: [
      '#include <napi.h>'
    ],
    helpers: [
      `static void* unwrapPointer(Napi::Object obj) {\n  if (!obj.Has("_ptr")) return nullptr;\n  return obj.Get("_ptr").As<Napi::External<void>>().Data();\n}\n`
    ],
    declarations: [],
    definitions: [
      `static Napi::Value {{methodFn}}_wrapper(const Napi::CallbackInfo& info) {\n  Napi::Env env = info.Env();\n  if (info.Length() < 1 || !info[0].IsObject()) { Napi::TypeError::New(env, "Expected handle").ThrowAsJavaScriptException(); return env.Undefined(); }\n  auto handle = ({{handleCxxType}})unwrapPointer(info[0].As<Napi::Object>());\n  auto result = {{methodCall}};\n  return Napi::Number::New(env, (double)result);\n}\n`
    ],
    moduleInit: [
      `exports.Set("{{methodFn}}", Napi::Function::New(env, {{methodFn}}_wrapper));`
    ]
  },
  ts: {
    parts: [
      `/* @gen:method {{className}} */
{{methodName}}(...args: any[]): any {
  return addon.{{methodFn}}(this[kGetPtr](), ...args);
}
`
    ]
  }
};

export default tpl;
