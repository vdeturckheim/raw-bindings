import type { HeaderAST } from 'h-parser';
import { TypeMapper } from '../type-mapper.ts';

export class CppGenerator {
  private includes: Set<string> = new Set();
  private ast: HeaderAST;
  private libraryName: string;
  private headerIncludePath: string | undefined;
  private headerIncludePaths: string[] = [];
  private generatedConstants: Set<string> = new Set();
  private generatedFunctions: Set<string> = new Set();
  private generatedStructs: Set<string> = new Set();

  constructor(
    ast: HeaderAST,
    libraryName: string = 'binding',
    headerIncludePath?: string,
    headerIncludePaths?: string[],
  ) {
    this.ast = ast;
    this.libraryName = libraryName;
    this.headerIncludePath = headerIncludePath || undefined;
    this.headerIncludePaths = headerIncludePaths || [];
  }

  generate(): string {
    const sections: string[] = [];

    // Initialize TypeMapper with known struct types
    const structNames = this.ast.structs.map(s => s.name);
    TypeMapper.setKnownStructTypes(structNames);
    
    // Initialize TypeMapper with typedef information
    if (this.ast.typedefs) {
      TypeMapper.setTypedefs(this.ast.typedefs);
    }

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
      '#include <memory>',
      '#include <cstring>',  // for strncpy
    ];

    // Include headers from multiple header paths if available
    if (this.headerIncludePaths.length > 0) {
      for (const headerPath of this.headerIncludePaths) {
        headers.push(`#include <${headerPath}>`);
      }
    }
    // Fallback to single header path
    else if (this.headerIncludePath) {
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
    return comment
      .split('\n')
      .map((line) => `// ${line}`)
      .join('\n');
  }

