import type { HeaderAST, Function, Enum, Struct, Typedef, Interface } from 'h-parser';
import { TypeMapper } from '../type-mapper.ts';

export class CppGenerator {
  private includes: Set<string> = new Set();
  private ast: HeaderAST;
  private libraryName: string;
  private headerIncludePath?: string;
  private generatedConstants: Set<string> = new Set();
  private generatedFunctions: Set<string> = new Set();
  private generatedStructs: Set<string> = new Set();

  constructor(ast: HeaderAST, libraryName: string = 'binding', headerIncludePath?: string) {
    this.ast = ast;
    this.libraryName = libraryName;
    this.headerIncludePath = headerIncludePath;
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
    if (this.headerIncludePath) {
      headers.push(`#include <${this.headerIncludePath}>`);
    } else if (this.ast.header) {
      headers.push(`#include <${this.ast.header}>`);
    }

    // Add any additional includes
    for (const include of this.includes) {
      headers.push(`#include <${include}>`);
    }

    return headers.join('\n');
  }

  private formatComment(comment: string): string {
    // Split comment into lines and ensure each line starts with //
    return comment.split('\n').map(line => `// ${line}`).join('\n');
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
        lines.push(this.formatComment(enumDef.documentation));
      }
      lines.push(`// enum ${enumDef.name}`);
      
      for (const constant of enumDef.constants) {
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        const functionName = `Get_${safeName}`;
        
        // Skip if we've already generated this constant
        if (this.generatedConstants.has(functionName)) {
          continue;
        }
        this.generatedConstants.add(functionName);
        
        if (constant.documentation) {
          lines.push(this.formatComment(constant.documentation));
        }
        lines.push(`static Napi::Value ${functionName}(const Napi::CallbackInfo& info) {`);
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
      
      // Skip opaque pointer types (ending with Impl or that are forward declarations)
      if (struct.name.endsWith('Impl') || struct.fields.length === 0) {
        continue;
      }
      
      const safeName = TypeMapper.sanitizeIdentifier(struct.name);
      const createFunctionName = `Create_${safeName}`;
      const getFieldFunctionName = `Get_${safeName}_Field`;
      
      // Skip if already generated
      if (this.generatedStructs.has(createFunctionName)) {
        continue;
      }
      this.generatedStructs.add(createFunctionName);
      this.generatedStructs.add(getFieldFunctionName);
      
      if (struct.documentation) {
        lines.push(this.formatComment(struct.documentation));
      }

      // Create struct wrapper
      lines.push(`static Napi::Value ${createFunctionName}(const Napi::CallbackInfo& info) {`);
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
          // Cast to appropriate Napi type first
          let napiValue = `obj.Get("${field.name}")`;
          if (mapping.napiType === 'Napi::String') {
            napiValue = `${napiValue}.As<Napi::String>()`;
          } else if (mapping.napiType === 'Napi::Number') {
            napiValue = `${napiValue}.As<Napi::Number>()`;
          } else if (mapping.napiType === 'Napi::Boolean') {
            napiValue = `${napiValue}.As<Napi::Boolean>()`;
          } else if (mapping.napiType === 'Napi::BigInt') {
            napiValue = `${napiValue}.As<Napi::BigInt>()`;
          }
          
          // Special handling for const char* fields
          if (field.type === 'const char *' || field.type === 'const char*') {
            lines.push(`            std::string ${field.name}_str = ${napiValue}.Utf8Value();`);
            lines.push(`            ptr->${field.name} = ${field.name}_str.c_str();`);
          } else {
            lines.push(`            ptr->${field.name} = ${TypeMapper.getNapiToC(napiValue, field.type)};`);
          }
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
      lines.push(`static Napi::Value ${getFieldFunctionName}(const Napi::CallbackInfo& info) {`);
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
      const wrapperName = `${safeName}_wrapper`;
      
      // Skip if we've already generated this function
      if (this.generatedFunctions.has(wrapperName)) {
        continue;
      }
      this.generatedFunctions.add(wrapperName);
      
      if (func.documentation) {
        lines.push(this.formatComment(func.documentation));
      }

      lines.push(`static Napi::Value ${wrapperName}(const Napi::CallbackInfo& info) {`);
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
        if (TypeMapper.isStructType(paramType)) {
          // Struct passed by value - unwrap the pointer and dereference
          lines.push(`    ${paramType} ${paramName} = *static_cast<${paramType}*>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
        } else if (TypeMapper.isPointerType(paramType) && !TypeMapper.isStringType(paramType)) {
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
      } else if (TypeMapper.isStructType(returnType)) {
        // Struct returned by value - allocate on heap and wrap as pointer
        lines.push(`    ${returnType} result = ${func.name}(${paramNames});`);
        lines.push(`    ${returnType}* resultPtr = new ${returnType}(result);`);
        lines.push(`    return wrapPointer(env, resultPtr, "${returnType}");`);
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

    // Track exported names to avoid duplicates
    const exportedNames = new Set<string>();

    // Export enum constants
    for (const enumDef of this.ast.enums) {
      for (const constant of enumDef.constants) {
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        const functionName = `Get_${safeName}`;
        
        // Only export if we actually generated this constant
        if (this.generatedConstants.has(functionName) && !exportedNames.has(constant.name)) {
          lines.push(`    exports.Set("${constant.name}", Napi::Function::New(env, ${functionName}));`);
          exportedNames.add(constant.name);
        }
      }
    }

    // Export struct creators
    for (const struct of this.ast.structs) {
      if (!struct.name) continue;
      
      // Skip opaque pointer types
      if (struct.name.endsWith('Impl') || struct.fields.length === 0) {
        continue;
      }
      
      const safeName = TypeMapper.sanitizeIdentifier(struct.name);
      const createFunctionName = `Create_${safeName}`;
      const getFieldFunctionName = `Get_${safeName}_Field`;
      const createName = `create_${struct.name}`;
      const getName = `get_${struct.name}_field`;
      
      // Only export if we generated it and haven't exported yet
      if (this.generatedStructs.has(createFunctionName) && !exportedNames.has(createName)) {
        lines.push(`    exports.Set("${createName}", Napi::Function::New(env, ${createFunctionName}));`);
        exportedNames.add(createName);
      }
      if (this.generatedStructs.has(getFieldFunctionName) && !exportedNames.has(getName)) {
        lines.push(`    exports.Set("${getName}", Napi::Function::New(env, ${getFieldFunctionName}));`);
        exportedNames.add(getName);
      }
    }

    // Export functions
    for (const func of this.ast.functions || []) {
      const safeName = TypeMapper.sanitizeIdentifier(func.name);
      if (!exportedNames.has(func.name)) {
        lines.push(`    exports.Set("${func.name}", Napi::Function::New(env, ${safeName}_wrapper));`);
        exportedNames.add(func.name);
      }
    }

    lines.push(`    return exports;`);
    lines.push(`}`);
    lines.push('');
    lines.push(`NODE_API_MODULE(${this.libraryName}, Init)`);

    return lines.join('\n');
  }
}