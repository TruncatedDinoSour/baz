#ifndef _MEM_H
#define _MEM_H
#ifndef MEM_NO_STDDEF
#include <stddef.h>
#endif

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS 32
#endif

#ifdef MEM_CUSTOM
static void *mem_alloc(size_t);
static void mem_free(void *);
static void *mem_realloc(void *, size_t);
#endif /* MEM_CUSTOM */

#ifdef MEM_IMPL
#ifdef MEM_CUSTOM
static void *mem_alloc(size_t size) {
    size_t *p;
    size += sizeof(size_t);

    p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,
             -1, 0);

    *p = size;
    return p + sizeof(size_t);
}

static void mem_free(void *ptr) {
    size_t *p = (size_t *)ptr - sizeof(size_t);
    munmap(p, *p);
}

static void *mem_realloc(void *ptr, size_t size) {
    void *new_ptr;
    size_t *p;
    size_t old_size;

    if (ptr == NULL)
        return mem_alloc(size);

    p        = (size_t *)ptr - sizeof(size_t);
    old_size = *p;

    if (size > old_size) {
        new_ptr = mem_alloc(size);
        memcpy(new_ptr, ptr, old_size);
        mem_free(ptr);
    } else {
        new_ptr = ptr;
        *p      = size;
    }

    return new_ptr;
}
#else
#define mem_alloc(size)        malloc(size)
#define mem_free(ptr)          free(ptr)
#define mem_realloc(ptr, size) realloc(ptr, size)
#endif /* MEM_CUSTOM */
#endif /* MEM_IMPL */
#endif /* _MEM_H */
