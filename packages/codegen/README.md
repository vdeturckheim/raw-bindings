# Codegen

Generate low-level Node.js bindings from C/C++/Objective-C/Objective-C++ header files.

## Features

- Direct 1:1 mapping of C/C++ functions to JavaScript
- Automatic NAPI wrapper generation
- TypeScript support (Node.js 24.3+)
- Support for:
  - Functions
  - Structs
  - Enums
  - Type definitions
  - Pointers and references
- No abstraction layers - raw API access

## Usage

### CLI Usage

```bash
# Generate bindings for a simple C library
node --experimental-strip-types packages/codegen/bin/cli.ts -n mylib -o ./mylib-binding /usr/include/mylib.h

# Generate bindings for libclang
node --experimental-strip-types packages/codegen/bin/cli.ts -n clang-bindings -l clang \
  -I /opt/homebrew/opt/llvm/include \
  -L /opt/homebrew/opt/llvm/lib \
  /opt/homebrew/opt/llvm/include/clang-c/Index.h

# Generate bindings for an Objective-C framework
node --experimental-strip-types packages/codegen/bin/cli.ts -n myframework -F CoreFoundation \
  --framework-path /System/Library/Frameworks \
  /path/to/header.h
```

## Generated Output

The codegen creates a complete Node.js addon package:

```
generated-binding/
├── src/
│   └── binding.cpp        # NAPI C++ wrapper
├── CMakeLists.txt         # Build configuration
├── package.json           # Node package
├── index.ts              # TypeScript bindings
└── test.ts               # Basic test file
```

## Building Generated Bindings

```bash
cd generated-binding
npm install
npm run build
npm test
```

## Type Mappings

| C/C++ Type | JavaScript Type |
|------------|-----------------|
| `int`, `float`, `double` | `number` |
| `bool` | `boolean` |
| `char*`, `const char*` | `string` |
| `long long`, `unsigned long long` | `bigint` |
| `void*` | `unknown` |
| structs | `{ _ptr: unknown, _type: string }` |
| enums | `number` |

## Example: Using Generated Bindings

```typescript
// After generating and building bindings for a C library
import * as mylib from './generated-binding';

// Call a C function
const result = mylib.someFunction(42, "hello");

// Use an enum constant
const status = mylib.StatusCode.SUCCESS;

// Create and use a struct
const myStruct = mylib.createMyStruct({
  field1: 123,
  field2: "test"
});

const field1Value = mylib.getMyStruct_field1(myStruct);
```

## Notes

- This package is a CLI, not a library. Programmatic APIs are internal (`packages/codegen/lib`).
- Node 24+ with `--experimental-strip-types` is required to run TypeScript directly.
