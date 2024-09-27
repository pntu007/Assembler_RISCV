# RISC-V to Binary Converter

## Overview

This project is a RISC-V instruction to binary converter written in C++. It translates a set of supported RISC-V assembly instructions into their corresponding binary machine code.

## Supported Instructions

The converter supports the following instructions, grouped by their types:

### R-Type Instructions
- `add`
- `sub`
- `and`
- `or`
- `xor`
- `sll`
- `srl`
- `slt`

### I-Type Instructions
- `addi`
- `xori`
- `ori`
- `andi`
- `slli`
- `srli`

### L-Type Instructions (Load)
- `lb`
- `lh`
- `lw`
- `lbu`
- `lhu`

### S-Type Instructions (Store)
- `sb`
- `sh`
- `sw`

### J-Type Instruction
- `jal`

### B-Type Instructions (Branch)
- `beq`
- `bne`
- `blt`
- `bge`
- `bltu`
- `bgeu`

### U-Type Instruction
- `lui`

## Features
- Converts RISC-V assembly instructions into their binary format.
- Supports multiple instruction types: R, I, L, S, J, B, and U.

## Getting Started

### Prerequisites
- C++ Compiler (e.g., g++, clang++)

### Compilation
- First create a input file inside the testing folder
- Write the input file name in the main function of assembler_riscv.cpp
- Then compile the assembler_riscv.cpp
- To compile, you can run the following command:

```bash
g++ -o assembler_riscv assembler_riscv.cpp
```

Once compiled, the output file will be generated inside the testing folder. <br>
There are RISCV_CARD and few other cpp script inside the helper folder. <br>
RISCV_CARD contains the all information regarding the instructions opcode, function3 value, function7 value <br>
Most of the values used inside the assembler_rissv.cpp is same as it is given in the card. <br>

### Example Input

```assembly
addi x18, x18,516
l:
andi x28,x18,1
add x19, x19, x28
srli x18,x18,1
beq x18,x0, r
jal x5, l
r:
```

### Example Output

The converter will output the binary encoding of each instruction in new file inside the testing folder.
```binary
00100000010010010000100100010011
00000000000110010111111000010011
00000001110010011000100110110011
00000000000110010101100100010011
00000000000010010000000101100011
11111111110111111111001011101111
```

## Contributing

If you'd like to contribute to this project, feel free to submit a pull request or open an issue.
