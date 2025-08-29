import { mkdir, writeFile } from 'node:fs/promises';
import { dirname, join } from 'node:path';
import type { IRModule } from '../ir/schema.ts';
import { renderTsIndexFromIR, renderCppBindingFromIR, renderCMakeLists, renderPackageJson } from '../template-gen.ts';

export interface NewGenOptions {
  outputDir: string;
  packageName: string;
  packageVersion?: string;
  libraryName?: string;
  includes: string[]; // header files to include from src
}

export async function generateProjectFromIR(ir: IRModule, opts: NewGenOptions): Promise<void> {
  const out = opts.outputDir;
  const src = join(out, 'src');
  await mkdir(src, { recursive: true });

  const indexTs = renderTsIndexFromIR(ir);
  const bindingCpp = renderCppBindingFromIR(ir, opts.includes);
  const cmake = renderCMakeLists({ packageName: opts.packageName });
  const pkg = renderPackageJson({ packageName: opts.packageName, packageVersion: opts.packageVersion || '0.0.1', libraryName: opts.libraryName || opts.packageName });

  await writeFile(join(out, 'index.ts'), indexTs, 'utf-8');
  await writeFile(join(out, 'CMakeLists.txt'), cmake, 'utf-8');
  await writeFile(join(out, 'package.json'), pkg, 'utf-8');
  await writeFile(join(src, 'binding.cpp'), bindingCpp, 'utf-8');
}

