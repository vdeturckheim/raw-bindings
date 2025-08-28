import assert from 'node:assert/strict';
import { describe, it } from 'node:test';
import * as binding from './index.ts';

describe('node-clang-raw bindings', () => {
  it('should load the module', () => {
    assert.ok(binding);
  });

  // Add more specific tests here based on the exported functions
  // Example:
  // it('should call a function', () => {
  //   const result = binding.someFunction();
  //   assert.ok(result);
  // });
});