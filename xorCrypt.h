#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int xorFile(FILE* input, FILE* output, const char * key);
void xor(char * input, char * output, const char * key);