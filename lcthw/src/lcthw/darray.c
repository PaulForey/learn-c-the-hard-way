#include <lcthw/darray.h>
#include <assert.h>

DArray* DArray_create(size_t element_size, size_t initial_max)
{
	DArray* array = malloc(sizeof(DArray));
	check_mem(array);

	check(initial_max > 0, "You must set an initial max > 0");
	array->max = initial_max;

	array->contents = calloc(initial_max, sizeof(void*));
	check_mem(array->contents);
	array->end = 0;
	array->element_size = element_size;
	array->expand_rate = DEFAULT_EXPAND_RATE;
	return array;
error:
	if(array) free(array);
	return NULL;
}

void DArray_clear(DArray* array)
{
	int i = 0;
	if(array->element_size > 0) {
		for(i = 0; i < array->max; i++) {
			if(array->contents[i] != NULL) {
				free(array->contents[i]);
			}
		}
	}
}

static inline int DArray_resize(DArray* array, size_t new_size)
{
	check(new_size > 0, "new_size must be > 0");
	array->max = new_size;

	void* contents = realloc(array->contents, array->max * sizeof(void*));
	// Check contents and assume that realloc doesn't harm the originals.
	check_mem(contents);
	array->contents = contents;
	return 0;
error:
	return -1;
}

int DArray_expand(DArray* array)
{
	size_t old_max = array->max;
	check(DArray_resize(array, array->max + array->expand_rate) == 0,
			"Failed to expand array to new size: %d",
			array->max + (int)array->expand_rate);
	memset(array->contents + old_max, 0, array->expand_rate + 1);
	return 0;
error:
	return -1;
}

int DArray_contract(DArray* array)
{
	int new_size = array->end < (int)array->expand_rate ?
								(int)array->expand_rate :
								array->end;
	return DArray_resize(array, new_size + 1);
}

void DArray_destroy(DArray* array)
{
	if(array) {
		if(array->contents) free(array->contents);
		free(array);
	}
}

void DArray_clear_destroy(DArray* array)
{
	DArray_clear(array);
	DArray_destroy(array);
}

int DArray_push(DArray* array, void* element)
{
	array->contents[array->end] = element;
	array->end++;
	if(DArray_end(array) >= DArray_max(array)) {
		return DArray_expand(array);
	} else {
		return 0;
	}
}

void* DArray_pop(DArray* array)
{
	check(array->end - 1 >= 0, "Attempted to pop from empty array.");
	void* element = DArray_remove(array, array->end - 1);
	array->end--;
	if(DArray_end(array) > (int)array->expand_rate
			&& DArray_end(array) % array->expand_rate) {
		DArray_contract(array);
	}
	return element;
error:
	return NULL;
}
