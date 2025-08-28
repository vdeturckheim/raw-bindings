import type { Template } from '../../templates/schema.ts';

// Instance method that returns a string via a CXString-like API: result type with
// separate getCString and dispose functions.
const tpl: Template = {
  id: 'core.method.member.cxstring',
  kind: 'method',
  description: 'Bind a C function returning CXString-like type to a JS string method.',
  cxx: {
    headers: [
      '#include <napi.h>'
    ],
    helpers: [
      `static void* unwrapPointer(Napi::Object obj) {\n  if (!obj.Has(\"_ptr\")) return nullptr;\n  return obj.Get(\"_ptr\").As<Napi::External<void>>().Data();\n}\n`
    ],
    declarations: [],
    definitions: [
      `static Napi::Value {{methodFn}}_wrapper(const Napi::CallbackInfo& info) {\n  Napi::Env env = info.Env();\n  if (info.Length() < 1 || !info[0].IsObject()) { Napi::TypeError::New(env, \"Expected handle\").ThrowAsJavaScriptException(); return env.Undefined(); }\n  auto handle = ({{handleCxxType}})unwrapPointer(info[0].As<Napi::Object>());\n  auto result = {{methodCall}};\n  const char* cstr = {{getCString}}(result);\n  Napi::Value out = Napi::String::New(env, cstr ? cstr : \"\");\n  {{dispose}}(result);\n  return out;\n}\n`
    ],
    moduleInit: [
      `exports.Set(\"{{methodFn}}\", Napi::Function::New(env, {{methodFn}}_wrapper));`
    ]
  },
  ts: {
    parts: [
      `/* @gen:method {{className}} */
{{methodName}}(...args: any[]): string {
  return addon.{{methodFn}}(this[kGetPtr](), ...args);
}
`
    ]
  }
};

export default tpl;
