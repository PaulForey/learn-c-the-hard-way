#ifndef _DArray_h
#define _DArray_h
#include <stdlib.h>
#include <assert.h>
#include <dbg.h>

typedef struct DArray {
    int end;
    int max;
    size_t element_size;
    size_t expand_rate;
    void **contents;
} DArray;
DArray* DArray_create(size_t element_size, size_t initial_max);
void DArray_destroy(DArray* array);
void DArray_clear(DArray* array);
int DArray_expand(DArray* array);
int DArray_contract(DArray* array);
int DArray_push(DArray* array, void* element);
void* DArray_pop(DArray* array);
void DArray_clear_destroy(DArray* array);
#define DArray_last(A) ((A)->contents[(A)->end-1])
#define DArray_first(A) ((A)->contents[0])
#define DArray_end(A) ((A)->end)
#define DArray_count(A) DArray_end(A)
#define DArray_max(A) ((A)->max)
#define DEFAULT_EXPAND_RATE 300
#define DArray_check(A) \
    check((A) != NULL, "DArray is NULL!"); \
    check((A)->contents != NULL, "DArray's content pointer is NULL!"); \
    check((A)->element_size > 0, "Element size is zero or smaller!"); \
    check((A)->end >= 0, "DArray's end is below 0!"); \
    check((A)->max >= 0, "DArray's max is below zero!"); \
    check((A)->max >= (A)->end, "DArray's max is smaller than it's end!");
static inline void DArray_set(DArray* array, int i, void* element)
{
    check(i < array->max, "DArray attempt to set past max");
    if(i > array->end) array->end = i;
    array->contents[i] = element;
error:
    return;
}
static inline void* DArray_get(DArray* array, int i)
{
    check(i < array->max, "DArray attempt to get past max");
    return array->contents[i];
error:
    return NULL;
}
static inline void* DArray_remove(DArray* array, int i)
{
    void *element = array->contents[i];
    array->contents[i] = NULL;
    return element;
}
static inline void* DArray_new(DArray* array)
{
    check(array->element_size > 0, "Can't use DArray_new on 0 size darrays.");
    return calloc(1, array->element_size);
error:
    return NULL;
}

void* DArray_fpop(DArray* array);
DArray* DArray_copy(DArray* array);
void DArray_print(DArray* array);
void DArray_debug(DArray* array);

#define DArray_free(E) free((E))
#endif // _DArray_H
