#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

void disassembleChunk(Chunk *chunk, const char *name);

void printValue(Value value);

int disassembleInstruction(Chunk *chunk, int offset);

#endif //CLOX_DEBUG_H
