import type { TypeMapping } from './types.ts';

export class TypeMapper {
  private static knownStructTypes: Set<string> = new Set();
  private static typedefMap: Map<string, string> = new Map();
  
  static setKnownStructTypes(structNames: string[]) {
    this.knownStructTypes = new Set(structNames);
  }
  
  static setTypedefs(typedefs: Array<{ name: string; underlying: string }>) {
    this.typedefMap = new Map(typedefs.map(t => [t.name, t.underlying]));
  }
  
  private static readonly primitiveTypes: Map<string, TypeMapping> = new Map([
    [
      'void',
      {
        cType: 'void',
        napiType: 'void',
        tsType: 'void',
        needsConversion: false,
      },
    ],
    [
      'bool',
      {
        cType: 'bool',
        napiType: 'Napi::Boolean',
        tsType: 'boolean',
        needsConversion: true,
      },
    ],
    [
      'char',
      {
        cType: 'char',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'unsigned char',
      {
        cType: 'unsigned char',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'short',
      {
        cType: 'short',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'unsigned short',
      {
        cType: 'unsigned short',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'int',
      {
        cType: 'int',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'unsigned int',
      {
        cType: 'unsigned int',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'long',
      {
        cType: 'long',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'unsigned long',
      {
        cType: 'unsigned long',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'long long',
      {
        cType: 'long long',
        napiType: 'Napi::BigInt',
        tsType: 'bigint',
        needsConversion: true,
      },
    ],
    [
      'unsigned long long',
      {
        cType: 'unsigned long long',
        napiType: 'Napi::BigInt',
        tsType: 'bigint',
        needsConversion: true,
      },
    ],
    [
      'float',
      {
        cType: 'float',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'double',
      {
        cType: 'double',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'size_t',
      {
        cType: 'size_t',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
    [
      'char *',
      {
        cType: 'char *',
        napiType: 'Napi::String',
        tsType: 'string',
        needsConversion: true,
        conversionTo: '$VAR.As<Napi::String>().Utf8Value().c_str()',
        conversionFrom: 'Napi::String::New(env, $VAR)',
      },
    ],
    [
      'ssize_t',
      {
        cType: 'ssize_t',
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      },
    ],
  ]);

  static getMapping(cType: string): TypeMapping {
    // Remove const qualifier
    const cleanType = cType.replace(/^const\s+/, '').replace(/\s+const$/, '');

    // Check primitive types
    if (TypeMapper.primitiveTypes.has(cleanType)) {
      return TypeMapper.primitiveTypes.get(cleanType)!;
    }

    // Handle char* and const char* as strings
    if (
      cleanType === 'char *' ||
      cleanType === 'const char *' ||
      cType === 'const char *'
    ) {
      return {
        cType: cType,
        napiType: 'Napi::String',
        tsType: 'string',
        needsConversion: true,
        conversionTo: 'std::string($VAR.Utf8Value())',
        conversionFrom: 'Napi::String::New(env, $VAR)',
      };
    }

    // Handle void*
    if (cleanType === 'void *') {
      return {
        cType: cType,
        napiType: 'Napi::External<void>',
        tsType: 'unknown',
        needsConversion: true,
        conversionTo: '$VAR.Data()',
        conversionFrom: 'Napi::External<void>::New(env, $VAR)',
      };
    }

    // Handle other pointers as external objects
    if (cleanType.endsWith('*')) {
      const baseType = cleanType.slice(0, -1).trim();
      return {
        cType: cType,
        napiType: `Napi::External<${baseType}>`,
        tsType: `{ _ptr: unknown }`,
        needsConversion: true,
        conversionTo: `$VAR.As<Napi::External<${baseType}>>().Data()`,
        conversionFrom: `wrapPointer(env, $VAR, "${baseType}")`,
      };
    }

    // Handle arrays (simplified - may need more work)
    if (cleanType.includes('[') && cleanType.includes(']')) {
      return {
        cType: cType,
        napiType: 'Napi::Array',
        tsType: 'unknown[]',
        needsConversion: true,
      };
    }

    // Handle enums as numbers
    if (cleanType.startsWith('enum ')) {
      return {
        cType: cType,
        napiType: 'Napi::Number',
        tsType: 'number',
        needsConversion: true,
      };
    }

    // Handle structs/classes as objects
    if (cleanType.startsWith('struct ') || cleanType.startsWith('class ')) {
      return {
        cType: cType,
        napiType: 'Napi::Object',
        tsType: `{ [key: string]: unknown }`,
        needsConversion: true,
      };
    }

    // Default: treat as opaque pointer
    return {
      cType: cType,
      napiType: `Napi::External<void>`,
      tsType: `{ _type: '${cType}' }`,
      needsConversion: true,
      conversionTo: '$VAR.As<Napi::External<void>>().Data()',
      conversionFrom: 'Napi::External<void>::New(env, $VAR)',
    };
  }

  static getNapiToC(napiVar: string, cType: string): string {
    const mapping = TypeMapper.getMapping(cType);
    if (!mapping.needsConversion) return napiVar;

    if (mapping.conversionTo) {
      return mapping.conversionTo.replace(/\$VAR/g, napiVar);
    }

    // Default conversions
    if (mapping.napiType === 'Napi::Number') {
      if (cType.includes('float') || cType.includes('double')) {
        return `${napiVar}.As<Napi::Number>().DoubleValue()`;
      }
      if (cType.includes('unsigned')) {
        return `${napiVar}.As<Napi::Number>().Uint32Value()`;
      }
      return `${napiVar}.As<Napi::Number>().Int32Value()`;
    }

    if (mapping.napiType === 'Napi::Boolean') {
      return `${napiVar}.As<Napi::Boolean>().Value()`;
    }

    if (mapping.napiType === 'Napi::BigInt') {
      if (cType.includes('unsigned')) {
        return `${napiVar}.As<Napi::BigInt>().Uint64Value(nullptr)`;
      }
      return `${napiVar}.As<Napi::BigInt>().Int64Value(nullptr)`;
    }

    return napiVar;
  }

  static getCToNapi(
    cVar: string,
    cType: string,
    envVar: string = 'env',
  ): string {
    const mapping = TypeMapper.getMapping(cType);
    if (!mapping.needsConversion) return cVar;

    if (mapping.conversionFrom) {
      return mapping.conversionFrom
        .replace(/\$VAR/g, cVar)
        .replace(/\benv\b/g, envVar);
    }

    // Default conversions
    if (mapping.napiType === 'Napi::Number') {
      return `Napi::Number::New(${envVar}, ${cVar})`;
    }

    if (mapping.napiType === 'Napi::Boolean') {
      return `Napi::Boolean::New(${envVar}, ${cVar})`;
    }

    if (mapping.napiType === 'Napi::BigInt') {
      return `Napi::BigInt::New(${envVar}, ${cVar})`;
    }

    return cVar;
  }

  static getTsType(cType: string): string {
    return TypeMapper.getMapping(cType).tsType;
  }

  static isPointerType(cType: string): boolean {
    const cleanType = cType.replace(/^const\s+/, '').replace(/\s+const$/, '');
    
    // Check if it's a typedef that resolves to a pointer
    if (this.typedefMap.has(cleanType)) {
      const underlying = this.typedefMap.get(cleanType)!;
      // Recursively check if the underlying type is a pointer
      return this.isPointerType(underlying);
    }
    
    return cleanType.includes('*') && !TypeMapper.isStringType(cType);
  }

  static isStringType(cType: string): boolean {
    return cType === 'char *' || cType === 'const char *';
  }

  static isVoidType(cType: string): boolean {
    return cType === 'void';
  }

  static isEnumType(cType: string): boolean {
    const cleanType = cType.replace(/^const\s+/, '').replace(/\s+const$/, '');
    
    // Pointers are never enums
    if (cleanType.includes('*')) return false;
    
    // Check if it's a typedef that resolves to a pointer (never an enum)
    if (this.typedefMap.has(cleanType)) {
      const underlying = this.typedefMap.get(cleanType)!;
      if (this.isPointerType(underlying)) {
        return false;
      }
    }
    
    // Check for explicit enum prefix
    if (cleanType.startsWith('enum ')) return true;
    
    // Check for common libclang enum patterns
    if (cleanType.startsWith('CX') && (
      cleanType.includes('Kind') ||
      cleanType.includes('Result') ||
      cleanType.includes('Flag') ||
      cleanType.includes('Option') ||
      cleanType.includes('Property') ||
      cleanType === 'CXIdxAttrKind' ||
      cleanType === 'CXTUResourceUsageKind'
    )) return true;
    
    // Check for other C enum patterns
    if (cleanType.startsWith('CX_')) return true;
    
    return false;
  }

  static isFunctionPointerType(cType: string): boolean {
    return cType.includes('(*') && cType.includes(')');
  }

  static isStructType(cType: string): boolean {
    // A struct type is one that is not a primitive, not a pointer, and not void
    const cleanType = cType.replace(/^const\s+/, '').replace(/\s+const$/, '');

    // Not a pointer
    if (cleanType.includes('*')) return false;

    // Check if it's a typedef that resolves to a pointer - NOT a struct!
    if (this.typedefMap.has(cleanType)) {
      const underlying = this.typedefMap.get(cleanType)!;
      if (this.isPointerType(underlying)) {
        return false;  // It's a typedef to a pointer, not a struct
      }
    }

    // Not a primitive
    if (TypeMapper.primitiveTypes.has(cleanType)) return false;

    // Not void
    if (cleanType === 'void') return false;

    // Not an enum (though enums are handled as numbers)
    if (this.isEnumType(cleanType)) return false;

    // Check if it's in our known struct types
    if (TypeMapper.knownStructTypes.has(cleanType)) return true;

    // Handle common size_t type
    if (cleanType === 'size_t') return false;
    
    // If it starts with uppercase letter, likely a struct/typedef (C convention)
    if (/^[A-Z]/.test(cleanType)) return true;

    // Likely a struct/class or typedef
    return true;
  }
  
  static isStructPointer(cType: string): boolean {
    if (!cType.includes('*')) return false;
    const baseType = cType.replace(/\s*\*\s*$/, '').replace(/^const\s+/, '').trim();
    return this.isStructType(baseType);
  }
  
  static isArrayType(cType: string): boolean {
    return cType.includes('[') && cType.includes(']');
  }
  
  static getArrayElementType(cType: string): string {
    if (!this.isArrayType(cType)) return cType;
    return cType.substring(0, cType.indexOf('['));
  }

  static sanitizeIdentifier(name: string): string {
    // Replace invalid characters with underscores
    return name.replace(/[^a-zA-Z0-9_]/g, '_');
  }
}
