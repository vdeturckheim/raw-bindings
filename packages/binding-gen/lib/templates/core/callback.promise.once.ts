import type { Template } from '../../templates/schema.ts';

// Promise-based callback: returns a Promise<number> resolved from worker thread via TSFN
const tpl: Template = {
  id: 'core.callback.promise.once',
  kind: 'callback',
  description: 'Return a Promise resolved with a number using ThreadSafeFunction.',
  cxx: {
    headers: [
      '#include <napi.h>',
      '#include <thread>'
    ],
    helpers: [],
    declarations: [],
    definitions: [
      `static Napi::Value {{fn}}(const Napi::CallbackInfo& info) {\n  Napi::Env env = info.Env();\n  if (info.Length() < 1 || !info[0].IsNumber()) {\n    Napi::TypeError::New(env, \"Expected (number)\").ThrowAsJavaScriptException();\n    return env.Undefined();\n  }\n  double v = info[0].As<Napi::Number>().DoubleValue();\n  auto deferred = Napi::Promise::Deferred::New(env);\n  Napi::Function noop = Napi::Function::New(env, [](const Napi::CallbackInfo&){});\n  auto tsfn = Napi::ThreadSafeFunction::New(env, noop, \"promise\", 0, 1);\n  std::thread t([tsfn, deferred, v]() mutable {\n    tsfn.BlockingCall(new double(v), [deferred](Napi::Env env, Napi::Function, double* value){\n      deferred.Resolve(Napi::Number::New(env, *value));\n      delete value;\n    });\n    tsfn.Release();\n  });\n  t.detach();\n  return deferred.Promise();\n}\n`
    ],
    moduleInit: [
      `exports.Set(\"{{fn}}\", Napi::Function::New(env, {{fn}}));`
    ]
  },
  ts: {
    parts: [
      `export function {{jsName}}(value: number): Promise<number> {\n  return addon.{{fn}}(value) as Promise<number>;\n}\n`
    ]
  }
};

export default tpl;

