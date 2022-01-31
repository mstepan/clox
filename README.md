# CLox
CLox is a high level language that uses byte-code interpretation technique for execution. VM is stack based. All code written in good old C.

## How to build and run
To create `clox` executable just run:
```
make 
```
To run compiled application, just execute:
```
make run
```

## High level architecture

### Lexer
Creates tokens from source code stream.

### Compiler.
Uses Lexer stream of tokens to generate bytecode representation. Compiler uses `Pratt's` top-down parsing technique.

### VM
Stack based VM that executes generated bytecode.

#### Supported VM instructions

| Opcode                         | Description                                                                                               | Total size, bytes |
|--------------------------------|-----------------------------------------------------------------------------------------------------------|-------------------|
| OP_CONST <1 byte index>        | Load single value from constant pool to VM stack  using 1 byte index.                                     | 2                 |
| OP_CONST_LONG <3 bytes index>  | Load constant stored in 3 bytes to VM stack. Bytes store in little-endian order (least-significant first) | 4                 |
| OP_RETURN                      |                                                                                                           | 1                 |