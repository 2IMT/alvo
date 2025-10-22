# alvo

## Roadmap

- [ ] Lexer
  - [x] Working Lexer
  - [x] Error Recovery
  - [x] Error Reporting
  - [x] Token Source Locations
  - [ ] Tests
- [ ] Parser
  - [x] Working Parser
  - [ ] Error Recovery
  - [ ] Error Reporting
  - [ ] Node Source Locations
  - [ ] Tests
- [ ] Type-Checker
  - [ ] Working Basic Structures
    - [ ] Expressions
    - [ ] Statements
    - [ ] User-Defined Types
    - [ ] Functions
    - [ ] Methods
  - [ ] Working Generics
    - [ ] Monomorphisation
    - [ ] Infinite Cycle Detection
  - [ ] Error Recovery
  - [ ] Error Reporting
  - [ ] Tests
- [ ] Interpreter
  - [ ] Working Interpreter
  - [ ] Garbage Collection
  - [ ] Tests

## Building

### Dependencies

- C++ compiler with C++20 support
- CMake 3.27+

### Build Process

```bash
mkdir -p build # Create build directory
cd build
cmake .. # Configure CMake
cmake --build . # Build targets
```

### Build options

| Name                 | Description                       | Default value |
| -------------------- | --------------------------------- | ------------- |
| `ALVO_TEST`          | Build tests                       | `ON`          |
| `ALVO_SANITIZE`      | Enable sanitizers for main target | `OFF`         |
| `ALVO_TEST_SANITIZE` | Enable sanitizers for test target | `ON`          |

Options are passed on the stage of CMake configuration in the following format:

```bash
# Sets `ALVO_TEST` to `ON` and `ALVO_SANITIZE` to `OFF`
cmake -DALVO_TEST=ON -DALVO_SANITIZE=OFF ..
```
