import { readFileSync, readdirSync } from 'node:fs';
import { join } from 'node:path';
import Handlebars from 'handlebars';

export interface TemplatePack { baseDir: string }

export class HbsEngine {
  private hbs: any = (Handlebars as any).create();
  private compiled: Map<string, any> = new Map();

  constructor(_pack: TemplatePack) {}

  registerHelper(name: string, fn: any) {
    this.hbs.registerHelper(name, fn);
  }

  registerPartialsFrom(dir: string) {
    const entries = readdirSync(dir, { withFileTypes: true });
    for (const e of entries) {
      if (!e.isFile()) continue;
      if (!e.name.endsWith('.hbs')) continue;
      const name = e.name.replace(/\.hbs$/, '');
      const src = readFileSync(join(dir, e.name), 'utf-8');
      this.hbs.registerPartial(name, src);
    }
  }

  compile(templatePath: string): any {
    if (this.compiled.has(templatePath)) return this.compiled.get(templatePath)!;
    const src = readFileSync(templatePath, 'utf-8');
    const tpl = this.hbs.compile(src, { noEscape: false });
    this.compiled.set(templatePath, tpl);
    return tpl;
  }

  render(templatePath: string, context: unknown): string {
    const tpl = this.compile(templatePath);
    return tpl(context);
  }
}
