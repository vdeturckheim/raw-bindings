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

### As a Library

```typescript
import { generateBindings } from 'codegen';

await generateBindings('/path/to/header.h', {
  outputDir: './my-binding',
  packageName: 'my-binding',
  libraryName: 'mylib',
  includePaths: ['/usr/include'],
  libraryPaths: ['/usr/lib'],
  libraries: ['mylib']
});
```

### As a CLI Tool

```bash
# Generate bindings for a simple C library
codegen -n mylib -o ./mylib-binding /usr/include/mylib.h

# Generate bindings for libclang
codegen -n clang-bindings -l clang \
  -I /opt/homebrew/opt/llvm/include \
  -L /opt/homebrew/opt/llvm/lib \
  /opt/homebrew/opt/llvm/include/clang-c/Index.h

# Generate bindings for an Objective-C framework
codegen -n myframework -F CoreFoundation \
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

## Self-Hosting Capability

This codegen can generate bindings for libclang, which means it can theoretically be used to regenerate the node-clang bindings that h-parser depends on, creating a self-hosting toolchain.