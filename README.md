# alvo

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
