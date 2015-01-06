#include <stdio.h>
#include <stdlib.h>

#include "pool.h"

int main(int argc, char *argv[])
{
	struct pool *p = pool_new(16, 256);
	if (!p) {
		fprintf(stderr, "EOM\n");
		return EXIT_FAILURE;
	}

	/* allocate objects into an array */
	void *a[32];
	for (size_t i = 0; i < 32; i++) {
		a[i] = palloc(p);
		fprintf(stderr, "%zd\talloc\t%p\n", i, a[i]);
	}

	/* shuffle the array */
	for (size_t i = 0; i < 32-1; i++) {
		size_t j = i + rand() / (RAND_MAX / (32 - i) + 1);
		void *tmp = a[i];
		a[i] = a[j];
		a[j] = tmp;
	}

	/* free the array */
	for (size_t i = 0; i < 32; i++) {
		fprintf(stderr, "%zd\tfree\t%p\n", i, a[i]);
		pfree(p, a[i]);
	}

	pool_free(p);
	return EXIT_SUCCESS;
}
