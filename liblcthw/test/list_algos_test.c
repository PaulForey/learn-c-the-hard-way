#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
#define NUM_VALUES 5

List* create_words()
{
    int i = 0;
    List* words = List_create();
    for(i = 0; i < NUM_VALUES; i++) {
        List_push(words, values[i]);
    }

    return words;
}

int is_sorted(List* words) {
    LIST_FOREACH(words, first, next, cur) {
        if(cur->next && strcmp(cur->value, cur->next->value) > 0) {
            debug("%s %s", (char*)cur->value, (char*)cur->next->value);
            return 0;
        }
    }

    return 1;
}

char* test_bubble_sort()
{
    List* words = create_words();

    //printf("Testing bubble sort...\n");
    //printf("List before sort:\n");
    //List_print(words);

    // Should work on a list that needs sorting:
    List* result = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(result != NULL, "Bubble sort failed");
    mu_assert(is_sorted(result), "Words are not sorted after bubble sort.");

    //printf("List after sort:\n");
    //List_print(words);

    // Should work on an already sorted list:
    List* result2 = List_bubble_sort(result, (List_compare)strcmp);
    mu_assert(result2 != NULL, "Bubble sort of already sorted list failed.");
    mu_assert(is_sorted(result2), "Words no longer sorted after sort of already sorted list.");

    List_destroy(words);
    List_destroy(result);
    List_destroy(result2);

    // Should work on an empty list:
    words = List_create();
    result = List_bubble_sort(words, (List_compare)strcmp);
    mu_assert(result != NULL, "Bubble sort failed on empty list.");
    mu_assert(is_sorted(result), "Words should be sorted if empty.");

    List_destroy(words);
    List_destroy(result);

    return NULL;
}

char* test_merge_sort()
{
    List* words = create_words();

    //printf("Testing merge sort...\n");
    //printf("List before sort:\n");
    //List_print(words);

    // Should work on a list that needs sorting:
    List* res = List_merge_sort(words, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

    //printf("List after sort:\n");
    //List_print(res);

    List* res2 = List_merge_sort(res, (List_compare)strcmp);
    mu_assert(is_sorted(res), "Should still be sorted after merge sort."); 
    mu_assert(is_sorted(res2), "Should still be sorted after merge sort."); 

    List_destroy(res2);
    List_destroy(res);
    List_destroy(words);
    return NULL;
}

char* all_tests()
{
    mu_suite_start();

    mu_run_test(test_bubble_sort);
    mu_run_test(test_merge_sort);

    return NULL;
}

RUN_TESTS(all_tests);
