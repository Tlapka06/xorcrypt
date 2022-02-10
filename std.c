#include "std.h"

void fatalError(const char * text) {
	perror(text);
	exit(1);
}

void * critmalloc(size_t __size) {
	void * ret;
	if ((ret = malloc(__size)) == NULL) {
		fatalError("ERROR: insufficient memory");
	}
	return ret;
}