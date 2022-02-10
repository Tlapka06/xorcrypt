#include "xorcrypt.h"

XORCRYPT_FUNCTION(xor_crypt) {
	int offset = 0;
	for (int i = 0; i < size; i++) {
		output[i] = input[i] ^ key[offset];
		if (++offset >= strlen(key)) {
			offset = 0;
		}
	}
}