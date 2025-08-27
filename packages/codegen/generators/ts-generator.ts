import type { HeaderAST, Function, Enum, Struct } from 'h-parser';
import { TypeMapper } from '../type-mapper.ts';

export class TsGenerator {
  private ast: HeaderAST;
  private packageName: string;

  constructor(ast: HeaderAST, packageName: string) {
    this.ast = ast;
    this.packageName = packageName;
  }

  generate(): string {
    const sections: string[] = [];

    // Import statement
    sections.push(this.generateImports());

    // Type definitions for structs
    if (this.ast.structs.length > 0) {
      sections.push(this.generateStructInterfaces());
    }

    // Enum constants
    if (this.ast.enums.length > 0) {
      sections.push(this.generateEnumExports());
    }

    // Function exports
    if (this.ast.functions?.length) {
      sections.push(this.generateFunctionExports());
    }

    // Struct helpers
    if (this.ast.structs.length > 0) {
      sections.push(this.generateStructHelpers());
    }

    return sections.join('\n\n');
  }

  private generateImports(): string {
    return `import nodeGypBuild from 'node-gyp-build';

// Load the native addon
const addon = nodeGypBuild(import.meta.dirname) as any;`;
  }

  private generateStructInterfaces(): string {
    const lines: string[] = ['// Struct type definitions'];
    
    // Deduplicate structs by name
    const seenStructNames = new Set<string>();

    for (const struct of this.ast.structs) {
      if (!struct.name) continue;
      
      // Skip if we've already processed this struct
      if (seenStructNames.has(struct.name)) {
        continue;
      }
      seenStructNames.add(struct.name);

      if (struct.documentation) {
        lines.push(`/**`);
        lines.push(` * ${struct.documentation}`);
        lines.push(` */`);
      }

      lines.push(`export interface ${struct.name} {`);
      lines.push(`  _ptr: unknown;`);
      lines.push(`  _type: '${struct.name}';`);
      lines.push(`}`);
      lines.push('');

      // Create init interface for struct creation
      lines.push(`export interface ${struct.name}Init {`);
      for (const field of struct.fields) {
        const tsType = TypeMapper.getTsType(field.type);
        lines.push(`  ${field.name}?: ${tsType};`);
      }
      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateEnumExports(): string {
    const lines: string[] = ['// Enum constants'];
    
    // Deduplicate enums by name
    const seenEnumNames = new Set<string>();

    for (const enumDef of this.ast.enums) {
      const enumName = enumDef.name || 'UnnamedEnum';
      
      // Skip if we've already processed this enum
      if (seenEnumNames.has(enumName)) {
        continue;
      }
      seenEnumNames.add(enumName);
      if (enumDef.documentation) {
        lines.push(`/**`);
        lines.push(` * ${enumDef.documentation}`);
        lines.push(` */`);
      }

      // Export as a const object instead of enum for better compatibility
      lines.push(`export const ${enumName} = {`);
      for (const constant of enumDef.constants) {
        if (constant.documentation) {
          lines.push(`  /** ${constant.documentation} */`);
        }
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        if (constant.value !== null) {
          lines.push(`  ${safeName}: ${constant.value},`);
        } else {
          lines.push(`  ${safeName}: addon.${constant.name}(),`);
        }
      }
      lines.push(`} as const;`);
      lines.push('');

      // Also export individual constants for compatibility
      for (const constant of enumDef.constants) {
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        if (constant.documentation) {
          lines.push(`/** ${constant.documentation} */`);
        }
        lines.push(`export const ${constant.name}: number = ${enumName}.${safeName};`);
      }
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateFunctionExports(): string {
    const lines: string[] = ['// Function exports'];

    for (const func of this.ast.functions || []) {
      if (func.documentation) {
        lines.push(`/**`);
        lines.push(` * ${func.documentation}`);
        
        // Add parameter documentation if available
        for (const param of func.params) {
          const paramName = param.name || 'arg';
          lines.push(` * @param ${paramName} - ${param.type.spelling}`);
        }
        
        if (!TypeMapper.isVoidType(func.return.spelling)) {
          lines.push(` * @returns ${func.return.spelling}`);
        }
        
        lines.push(` */`);
      }

      const paramList = func.params.map((param, i) => {
        const paramName = param.name || `arg${i}`;
        const tsType = TypeMapper.getTsType(param.type.spelling);
        return `${paramName}: ${tsType}`;
      }).join(', ');

      const returnType = TypeMapper.getTsType(func.return.spelling);

      lines.push(`export function ${func.name}(${paramList}): ${returnType} {`);
      
      const args = func.params.map((param, i) => param.name || `arg${i}`).join(', ');
      if (TypeMapper.isVoidType(func.return.spelling)) {
        lines.push(`  addon.${func.name}(${args});`);
      } else {
        lines.push(`  return addon.${func.name}(${args});`);
      }
      
      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateStructHelpers(): string {
    const lines: string[] = ['// Struct helper functions'];
    
    // Deduplicate structs by name
    const seenStructNames = new Set<string>();

    for (const struct of this.ast.structs) {
      if (!struct.name) continue;
      
      // Skip if we've already processed this struct
      if (seenStructNames.has(struct.name)) {
        continue;
      }
      seenStructNames.add(struct.name);

      const safeName = TypeMapper.sanitizeIdentifier(struct.name);

      // Create function
      if (struct.documentation) {
        lines.push(`/**`);
        lines.push(` * Create a new ${struct.name} instance`);
        lines.push(` * ${struct.documentation}`);
        lines.push(` */`);
      }
      lines.push(`export function create${struct.name}(init?: ${struct.name}Init): ${struct.name} {`);
      lines.push(`  return addon.create_${struct.name}(init);`);
      lines.push(`}`);
      lines.push('');

      // Field getters
      for (const field of struct.fields) {
        const fieldName = TypeMapper.sanitizeIdentifier(field.name);
        const tsType = TypeMapper.getTsType(field.type);
        
        lines.push(`/**`);
        lines.push(` * Get ${field.name} field from ${struct.name}`);
        lines.push(` */`);
        lines.push(`export function get${struct.name}_${fieldName}(struct: ${struct.name}): ${tsType} {`);
        lines.push(`  return addon.get_${struct.name}_field(struct, '${field.name}');`);
        lines.push(`}`);
        lines.push('');
      }

      // Convenience getter for all fields
      lines.push(`/**`);
      lines.push(` * Get all fields from ${struct.name} as an object`);
      lines.push(` */`);
      lines.push(`export function get${struct.name}Fields(struct: ${struct.name}): ${struct.name}Init {`);
      lines.push(`  return {`);
      for (const field of struct.fields) {
        lines.push(`    ${field.name}: addon.get_${struct.name}_field(struct, '${field.name}'),`);
      }
      lines.push(`  };`);
      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }
}