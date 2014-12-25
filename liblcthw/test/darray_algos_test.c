#include "minunit.h"
#include <lcthw/darray_algos.h>

int testcmp(char **a, char **b)
{
    return strcmp(*a, *b);
}

DArray* create_words()
{
    DArray* result = DArray_create(sizeof(char*), 5);
    check(result != NULL, "Could not create words darray!");
    char* words[] = {"asdfasfd", "werwar", "13234", "asdfasfd", "oioj"};
    int i = 0;
    for(i = 0; i < 5; i++) {
        DArray_push(result, words[i]);
    }
    return result;
error:
    return NULL;
}

int is_sorted(DArray* array)
{
    int i = 0;
    for(i = 0; i < DArray_count(array)-1; i++) {
        if(strcmp(DArray_get(array, i), DArray_get(array, i+1)) > 0 ) {
            return 0;
        }
    }
    return 1;
}

char* test_qsort()
{
    DArray* words = create_words();
    DArray_check(words);
    //DArray_print(words);
    mu_assert(words != NULL, "Failed to create words.");
    mu_assert(!is_sorted(words), "Words should not start sorted.");
    debug("--- Testing qsort sorting algorithm");
    int rc = DArray_qsort(words, (DArray_compare)testcmp);
    DArray_check(words);
    //DArray_print(words);
    mu_assert(rc == 0, "qsort failed");
    mu_assert(is_sorted(words), "Words not sorted.");
    DArray_destroy(words);
    return NULL;
error:
    return "Other error detected";
}

/*
char* test_heapsort()
{
    return run_sort_test(DArray_heapsort, "heapsort");
}
*/

char* test_mergesort()
{
    DArray* words = create_words();
    DArray_check(words);
    //DArray_print(words);
    mu_assert(words != NULL, "Failed to create words.");
    mu_assert(!is_sorted(words), "Words should not start sorted.");
    debug("--- Testing mergesort sorting algorithm");
    DArray* result = DArray_mergesort(words, (DArray_compare)testcmp);
    mu_assert(result != NULL, "sort failed");
    DArray_check(result);
    //DArray_print(result);
    mu_assert(is_sorted(result), "Words not sorted.");
    DArray_destroy(words);
    DArray_destroy(result);
    return NULL;
error:
    return "Other error detected";
}

char* all_tests()
{
    mu_suite_start();
    mu_run_test(test_qsort);
    //mu_run_test(test_heapsort);
    mu_run_test(test_mergesort);
    return NULL;
}

RUN_TESTS(all_tests);
