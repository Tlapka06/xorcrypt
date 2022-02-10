#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define debug_print(fmt, ...) \
if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__);

void fatalError(const char * text);
void * critmalloc(size_t __size);