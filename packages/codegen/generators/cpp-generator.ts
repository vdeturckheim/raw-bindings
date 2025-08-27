import type { HeaderAST, Function, Enum, Struct, Typedef, Interface } from 'h-parser';
import { TypeMapper } from '../type-mapper.ts';

export class CppGenerator {
  private includes: Set<string> = new Set();
  private ast: HeaderAST;
  private libraryName: string;

  constructor(ast: HeaderAST, libraryName: string = 'binding') {
    this.ast = ast;
    this.libraryName = libraryName;
  }

  generate(): string {
    const sections: string[] = [];

    // Headers
    sections.push(this.generateHeaders());

    // Helper functions
    sections.push(this.generateHelpers());

    // Enum constants
    if (this.ast.enums.length > 0) {
      sections.push(this.generateEnumConstants());
    }

    // Struct wrappers
    if (this.ast.structs.length > 0) {
      sections.push(this.generateStructWrappers());
    }

    // Function wrappers
    if (this.ast.functions?.length) {
      sections.push(this.generateFunctionWrappers());
    }

    // Module initialization
    sections.push(this.generateModuleInit());

    return sections.join('\n\n');
  }

  private generateHeaders(): string {
    // Standard NAPI headers
    const headers = [
      '#include <napi.h>',
      '#include <string>',
      '#include <vector>',
      '#include <memory>'
    ];

    // Include the original header
    if (this.ast.header) {
      headers.push(`#include <${this.ast.header}>`);
    }

    // Add any additional includes
    for (const include of this.includes) {
      headers.push(`#include <${include}>`);
    }

    return headers.join('\n');
  }

  private generateHelpers(): string {
    return `// Helper function to wrap pointers as JavaScript objects
static Napi::Object wrapPointer(Napi::Env env, void* ptr, const std::string& typeName) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("_ptr", Napi::External<void>::New(env, ptr));
    obj.Set("_type", Napi::String::New(env, typeName));
    return obj;
}

// Helper to unwrap pointer from JavaScript object
static void* unwrapPointer(Napi::Object obj) {
    if (!obj.Has("_ptr")) {
        return nullptr;
    }
    return obj.Get("_ptr").As<Napi::External<void>>().Data();
}`;
  }

  private generateEnumConstants(): string {
    const lines: string[] = ['// Enum constants'];

    for (const enumDef of this.ast.enums) {
      if (enumDef.documentation) {
        lines.push(`// ${enumDef.documentation}`);
      }
      lines.push(`// enum ${enumDef.name}`);
      
      for (const constant of enumDef.constants) {
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        if (constant.documentation) {
          lines.push(`// ${constant.documentation}`);
        }
        lines.push(`static Napi::Value Get_${safeName}(const Napi::CallbackInfo& info) {`);
        lines.push(`    return Napi::Number::New(info.Env(), ${constant.name});`);
        lines.push(`}`);
        lines.push('');
      }
    }

    return lines.join('\n');
  }

