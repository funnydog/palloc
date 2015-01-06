#ifndef POOL_H
#define POOL_H

struct pool *pool_new(size_t num, size_t size);
void pool_clear(struct pool *p);
void pool_free(struct pool *p);

void *palloc(struct pool *p);
void *pzalloc(struct pool *p);
void  pfree(struct pool *p, void *ptr);

#endif /* POOL_H */
