import type { HeaderAST } from '../h-parser/types.ts';
import { TypeMapper } from '../../type-mapper.ts';

export class TsGenerator {
  private ast: HeaderAST;

  constructor(ast: HeaderAST, _packageName: string) {
    this.ast = ast;
  }

  generate(): string {
    const sections: string[] = [];

    // Create stable, sorted views for deterministic output
    const sortedEnums = [...this.ast.enums].sort((a, b) => (a.name || '').localeCompare(b.name || ''));
    const sortedStructs = [...this.ast.structs].sort((a, b) => (a.name || '').localeCompare(b.name || ''));
    const sortedFunctions = [...(this.ast.functions || [])].sort((a, b) => a.name.localeCompare(b.name));

    // Import statement
    sections.push(this.generateImports());

    // Type definitions for structs
    if (sortedStructs.length > 0) {
      sections.push(this.generateStructInterfaces(sortedStructs));
    }

    // Enum constants
    if (sortedEnums.length > 0) {
      sections.push(this.generateEnumExports(sortedEnums));
    }

    // Function exports
    if (sortedFunctions.length) {
      sections.push(this.generateFunctionExports(sortedFunctions));
    }

    // Struct helpers
    if (sortedStructs.length > 0) {
      sections.push(this.generateStructHelpers());
    }

    return sections.join('\n\n');
  }

  private generateImports(): string {
    return `import nodeGypBuild from 'node-gyp-build';

// Load the native addon
const addon = nodeGypBuild(import.meta.dirname) as any;`;
  }

  private generateStructInterfaces(sortedStructs: HeaderAST['structs']): string {
    const lines: string[] = ['// Struct type definitions'];

    // Deduplicate structs by name
    const seenStructNames = new Set<string>();

    for (const struct of sortedStructs) {
      if (!struct.name) continue;

      // Handle anonymous structs with better naming
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

      // Skip if we've already processed this struct
      if (seenStructNames.has(structName)) {
        continue;
      }
      seenStructNames.add(structName);

      if (struct.documentation) {
        lines.push(`/**`);
        lines.push(` * ${struct.documentation}`);
        lines.push(` */`);
      }

      lines.push(`export interface ${structName} {`);
      lines.push(`  _ptr: unknown;`);
      lines.push(`  _type: '${structName}';`);
      lines.push(`}`);
      lines.push('');

      // Create init interface for struct creation
      lines.push(`export interface ${structName}Init {`);
      for (const field of [...struct.fields].sort((a, b) => (a.name||'').localeCompare(b.name||''))) {
        // Skip fields with empty names (anonymous union members)
        if (!field.name || field.name.trim() === '') {
          continue;
        }
        const tsType = TypeMapper.getTsType(field.type);
        lines.push(`  ${field.name}?: ${tsType};`);
      }
      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateEnumExports(sortedEnums: HeaderAST['enums']): string {
    const lines: string[] = ['// Enum constants'];

    // Deduplicate enums by name
    const seenEnumNames = new Set<string>();

    for (const enumDef of sortedEnums) {
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
      for (const constant of [...enumDef.constants].sort((a, b) => a.name.localeCompare(b.name))) {
        if (constant.documentation) {
          lines.push(`  /** ${constant.documentation} */`);
        }
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        if (constant.value !== null) {
          lines.push(`  ${safeName}: ${constant.value},`);
        } else {
          // Access the enum value directly from addon (it's exported as a number now)
          lines.push(`  ${safeName}: addon.${constant.name},`);
        }
      }
      lines.push(`} as const;`);
      lines.push('');

      // Also export individual constants for compatibility
      for (const constant of [...enumDef.constants].sort((a, b) => a.name.localeCompare(b.name))) {
        const safeName = TypeMapper.sanitizeIdentifier(constant.name);
        if (constant.documentation) {
          lines.push(`/** ${constant.documentation} */`);
        }
        lines.push(
          `export const ${constant.name}: number = ${enumName}.${safeName};`,
        );
      }
      lines.push('');
    }

    return lines.join('\n');
  }

  private generateFunctionExports(sortedFunctions: NonNullable<HeaderAST['functions']>): string {
    const lines: string[] = ['// Function exports'];

    for (const func of sortedFunctions) {
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

      const paramList = func.params
        .map((param, i) => {
          const paramName = param.name || `arg${i}`;
          const tsType = TypeMapper.getTsType(param.type.spelling);
          return `${paramName}: ${tsType}`;
        })
        .join(', ');

      const returnType = TypeMapper.getTsType(func.return.spelling);

      lines.push(`export function ${func.name}(${paramList}): ${returnType} {`);

      const args = func.params
        .map((param, i) => param.name || `arg${i}`)
        .join(', ');
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

      // Handle anonymous structs with better naming
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

      // Skip if we've already processed this struct
      if (seenStructNames.has(structName)) {
        continue;
      }
      seenStructNames.add(structName);

      // Create function
      if (struct.documentation) {
        lines.push(`/**`);
        lines.push(` * Create a new ${structName} instance`);
        lines.push(` * ${struct.documentation}`);
        lines.push(` */`);
      }
      lines.push(
        `export function create${structName}(init?: ${structName}Init): ${structName} {`,
      );
      lines.push(`  return addon.create_${structName}(init);`);
      lines.push(`}`);
      lines.push('');

      // Field getters
      for (const field of [...struct.fields].sort((a, b) => (a.name||'').localeCompare(b.name||''))) {
        // Skip fields with empty names (anonymous union members)
        if (!field.name || field.name.trim() === '') {
          continue;
        }
        const fieldName = TypeMapper.sanitizeIdentifier(field.name);
        const tsType = TypeMapper.getTsType(field.type);

        lines.push(`/**`);
        lines.push(` * Get ${field.name} field from ${structName}`);
        lines.push(` */`);
        lines.push(
          `export function get${structName}_${fieldName}(struct: ${structName}): ${tsType} {`,
        );
        lines.push(
          `  return addon.get_${structName}_field(struct, '${field.name}');`,
        );
        lines.push(`}`);
        lines.push('');
      }

      // Convenience getter for all fields
      lines.push(`/**`);
      lines.push(` * Get all fields from ${structName} as an object`);
      lines.push(` */`);
      lines.push(
        `export function get${structName}Fields(struct: ${structName}): ${structName}Init {`,
      );
      lines.push(`  return {`);
      for (const field of struct.fields) {
        // Skip fields with empty names (anonymous union members)
        if (!field.name || field.name.trim() === '') {
          continue;
        }
        lines.push(
          `    ${field.name}: addon.get_${structName}_field(struct, '${field.name}'),`,
        );
      }
      lines.push(`  };`);
      lines.push(`}`);
      lines.push('');
    }

    return lines.join('\n');
  }
}
