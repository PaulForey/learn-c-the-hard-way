#include <lcthw/darray_algos.h>
#include <stdlib.h>

int DArray_qsort(DArray* array, DArray_compare cmp)
{
    DArray_check(array);
    qsort(array->contents, DArray_count(array), sizeof(void*), cmp);
    return 0;
error:
    return 1;
}

/*
int DArray_heapsort(DArray* array, DArray_compare cmp)
{
    return heapsort(array->contents, DArray_count(array), sizeof(void*), cmp);
}
*/

DArray* _merge_array(DArray* left, DArray* right, DArray_compare cmp)
{
    DArray* result = DArray_create(left->element_size,
            left->max > right->max ? left->max : right->max);
    DArray_check(left);
    DArray_check(right);
    DArray_check(result);
    void* element = NULL;
    while(left->end > 0 || right->end > 0) {
        int i = 0;
        DArray_debug(left);
        DArray_debug(right);
        debug("Left end: %d\tRight end: %d", left->end, right->end);
        if(left->end > 0 && right->end > 0) {
            check(left->contents[i] != NULL,
                        "left->contents[%i] is NULL!", i);
            check(right->contents[i] != NULL,
                        "right->contents[%i] is NULL!", i);
            if(cmp(&left->contents[i], &right->contents[i]) <= 0) {
                debug("Fire left!");
                element = DArray_fpop(left);
            } else {
                debug("Fire right!");
                element = DArray_fpop(right);
            }
        } else if(left->end > 0) {
            debug("Fire left?");
            element = DArray_fpop(left);
        } else if(right->end > 0) {
            debug("Fire right?");
            element = DArray_fpop(right);
        } else sentinel("_merge_array shouldn't get here.");
        check_mem(element);
        DArray_push(result, element);
        i++;
    }

    DArray_destroy(left);
    DArray_destroy(right);
    return result;
error:
    DArray_destroy(left);
    DArray_destroy(right);
    if(result) DArray_destroy(result);
    return NULL;
}

DArray* _merge_sort(DArray* array, DArray_compare cmp)
{
    DArray_check(array);
    if(array->end <= 1)
        return array;
    DArray* left = DArray_create(array->element_size, array->max);
    check_mem(left);
    DArray* right = DArray_create(array->element_size, array->max);
    check_mem(right);
    int middle = array->end/2;
    int i = 0;
    for(i = 0; i < array->end; i++) {
        if(i < middle) {
            DArray_push(left, DArray_get(array, i));
        } else {
            DArray_push(right, DArray_get(array, i));
        }
    }
    DArray* sort_left = _merge_sort(left, cmp);
    check(left != NULL, "Error in left merge sort");
    DArray* sort_right = _merge_sort(right, cmp);
    check(right != NULL, "Error in right merge sort");

    if(sort_left != left) DArray_destroy(left);
    if(sort_right != right) DArray_destroy(right);

    return _merge_array(sort_left, sort_right, cmp);
error:
    return NULL;
}

DArray* DArray_mergesort(DArray* array, DArray_compare cmp)
{
    DArray_check(array);
    DArray* result = _merge_sort(array, cmp);
    check(result != NULL, "merge sort failed.");
    DArray_check(result);
    return result;
error:
    return NULL;
}