  private generateStructWrappers(): string {
    const lines: string[] = ['// Struct wrappers'];

    for (const struct of this.ast.structs) {
      if (!struct.name) continue;
      
      const safeName = TypeMapper.sanitizeIdentifier(struct.name);
      
      if (struct.documentation) {
        lines.push(`// ${struct.documentation}`);
      }

      // Create struct wrapper
      lines.push(`static Napi::Value Create_${safeName}(const Napi::CallbackInfo& info) {`);
      lines.push(`    Napi::Env env = info.Env();`);
      lines.push(`    `);
      lines.push(`    // Allocate struct`);
      lines.push(`    ${struct.name}* ptr = new ${struct.name}();`);
      lines.push(`    `);
      lines.push(`    // Initialize from JavaScript object if provided`);
      lines.push(`    if (info.Length() > 0 && info[0].IsObject()) {`);
      lines.push(`        Napi::Object obj = info[0].As<Napi::Object>();`);
      
      for (const field of struct.fields) {
        const fieldName = TypeMapper.sanitizeIdentifier(field.name);
        lines.push(`        if (obj.Has("${field.name}")) {`);
        
        const mapping = TypeMapper.getMapping(field.type);
        if (mapping.needsConversion) {
          lines.push(`            ptr->${field.name} = ${TypeMapper.getNapiToC(`obj.Get("${field.name}")`, field.type)};`);
        } else {
          lines.push(`            ptr->${field.name} = obj.Get("${field.name}");`);
        }
        
        lines.push(`        }`);
      }
      
      lines.push(`    }`);
      lines.push(`    `);
      lines.push(`    return wrapPointer(env, ptr, "${struct.name}");`);
      lines.push(`}`);
      lines.push('');

      // Getter for struct fields
      lines.push(`static Napi::Value Get_${safeName}_Field(const Napi::CallbackInfo& info) {`);
      lines.push(`    Napi::Env env = info.Env();`);
      lines.push(`    `);
      lines.push(`    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {`);
      lines.push(`        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();`);
      lines.push(`        return env.Undefined();`);
      lines.push(`    }`);
      lines.push(`    `);
      lines.push(`    ${struct.name}* ptr = static_cast<${struct.name}*>(unwrapPointer(info[0].As<Napi::Object>()));`);
      lines.push(`    std::string fieldName = info[1].As<Napi::String>().Utf8Value();`);
      lines.push(`    `);

      for (const field of struct.fields) {
        lines.push(`    if (fieldName == "${field.name}") {`);
        lines.push(`        return ${TypeMapper.getCToNapi(`ptr->${field.name}`, field.type)};`);
        lines.push(`    }`);
      }

      lines.push(`    `);
      lines.push(`    return env.Undefined();`);
      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateFunctionWrappers(): string {
    const lines: string[] = ['// Function wrappers'];

    for (const func of this.ast.functions || []) {
      const safeName = TypeMapper.sanitizeIdentifier(func.name);
      
      if (func.documentation) {
        lines.push(`// ${func.documentation}`);
      }

      lines.push(`static Napi::Value ${safeName}_wrapper(const Napi::CallbackInfo& info) {`);
      lines.push(`    Napi::Env env = info.Env();`);
      lines.push(`    `);

      // Parameter validation
      if (func.params.length > 0) {
        lines.push(`    if (info.Length() < ${func.params.length}) {`);
        lines.push(`        Napi::TypeError::New(env, "Expected ${func.params.length} arguments").ThrowAsJavaScriptException();`);
        lines.push(`        return env.Undefined();`);
        lines.push(`    }`);
        lines.push(`    `);
      }

      // Convert parameters
      for (let i = 0; i < func.params.length; i++) {
        const param = func.params[i];
        const paramName = param.name || `arg${i}`;
        const paramType = param.type.spelling;
        
        lines.push(`    // Parameter: ${paramName} (${paramType})`);
        
        const mapping = TypeMapper.getMapping(paramType);
        if (TypeMapper.isPointerType(paramType) && !TypeMapper.isStringType(paramType)) {
          lines.push(`    ${paramType} ${paramName} = static_cast<${paramType}>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
        } else if (mapping.needsConversion) {
          lines.push(`    auto ${paramName} = ${TypeMapper.getNapiToC(`info[${i}]`, paramType)};`);
        } else {
          lines.push(`    auto ${paramName} = info[${i}];`);
        }
      }

      lines.push(`    `);

      // Call the function
      const returnType = func.return.spelling;
      const paramNames = func.params.map((p, i) => p.name || `arg${i}`).join(', ');
      
      if (TypeMapper.isVoidType(returnType)) {
        lines.push(`    ${func.name}(${paramNames});`);
        lines.push(`    return env.Undefined();`);
      } else {
        lines.push(`    auto result = ${func.name}(${paramNames});`);
        lines.push(`    return ${TypeMapper.getCToNapi('result', returnType)};`);
      }

      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateModuleInit(): string {
    const lines: string[] = [
      `static Napi::Object Init(Napi::Env env, Napi::Object exports) {`
    ];

    // Export enum constants
    for (const enumDef of this.ast.enums) {
      for (const constant of enumDef.constants) {
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        lines.push(`    exports.Set("${constant.name}", Napi::Function::New(env, Get_${safeName}));`);
      }
    }

    // Export struct creators
    for (const struct of this.ast.structs) {
      if (!struct.name) continue;
      const safeName = TypeMapper.sanitizeIdentifier(struct.name);
      lines.push(`    exports.Set("create_${struct.name}", Napi::Function::New(env, Create_${safeName}));`);
      lines.push(`    exports.Set("get_${struct.name}_field", Napi::Function::New(env, Get_${safeName}_Field));`);
    }

    // Export functions
    for (const func of this.ast.functions || []) {
      const safeName = TypeMapper.sanitizeIdentifier(func.name);
      lines.push(`    exports.Set("${func.name}", Napi::Function::New(env, ${safeName}_wrapper));`);
    }

    lines.push(`    return exports;`);
    lines.push(`}`);
    lines.push('');
    lines.push(`NODE_API_MODULE(${this.libraryName}, Init)`);

    return lines.join('\n');
  }
}