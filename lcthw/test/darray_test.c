#include "minunit.h"
#include <lcthw/darray.h>
static DArray* array = NULL;
static int* val1 = NULL;
static int* val2 = NULL;

char* test_create()
{
	array = DArray_create(sizeof(int), 100);
	mu_assert(array != NULL, "DArray_create failed.");
	mu_assert(array->contents != NULL,  "Contents are wrong in DArray.");
	mu_assert(array->end == 0, "End isn't in the right spot.");
	mu_assert(array->element_size == sizeof(int), "element size is wrong.");
	mu_assert(array->max == 100, "wrong max length on initial size.");
	return NULL;
}

char* test_destroy()
{
	DArray_destroy(array);
	return NULL;
}

char* test_new()
{
	val1 = DArray_new(array);
	mu_assert(val1 != NULL, "failed to make a new element.");
	val2 = DArray_new(array);
	mu_assert(val2 != NULL, "failed to make a new element.");
	return NULL;
}

char* test_set()
{
	DArray_set(array, 0, val1);
	DArray_set(array, 1, val2);
	return NULL;
}

char* test_get()
{
	mu_assert(DArray_get(array, 0) == val1, "Wrong first value.");
	mu_assert(DArray_get(array, 1) == val2, "Wrong second value.");
	return NULL;
}