  private generateHelpers(): string {
    return `// Helper function to wrap pointers as JavaScript objects
static Napi::Object wrapPointer(Napi::Env env, void* ptr, const std::string& typeName) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("_ptr", Napi::External<void>::New(env, ptr));
    obj.Set("_type", Napi::String::New(env, typeName));
    return obj;
}

// Overload for const pointers
static Napi::Object wrapPointer(Napi::Env env, const void* ptr, const std::string& typeName) {
    Napi::Object obj = Napi::Object::New(env);
    obj.Set("_ptr", Napi::External<void>::New(env, const_cast<void*>(ptr)));
    obj.Set("_type", Napi::String::New(env, typeName));
    obj.Set("_const", Napi::Boolean::New(env, true));
    return obj;
}

// Helper function to wrap const pointers as JavaScript objects (kept for compatibility)
static Napi::Object wrapConstPointer(Napi::Env env, const void* ptr, const std::string& typeName) {
    return wrapPointer(env, ptr, typeName);
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
        lines.push(
          `static Napi::Value ${functionName}(const Napi::CallbackInfo& info) {`,
        );
        lines.push(
          `    return Napi::Number::New(info.Env(), ${constant.name});`,
        );
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

      // Handle anonymous structs with better naming
      let structName = struct.name;
      let actualTypeName = struct.name;
      
      if (structName.includes('(unnamed at ') || structName.includes('__') || structName.includes('/')) {
        // Look for a typedef that might reference this struct
        const typedef = this.ast.typedefs?.find(t => t.underlying === struct.name || t.spelling === struct.name);
        if (typedef) {
          actualTypeName = typedef.name;
          structName = typedef.name;
        } else {
          // Use a generic name for function generation, but we can't use the struct
          structName = `AnonymousStruct_${this.generatedStructs.size}`;
          // Skip generating this struct since we can't reference it properly
          continue;
        }
      }

      const safeName = TypeMapper.sanitizeIdentifier(structName);
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
      lines.push(
        `static Napi::Value ${createFunctionName}(const Napi::CallbackInfo& info) {`,
      );
      lines.push(`    Napi::Env env = info.Env();`);
      lines.push(`    `);
      lines.push(`    // Allocate struct`);
      lines.push(`    ${actualTypeName}* ptr = new ${actualTypeName}();`);
      lines.push(`    `);
      lines.push(`    // Initialize from JavaScript object if provided`);
      lines.push(`    if (info.Length() > 0 && info[0].IsObject()) {`);
      lines.push(`        Napi::Object obj = info[0].As<Napi::Object>();`);

      for (const field of struct.fields) {
        // Skip fields with empty names (anonymous union members)
        if (!field.name || field.name.trim() === '') {
          continue;
        }
        
        lines.push(`        if (obj.Has("${field.name}")) {`);

        // Handle different field types appropriately
        if (TypeMapper.isArrayType(field.type)) {
          // Handle arrays specially
          const elementType = TypeMapper.getArrayElementType(field.type);
          if (elementType === 'char') {
            // String arrays - copy as C strings
            lines.push(`            std::string ${field.name}_str = obj.Get("${field.name}").As<Napi::String>().Utf8Value();`);
            lines.push(`            strncpy(ptr->${field.name}, ${field.name}_str.c_str(), sizeof(ptr->${field.name}) - 1);`);
            lines.push(`            ptr->${field.name}[sizeof(ptr->${field.name}) - 1] = '\\0';`);
          } else {
            // Numeric arrays - copy from JS array
            lines.push(`            Napi::Array ${field.name}_arr = obj.Get("${field.name}").As<Napi::Array>();`);
            lines.push(`            for (size_t i = 0; i < ${field.name}_arr.Length() && i < sizeof(ptr->${field.name})/sizeof(ptr->${field.name}[0]); i++) {`);
            if (elementType === 'void *' || elementType === 'const void *') {
              lines.push(`                if (${field.name}_arr.Get(i).IsExternal()) {`);
              lines.push(`                    ptr->${field.name}[i] = ${field.name}_arr.Get(i).As<Napi::External<void>>().Data();`);
              lines.push(`                } else {`);
              lines.push(`                    ptr->${field.name}[i] = nullptr;`);
              lines.push(`                }`);
            } else {
              lines.push(`                ptr->${field.name}[i] = ${TypeMapper.getNapiToC(`${field.name}_arr.Get(i)`, elementType)};`);
            }
            lines.push(`            }`);
          }
        } else if (TypeMapper.isStructType(field.type)) {
          // Handle nested structs - unwrap pointer and copy struct
          lines.push(`            ${field.type}* ${field.name}_ptr = static_cast<${field.type}*>(unwrapPointer(obj.Get("${field.name}").As<Napi::Object>()));`);
          lines.push(`            if (${field.name}_ptr) {`);
          lines.push(`                ptr->${field.name} = *${field.name}_ptr;`);
          lines.push(`            }`);
        } else if (TypeMapper.isEnumType(field.type)) {
          // Handle enum types with explicit casting
          lines.push(`            ptr->${field.name} = static_cast<${field.type}>(obj.Get("${field.name}").As<Napi::Number>().Int32Value());`);
        } else if (TypeMapper.isFunctionPointerType(field.type)) {
          // Skip function pointer fields - they can't be assigned from JavaScript safely
          lines.push(`            // Note: Function pointer field ${field.name} skipped - not assignable from JavaScript`);
        } else if (TypeMapper.isStringType(field.type)) {
          // Special handling for string fields - store the string and keep it alive
          lines.push(`            // Note: String lifetime management needed for production use`);
          lines.push(`            std::string ${field.name}_str = obj.Get("${field.name}").As<Napi::String>().Utf8Value();`);
          lines.push(`            ptr->${field.name} = ${field.name}_str.c_str();`);
        } else if (field.type === 'void *' || field.type === 'void*') {
          // Handle void* fields specifically
          lines.push(`            if (obj.Get("${field.name}").IsExternal()) {`);
          lines.push(`                ptr->${field.name} = obj.Get("${field.name}").As<Napi::External<void>>().Data();`);
          lines.push(`            } else {`);
          lines.push(`                ptr->${field.name} = nullptr;`);
          lines.push(`            }`);
        } else if (field.type.includes('*')) {
          // Handle ALL pointer types (including ones not caught by isPointerType)
          lines.push(`            if (obj.Get("${field.name}").IsObject()) {`);
          lines.push(`                ptr->${field.name} = static_cast<${field.type}>(unwrapPointer(obj.Get("${field.name}").As<Napi::Object>()));`);
          lines.push(`            } else if (obj.Get("${field.name}").IsNull() || obj.Get("${field.name}").IsUndefined()) {`);
          lines.push(`                ptr->${field.name} = nullptr;`);
          lines.push(`            } else {`);
          lines.push(`                // Invalid value for pointer field`);
          lines.push(`                ptr->${field.name} = nullptr;`);
          lines.push(`            }`);
        } else {
          // Handle regular types (non-pointers)
          const mapping = TypeMapper.getMapping(field.type);
          if (mapping.needsConversion) {
            lines.push(
              `            ptr->${field.name} = ${TypeMapper.getNapiToC(`obj.Get("${field.name}")`, field.type)};`,
            );
          } else {
            lines.push(
              `            ptr->${field.name} = obj.Get("${field.name}");`,
            );
          }
        }

        lines.push(`        }`);
      }

      lines.push(`    }`);
      lines.push(`    `);
      lines.push(`    return wrapPointer(env, ptr, "${structName}");`);
      lines.push(`}`);
      lines.push('');

      // Getter for struct fields
      lines.push(
        `static Napi::Value ${getFieldFunctionName}(const Napi::CallbackInfo& info) {`,
      );
      lines.push(`    Napi::Env env = info.Env();`);
      lines.push(`    `);
      lines.push(
        `    if (info.Length() < 2 || !info[0].IsObject() || !info[1].IsString()) {`,
      );
      lines.push(
        `        Napi::TypeError::New(env, "Expected struct object and field name").ThrowAsJavaScriptException();`,
      );
      lines.push(`        return env.Undefined();`);
      lines.push(`    }`);
      lines.push(`    `);
      lines.push(
        `    ${actualTypeName}* ptr = static_cast<${actualTypeName}*>(unwrapPointer(info[0].As<Napi::Object>()));`,
      );
      lines.push(
        `    std::string fieldName = info[1].As<Napi::String>().Utf8Value();`,
      );
      lines.push(`    `);

      for (const field of struct.fields) {
        // Skip fields with empty names (anonymous union members)
        if (!field.name || field.name.trim() === '') {
          continue;
        }
        
        lines.push(`    if (fieldName == "${field.name}") {`);
        
        // Handle different field types in getters
        if (TypeMapper.isArrayType(field.type)) {
          const elementType = TypeMapper.getArrayElementType(field.type);
          if (elementType === 'char') {
            // Return string arrays as strings
            lines.push(`        return Napi::String::New(env, ptr->${field.name});`);
          } else {
            // Return numeric arrays as JS arrays
            lines.push(`        Napi::Array arr = Napi::Array::New(env);`);
            lines.push(`        size_t arraySize = sizeof(ptr->${field.name})/sizeof(ptr->${field.name}[0]);`);
            lines.push(`        for (size_t i = 0; i < arraySize; i++) {`);
            if (elementType === 'void *' || elementType === 'const void *') {
              lines.push(`            arr.Set(i, Napi::External<void>::New(env, const_cast<void*>(ptr->${field.name}[i])));`);
            } else {
              lines.push(`            arr.Set(i, ${TypeMapper.getCToNapi(`ptr->${field.name}[i]`, elementType)});`);
            }
            lines.push(`        }`);
            lines.push(`        return arr;`);
          }
        } else if (TypeMapper.isStructType(field.type)) {
          // Return nested structs as new wrapped pointers
          lines.push(`        ${field.type}* fieldPtr = new ${field.type}(ptr->${field.name});`);
          lines.push(`        return wrapPointer(env, fieldPtr, "${field.type}");`);
        } else if (TypeMapper.isFunctionPointerType(field.type)) {
          // Handle function pointers - return as external pointer
          lines.push(`        return Napi::External<void>::New(env, reinterpret_cast<void*>(ptr->${field.name}));`);
        } else if (TypeMapper.isPointerType(field.type) && field.type.includes('const')) {
          // Handle const pointers specially
          const baseType = field.type.replace(/\s*\*\s*$/, '').replace(/^const\s+/, '');
          lines.push(`        return wrapConstPointer(env, ptr->${field.name}, "${baseType}");`);
        } else if (TypeMapper.isPointerType(field.type)) {
          // Handle regular pointers
          const baseType = field.type.replace(/\s*\*\s*$/, '');
          lines.push(`        return wrapPointer(env, ptr->${field.name}, "${baseType}");`);
        } else if (TypeMapper.isEnumType(field.type)) {
          // Handle enum fields - return as number
          lines.push(`        return Napi::Number::New(env, static_cast<int>(ptr->${field.name}));`);
        } else {
          // Handle regular types
          lines.push(
            `        return ${TypeMapper.getCToNapi(`ptr->${field.name}`, field.type)};`,
          );
        }
        
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

      lines.push(
        `static Napi::Value ${wrapperName}(const Napi::CallbackInfo& info) {`,
      );
      lines.push(`    Napi::Env env = info.Env();`);
      lines.push(`    `);

      // Parameter validation
      if (func.params.length > 0) {
        lines.push(`    if (info.Length() < ${func.params.length}) {`);
        lines.push(
          `        Napi::TypeError::New(env, "Expected ${func.params.length} arguments").ThrowAsJavaScriptException();`,
        );
        lines.push(`        return env.Undefined();`);
        lines.push(`    }`);
        lines.push(`    `);
      }

      // Special complete handling for clang_visitChildren
      if (func.name === 'clang_visitChildren') {
        // Handle all three parameters specially
        lines.push(`    // Parameter: parent (CXCursor)`);
        lines.push(`    CXCursor parent = *static_cast<CXCursor*>(unwrapPointer(info[0].As<Napi::Object>()));`);
        lines.push(`    `);
        lines.push(`    // Parameter: visitor (JavaScript callback function)`);
        lines.push(`    if (!info[1].IsFunction()) {`);
        lines.push(`        Napi::TypeError::New(env, "Expected function for visitor parameter").ThrowAsJavaScriptException();`);
        lines.push(`        return env.Undefined();`);
        lines.push(`    }`);
        lines.push(`    Napi::Function jsVisitor = info[1].As<Napi::Function>();`);
        lines.push(`    `);
        lines.push(`    // Parameter: client_data (ignored - we use our own)`);
        lines.push(`    // Note: We ignore the JavaScript client_data and use our own for the callback`);
        lines.push(`    `);
        lines.push(`    // Create C++ visitor function`);
        lines.push(`    auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData client_data) -> enum CXChildVisitResult {`);
        lines.push(`        auto* callbackData = static_cast<std::pair<Napi::Function*, Napi::Env*>*>(client_data);`);
        lines.push(`        Napi::Function* jsFunc = callbackData->first;`);
        lines.push(`        Napi::Env* env = callbackData->second;`);
        lines.push(`        `);
        lines.push(`        // Wrap cursors for JavaScript`);
        lines.push(`        CXCursor* cursorPtr = new CXCursor(cursor);`);
        lines.push(`        Napi::Object cursorObj = wrapPointer(*env, cursorPtr, "CXCursor");`);
        lines.push(`        `);
        lines.push(`        CXCursor* parentPtr = new CXCursor(parent);`);
        lines.push(`        Napi::Object parentObj = wrapPointer(*env, parentPtr, "CXCursor");`);
        lines.push(`        `);
        lines.push(`        // Call JavaScript function`);
        lines.push(`        Napi::Value result = jsFunc->Call({cursorObj, parentObj});`);
        lines.push(`        `);
        lines.push(`        // Convert result to enum`);
        lines.push(`        if (result.IsNumber()) {`);
        lines.push(`            return static_cast<enum CXChildVisitResult>(result.As<Napi::Number>().Int32Value());`);
        lines.push(`        }`);
        lines.push(`        return CXChildVisit_Break; // Default to break on error`);
        lines.push(`    };`);
        lines.push(`    `);
        lines.push(`    // Create callback data`);
        lines.push(`    std::pair<Napi::Function*, Napi::Env*> callbackData(&jsVisitor, &env);`);
        lines.push(`    `);
        lines.push(`    // Call clang_visitChildren with our lambda`);
        lines.push(`    auto result = clang_visitChildren(parent, visitor, &callbackData);`);
        lines.push(`    return Napi::Number::New(env, result);`);
        lines.push(`}`);
        lines.push('');
        continue; // Skip the rest of function generation
      }

      // Convert parameters
      for (let i = 0; i < func.params.length; i++) {
        const param = func.params[i];
        if (!param) continue;
        const paramName = param.name || `arg${i}`;
        const paramType = param.type.spelling;

        lines.push(`    // Parameter: ${paramName} (${paramType})`);

        const mapping = TypeMapper.getMapping(paramType);
        if (TypeMapper.isStructType(paramType)) {
          // Struct passed by value - unwrap the pointer and dereference
          lines.push(
            `    ${paramType} ${paramName} = *static_cast<${paramType}*>(unwrapPointer(info[${i}].As<Napi::Object>()));`,
          );
        } else if (TypeMapper.isEnumType(paramType)) {
          // Enum parameters need explicit casting
          lines.push(
            `    ${paramType} ${paramName} = static_cast<${paramType}>(info[${i}].As<Napi::Number>().Int32Value());`,
          );
        } else if (TypeMapper.isFunctionPointerType(paramType)) {
          // Handle other function pointers the old way
          const typedefType = paramType.replace(/\(\*\)/, `(*${paramName}_t)`);
          lines.push(`    typedef ${typedefType};`);
          lines.push(
            `    ${paramName}_t ${paramName} = reinterpret_cast<${paramName}_t>(unwrapPointer(info[${i}].As<Napi::Object>()));`,
          );
        } else if (TypeMapper.isStructPointer(paramType)) {
          // Handle struct pointers (including const)
          // Allow null/undefined for optional struct pointers
          lines.push(`    ${paramType} ${paramName} = nullptr;`);
          lines.push(`    if (!info[${i}].IsNull() && !info[${i}].IsUndefined() && info[${i}].IsObject()) {`);
          lines.push(`        ${paramName} = static_cast<${paramType}>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
          lines.push(`    }`);
        } else if (paramType === 'size_t *' || paramType === 'int *') {
          // Handle size_t and int pointers first - can be passed as wrapped pointers
          // Note: h-parser sometimes resolves size_t* to int* on some platforms
          // Special case: clang_getFileContents expects size_t* for the size parameter
          if (paramType === 'int *' && func.name === 'clang_getFileContents' && paramName === 'size') {
            // clang_getFileContents actually expects size_t* for the size parameter
            lines.push(`    size_t * ${paramName} = static_cast<size_t *>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
          } else if (paramType === 'size_t *') {
            lines.push(`    size_t * ${paramName} = static_cast<size_t *>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
          } else {
            lines.push(`    int * ${paramName} = static_cast<int *>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
          }
        } else if (paramType === 'const char *const *' || paramType === 'const char * const *' || paramType === 'char **') {
          // Handle string array parameters (usually for command-line args)
          // These can be null or a JavaScript array of strings
          lines.push(`    ${paramType} ${paramName} = nullptr;`);
          lines.push(`    std::vector<std::string> ${paramName}_strings;`);
          lines.push(`    std::vector<const char*> ${paramName}_cstrs;`);
          lines.push(`    if (!info[${i}].IsNull() && !info[${i}].IsUndefined()) {`);
          lines.push(`        if (info[${i}].IsArray()) {`);
          lines.push(`            Napi::Array arr = info[${i}].As<Napi::Array>();`);
          lines.push(`            for (uint32_t j = 0; j < arr.Length(); j++) {`);
          lines.push(`                ${paramName}_strings.push_back(arr.Get(j).As<Napi::String>().Utf8Value());`);
          lines.push(`            }`);
          lines.push(`            for (const auto& s : ${paramName}_strings) {`);
          lines.push(`                ${paramName}_cstrs.push_back(s.c_str());`);
          lines.push(`            }`);
          lines.push(`            ${paramName} = ${paramName}_cstrs.data();`);
          lines.push(`        } else if (info[${i}].IsObject()) {`);
          lines.push(`            // Assume it's a wrapped pointer`);
          lines.push(`            ${paramName} = *static_cast<${paramType}*>(unwrapPointer(info[${i}].As<Napi::Object>()));`);
          lines.push(`        }`);
          lines.push(`    }`);
        } else if (
          TypeMapper.isPointerType(paramType) &&
          !TypeMapper.isStringType(paramType)
        ) {
          // Handle pointer types (including typedef pointers)
          // Need to dereference because unwrapPointer returns a pointer to the stored pointer
          lines.push(
            `    ${paramType} ${paramName} = *static_cast<${paramType}*>(unwrapPointer(info[${i}].As<Napi::Object>()));`,
          );
        } else if (paramType.endsWith(' *') && TypeMapper.isEnumType(paramType.replace(' *', ''))) {
          // Handle enum pointers
          const enumType = paramType.replace(' *', '');
          lines.push(`    ${enumType} ${paramName}_val = static_cast<${enumType}>(info[${i}].As<Napi::Number>().Int32Value());`);
          lines.push(`    ${enumType}* ${paramName} = &${paramName}_val;`);
        } else if (paramType === 'size_t') {
          // Handle size_t as number
          lines.push(
            `    size_t ${paramName} = info[${i}].As<Napi::Number>().Uint32Value();`,
          );
        } else if (TypeMapper.isStringType(paramType)) {
          // Handle string parameters - need to keep the string alive
          lines.push(
            `    std::string ${paramName}_str = info[${i}].As<Napi::String>().Utf8Value();`,
          );
          lines.push(
            `    const char* ${paramName} = ${paramName}_str.c_str();`,
          );
        } else if (mapping.needsConversion) {
          lines.push(
            `    auto ${paramName} = ${TypeMapper.getNapiToC(`info[${i}]`, paramType)};`,
          );
        } else {
          lines.push(`    auto ${paramName} = info[${i}];`);
        }
      }

      lines.push(`    `);

      // Special handling for clang_visitChildren - all parameters have been processed above
      // No need to process anymore, just generate the call

      // Call the function
      const returnType = func.return.spelling;
      const paramNames = func.params
        .map((p, i) => p.name || `arg${i}`)
        .join(', ');

      if (TypeMapper.isVoidType(returnType)) {
        lines.push(`    ${func.name}(${paramNames});`);
        lines.push(`    return env.Undefined();`);
      } else if (TypeMapper.isStructType(returnType)) {
        // Struct returned by value - allocate on heap and wrap as pointer
        lines.push(`    ${returnType} result = ${func.name}(${paramNames});`);
        lines.push(`    ${returnType}* resultPtr = new ${returnType}(result);`);
        lines.push(`    return wrapPointer(env, resultPtr, "${returnType}");`);
      } else if (TypeMapper.isPointerType(returnType)) {
        // Handle pointer types (including typedefs to pointers like CXTranslationUnit)
        lines.push(`    ${returnType} result = ${func.name}(${paramNames});`);
        // For typedef pointers, we need to create a heap-allocated copy to match unwrap expectations
        lines.push(`    ${returnType}* resultPtr = new ${returnType};`);
        lines.push(`    *resultPtr = result;`);
        lines.push(`    return wrapPointer(env, resultPtr, "${returnType}");`);
      } else {
        lines.push(`    auto result = ${func.name}(${paramNames});`);
        if (TypeMapper.isEnumType(returnType)) {
          // Return enums as numbers
          lines.push(`    return Napi::Number::New(env, static_cast<int>(result));`);
        } else {
          lines.push(
            `    return ${TypeMapper.getCToNapi('result', returnType)};`,
          );
        }
      }

      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateModuleInit(): string {
    const lines: string[] = [
      `static Napi::Object Init(Napi::Env env, Napi::Object exports) {`,
    ];

    // Track exported names to avoid duplicates
    const exportedNames = new Set<string>();

    // Export enum constants
    for (const enumDef of this.ast.enums) {
      for (const constant of enumDef.constants) {
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        const functionName = `Get_${safeName}`;

        // Only export if we actually generated this constant
        if (
          this.generatedConstants.has(functionName) &&
          !exportedNames.has(constant.name)
        ) {
          lines.push(
            `    exports.Set("${constant.name}", Napi::Function::New(env, ${functionName}));`,
          );
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

      // Handle anonymous structs with better naming (same logic as above)
      let structName = struct.name;
      if (structName.includes('(unnamed at ') || structName.includes('__') || structName.includes('/')) {
        // Look for a typedef that might reference this struct
        const typedef = this.ast.typedefs?.find(t => t.underlying === struct.name || t.spelling === struct.name);
        if (typedef) {
          structName = typedef.name;
        } else {
          // Skip if we can't find a proper name
          continue;
        }
      }

      const safeName = TypeMapper.sanitizeIdentifier(structName);
      const createFunctionName = `Create_${safeName}`;
      const getFieldFunctionName = `Get_${safeName}_Field`;
      const createName = `create_${structName}`;
      const getName = `get_${structName}_field`;

      // Only export if we generated it and haven't exported yet
      if (
        this.generatedStructs.has(createFunctionName) &&
        !exportedNames.has(createName)
      ) {
        lines.push(
          `    exports.Set("${createName}", Napi::Function::New(env, ${createFunctionName}));`,
        );
        exportedNames.add(createName);
      }
      if (
        this.generatedStructs.has(getFieldFunctionName) &&
        !exportedNames.has(getName)
      ) {
        lines.push(
          `    exports.Set("${getName}", Napi::Function::New(env, ${getFieldFunctionName}));`,
        );
        exportedNames.add(getName);
      }
    }

    // Export functions
    for (const func of this.ast.functions || []) {
      const safeName = TypeMapper.sanitizeIdentifier(func.name);
      if (!exportedNames.has(func.name)) {
        lines.push(
          `    exports.Set("${func.name}", Napi::Function::New(env, ${safeName}_wrapper));`,
        );
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
