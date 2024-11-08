# RISC-V

## Overview

This project consists of three scripts, namely [`assembler_risv.cpp`](#supported-instructions), [`pipeline_riscv.cpp`](#getting-started), and [`nonPipeline_risv.cpp`](#getting-started). The Assembler Script converts RISC-V instructions to Machine Language, while the Pipeline or Non-Pipeline script processes the Machine Language produced by the assembler to generate output. Additionally, there is a [`cache.cpp`](#cache) script demonstrating a set-associative cache implementation.

## Table of Contents
- [Overview](#overview)
- [Supported Instructions](#supported-instructions)
- [Getting Started](#getting-started)
- [Example Input](#example-input)
- [Example Main Function](#example-main-function)
- [Other Features](#other-features)
- [Cache](#cache)
- [Contributing](#contributing)

## Supported Instructions

The assembler supports the following instructions, grouped by their types:

### R-Type Instructions
- `add`  - Addition
- `sub`  - Subtraction
- `and`  - And
- `or`   - Or
- `xor`  - Xor
- `sll`  - Shift Left Logical
- `srl`  - Shift Right Logical
- `mul`  - Multiplication

### I-Type Instructions
- `addi`  - Addition Immediate
- `xori`  - Xor Immediate
- `ori`   - Or Immediate
- `andi`  - And Immediate
- `slli`  - Shift Left Logical Immediate
- `srli`  - Shift Right Logical Immediate

### L-Type Instructions (Load)
- `lb`   - Load Byte
- `lh`   - Load Half
- `lw`   - Load Word
- `lbu`  - Load Byte (U)
- `lhu`  - Load Half (U)

### S-Type Instructions (Store)
- `sb`  - Store Byte
- `sh`  - Store Half
- `sw`  - Store Word 

### J-Type Instruction
- `jal`  - Jump and Link

### B-Type Instructions (Branch)
- `beq`   - Branch ==
- `bne`   - Branch !=
- `blt`   - Branch <
- `bge`   - Branch >=
- `bltu`  - Branch < (U)
- `bgeu`  - Branch >= (U)

### U-Type Instruction
- `lui`  - Load Upper Immediate

## Getting Started

### Prerequisites
- C++ Compiler (e.g., g++, clang++)

### Compilation
- First, create an input file inside the testing folder with a ".s" extension.
- Specify the input file name in the main function of `pipeline_riscv.cpp` or `nonPipeline_risv.cpp`.
- Use the same file name with a ".o" extension; this file will be created automatically. 
- Compile the chosen script, either pipeline or non-pipeline:
  ```bash
  g++ -o pipeline_riscv pipeline_riscv.cpp
  ```
- The output can be verified by printing the register and memory locations as needed with `cout` statements.

## Example Input

#### Rules to Write Input File
- 32 registers are available (0-31).
- Access registers with `x` followed by the register number, e.g., `x4` for the 4th register.
- Use lowercase for instructions, with spaces to separate parts, and commas after each register. 
- Labels are case-insensitive; lowercase is recommended.
- Negative immediates are NOT SUPPORTED; use `sub` for negative values.

```assembly
lw x18, 0(x0)

l:
    andi x28, x18, 1
    add x19, x19, x28
    srli x18, x18, 1
    beq x18, x0, r
    jal x5, l
r:
    sw x19, 1(x0)
```

## Example Main Function

The main function assumes an input file named `setBit`. Input values are written to memory before invoking the assembler. Final values are stored in memory locations and registers, printed using `cout` after pipeline invocation.

```cpp
int main(){
    string s1 = "setBit.s";
    string s2 = "setBit.o";

    myMemory[0] = 5;

    assembler a(s1);
    pipeline p(s2);

    cout << myMemory[1] << endl;
}
```

## Other Features
- **Helper Folder**: Contains scripts for testing, such as `outputChecker.cpp` (checks assembler output against manually written binary code) and `try.cpp` (for testing instruction format checks).
- **Offset Calculation**: Branch and Jump types have a unique offset calculation compared to the [standard](https://luplab.gitlab.io/rvcodecjs/).
- **Custom Implementations**: Variants of `sub` and `mul` make the implementation simpler.
- **Testing Folder**: Contains pre-existing assembly scripts for reference and further testing.

## Cache

- This project includes a set-associative cache.
- Users can implement custom replacement algorithms (e.g., LRU, LFU, FIFO) in the `getNewLocation` function.
- The `handleRandomRequests2` function generates normal-distributed random numbers for hit and miss rate calculations.

## Contributing

If you'd like to contribute to this project, feel free to submit a pull request or open an issue.
