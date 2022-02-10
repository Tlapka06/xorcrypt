#define DEBUG 1

#include "std.h"
#include <readpassphrase.h>
#include "xorcrypt.h"
#include <limits.h>

//!!!!!!!!!!!!!!//
//	  WARNING	//
//!!!!!!!!!!!!!!//
// MAX_INTERACTIVE_KEY_LENGTH is used to allocate on stack
// it should not be greater than 1024
#define MAX_INTERACTIVE_KEY_LENGTH 512
#if MAX_INTERACTIVE_KEY_LENGTH > 1024
#error MAX_INTERACTIVE_KEY_LENGTH is used to allocate on stack, it should not be greater than 1024
#endif

#define DEFAULT_BLOCK_SIZE 4096

void help();

int main(int argc, char *argv[]) {
	char * key = NULL;
	char * filenameIn = NULL;
	char * filenameOut = NULL;
	FILE * fileIn;
	FILE * fileOut;
	char * buffer;
	enum {
		OPT_NONE = 0,
		OPT_FORCE_ENCRYPT = 1
	} options = OPT_NONE;
	
	// Arguments
	while (optind < argc) {
		getopt(argc, argv, "k:fo:vh");
		debug_print("DEBUG: option: -%c \"%s\" (optind = %i opterr = %i argc = %i &argv[optind-1] = %p)\n", optopt, optarg, optind, opterr, argc, argv[optind-1]);
		switch (optopt) {
			case 'k':
				key = argv[optind-1];
				break;
			case 'f':
				options |= OPT_FORCE_ENCRYPT;
				break;
			case 'o':
				filenameOut = argv[optind-1];
				break;
			case 'v':
				fprintf(stderr, "xorCrypt (c) Jakub Janík 2022 <mail.me@anidor.org>");
				exit(0);
			case 'h':
			case '?':
				help();
			default:
				// sets file
				if (filenameIn == NULL) {
					filenameIn = argv[optind++];
				} else {
					help();
				}
				break;
		}
	}
	
	
	debug_print("DEBUG: &key = %p &filenameIn = %p\n", key, filenameIn);
	// If user haven't specified key with argument, ask for it now
	if (key == NULL) {
		key = alloca(MAX_INTERACTIVE_KEY_LENGTH);
		do {
			if (readpassphrase("Key:", key, MAX_INTERACTIVE_KEY_LENGTH, RPP_REQUIRE_TTY) == NULL) {
				fatalError("ERROR: failed to read password");
			}
		} while (key[0] == 0);
	}
	debug_print("DEBUG: key = \"%s\" filenameIn = \"%s\" filenameOut = \"%s\"", key, filenameIn, filenameOut);
	
	//////////////////
	//	 XOR crypt	//
	//////////////////
	size_t readSize;
	enum {
		M_DECRYPT = 0,
		M_ENCRYPT = 1
	} mode;
	
	buffer = critmalloc(DEFAULT_BLOCK_SIZE);
	
	fileIn = fopen(filenameIn, "rb");
	if (fileIn == NULL) {
		fatalError("ERROR: cannot open input file");
	}
	
	if ((readSize = fread(buffer, 1, DEFAULT_BLOCK_SIZE, fileIn)) == 0) {
		fatalError("ERROR: file has no content");
	}
	
	mode = memcmp(((struct xorHeader *)buffer)->id, "XOR", 3) != 0 || options & OPT_FORCE_ENCRYPT;
	
	// End
	fprintf(stderr, "\nCrypting done\n");
}

void help() {
	fprintf(stderr, "xorCrypt\nUsage:\n\txorCrypt -h/?\tshow this help\n\txorCrypt -v\tprints version\n\txorCrypt [-s] [-f] [-a <algorithm>] [-k <key>] [-o filename] file\n\t\t-f\tforce encryption\n\t\t-a\tlet you choose algorithm (TBD)\n\t\t-k\tlet you enter key\n\t\t-o\tname of output file");
	exit(0);
}