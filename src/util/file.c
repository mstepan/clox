//
// Created by Maksym Stepanenko on 20.02.2022.
//
#include "common.h"
#include "file.h"

/**
 * Read the whole file content into dynamic char array.
 */
char *readFile(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Can't open file: %s\n", path);
        exit(74);
    }

    // calculate file size by moving cursor till the end
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    // rewind cursor back
    rewind(file);

    char *buf = (char *) malloc(fileSize + 1);
    if (buf == NULL) {
        fprintf(stderr, "Can't allocate enough memory to read file: %s\n", path);
        exit(74);
    }

    size_t readBytes = fread(buf, sizeof(char), fileSize, file);
    if (readBytes < fileSize) {
        fprintf(stderr, "Can't fully read file: %s\n", path);
        exit(74);
    }

    buf[readBytes] = '\0';

    fclose(file);
    return buf;
}
