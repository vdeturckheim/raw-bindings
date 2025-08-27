declare module 'node-gyp-build' {
  function nodeGypBuild(dirname: string): unknown;
  export = nodeGypBuild;
}
