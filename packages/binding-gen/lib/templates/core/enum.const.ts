import type { Template } from '../../templates/schema.ts';

// TS-only enum/flags export. No native code emitted.
const tpl: Template = {
  id: 'core.enum.const',
  kind: 'enum',
  description: 'Emit a TS const object and type alias for C enums/flags.',
  cxx: {
    headers: ['#include <napi.h>'],
    helpers: [],
    declarations: [],
    definitions: [],
    moduleInit: []
  },
  ts: {
    parts: [
      `export const {{name}} = Object.freeze({
{{#each entries}}  {{this.name}}: {{this.value}},
{{/each}}
}) as const;
export type {{name}} = typeof {{name}}[keyof typeof {{name}}];
`
    ]
  }
};

export default tpl;
