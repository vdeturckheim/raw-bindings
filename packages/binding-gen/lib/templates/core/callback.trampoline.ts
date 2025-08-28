import type { Template } from '../../templates/schema.ts';

// Minimal callback trampoline using ThreadSafeFunction.
// Exposes a function that accepts a JS callback and a number, and calls it once from native.
const tpl: Template = {
  id: 'core.callback.trampoline',
  kind: 'callback',
  description: 'ThreadSafeFunction trampoline: call a JS callback once with a number.',
  cxx: {
    headers: [
      '#include <napi.h>',
      '#include <thread>'
    ],
    helpers: [],
    declarations: [],
    definitions: [
      `static Napi::Value {{fn}}(const Napi::CallbackInfo& info) {\n  Napi::Env env = info.Env();\n  if (info.Length() < 2 || !info[0].IsFunction() || !info[1].IsNumber()) {\n    Napi::TypeError::New(env, \"Expected (Function, number)\").ThrowAsJavaScriptException();\n    return env.Undefined();\n  }\n  Napi::Function cb = info[0].As<Napi::Function>();\n  double v = info[1].As<Napi::Number>().DoubleValue();\n\n  auto tsfn = Napi::ThreadSafeFunction::New(env, cb, \"cb\", 0, 1);\n  std::thread t([tsfn, v]() mutable {\n    auto status = tsfn.BlockingCall(new double(v), [](Napi::Env env, Napi::Function jsCallback, double* value) {\n      jsCallback.Call({ Napi::Number::New(env, *value) });\n      delete value;\n    });\n    (void)status;\n    tsfn.Release();\n  });\n  t.join();\n  return env.Undefined();\n}\n`
    ],
    moduleInit: [
      `exports.Set(\"{{fn}}\", Napi::Function::New(env, {{fn}}));`
    ]
  },
  ts: {
    parts: [
      `export function {{jsName}}(value: number, cb: (n: number) => void): void {\n  addon.{{fn}}(cb, value);\n}\n`
    ]
  }
};

export default tpl;
