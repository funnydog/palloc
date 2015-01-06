#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "pool.h"

struct pool
{
	size_t count;		/* number of elements in the pool */
	size_t size;		/* size of each element */

	uint8_t *pool;		/* pool of allocated memory */
	uint8_t *free;		/* pointer to the first free element */

	struct pool *next;	/* pointer to next pool if this is filled */
};

struct pool *pool_new(size_t count, size_t size)
{
	assert(size >= sizeof(uint8_t));
	assert(count < SIZE_MAX);
	assert((count + 1) * size >= size);
	assert((count + 1) * size <= SIZE_MAX - sizeof(struct pool));

	size_t psize = sizeof(struct pool) + (count + 1) * size;
	struct pool *head = malloc(psize);
	if (!head)
		return NULL;

	/* set the pool pointer and align it to the
	 * size of elements
	 */
	head->pool = (uint8_t *)head + sizeof(*head);
	head->pool += size - ((size_t)head->pool % size);

	head->count = count;
	head->size = size;
	head->next = NULL;

	pool_clear(head);
	return head;
}

void pool_clear(struct pool *p)
{
	while (p) {
		uint8_t *from = p->pool + (p->count - 1) * p->size;
		*(uint8_t **)from = NULL;
		while (from > p->pool) {
			*(uint8_t **)(from - p->size) = from;
			from -= p->size;
		}
		p->free = from;

		p = p->next;
	}
}

void pool_free(struct pool *p)
{
	struct pool *tmp;
	while (p) {
		tmp = p;
		p = p->next;

		free(tmp);
	}
}

void *palloc(struct pool *p)
{
	assert(p);

	while (!p->free && p->next)
		p = p->next;

	if (!p->free) {
		p->next = pool_new(p->count, p->size);
		if (!p->next)
			return NULL;

		p = p->next;
	}

	void *rv = p->free;
	p->free = *(uint8_t **)p->free;

	return rv;
}

void *pzalloc(struct pool *p)
{
	void *rv = palloc(p);
	if (!rv)
		return NULL;

	return memset(rv, 0, p->size);
}

void pfree(struct pool *p, void *ptr)
{
	assert(p);
	if (ptr) {
		*(uint8_t **)ptr = p->free;
		p->free = ptr;
	}
}
