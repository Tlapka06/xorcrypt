#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "xorCrypt.h"

#define MAX_KEY_LENGTH 256
#define BUFFER_SIZE 512
#define DEBUG 0

#define debug_print(fmt, ...) \
do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

void fatalError(const char * text);
void help();

int main(int argc, char *argv[]) {
	char* key;
	FILE* inFile = stdin;
	FILE* outFile = stdout;
	enum { OPT_SILENT=1 } options = 0;
	
	// Allocate space for key
	key = malloc(MAX_KEY_LENGTH);
	if (key == NULL) {
		fatalError("Key allocation failed");
	}
	
	while (getopt(argc, argv, "hvsk:i:o:") != -1) {
		debug_print("DEBUG: option: -%c \"%s\" (%i:%i)\n", optopt, optarg, optind, opterr);
		switch (optopt) {
			case 'k':
				// Key
				if (strlcpy(key, optarg, MAX_KEY_LENGTH) >= MAX_KEY_LENGTH) {
					fatalError("Key is longer than MAX_KEY_LENGTH");
				}
				break;
			case 'i':
				// Input file
				if (inFile == stdin) {
					inFile = fopen(optarg, "rb");
					if (inFile == NULL) {
						fatalError("Failed to open input file");
					}
				}
				break;
			case 'o':
				// Output file
				if (outFile == stdout) {
					outFile = fopen(optarg, "wb");
					if (outFile == NULL) {
						fatalError("Failed to open output file");
					}
				}
				break;
			case 's':
				options = OPT_SILENT;
				break;
			case 'v':
				fprintf(stderr, "xorCrypt (c) Jakub Janík 2022 <mail.me@anidor.org>");
				exit(0);
			case 'h':
			case '?':
				help();
			default:
				help();
		}
	}
	
	debug_print("DEBUG: options: %i\n", options);
	
	if (!(options & OPT_SILENT)) {
		fprintf(stderr, "\t XOR crypt tool\n\t================\n");
	}
	
	// If user haven't specified key with argument, ask for it now
	while (key[0] == 0) {
		fprintf(stderr, "Key:");
		fgets(key, MAX_KEY_LENGTH, stdin);
		key[strlen(key)-1] = 0;
	}
	
	debug_print("DEBUG: key: \"%s\"\n", key);
	
	// XOR crypt
	int keyOffset = 0;
	int byte;
	while (1) {
		byte = fgetc(inFile);
		debug_print("DEBUG: byte: %x(%c)\n", byte, byte);
		if (byte == EOF) {
			if (feof(inFile)) {
				break;
			}
			fatalError("Failed to read input file");
		}
		
		byte = byte ^ key[keyOffset];
		debug_print("DEBUG: xored byte: 0x%x(%c) xored by 0x%x(%c)\n", byte, byte, key[keyOffset], key[keyOffset]);
		if (++keyOffset >= strlen(key)) {
			keyOffset = 0;
		}
		
		if (fputc(byte, outFile) == EOF) {
			fatalError("Failed to write output file");
		}
	}
	
	fclose(inFile);
	fclose(outFile);
	
	if (!(options & OPT_SILENT)) {
		fprintf(stderr, "\nCrypting done\n");
	}
}

void fatalError(const char * text) {
	perror(text);
	exit(1);
}

void help() {
	fprintf(stderr, "Usage:\n\txorCrypt [-s] [-k <key>] [-i <inputFile>] [-o <outputFile>]\n\txorCrypt -h/?\n\txorCrypt -v\n");
	exit(0);
}