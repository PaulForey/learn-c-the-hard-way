#include "minunit.h"
#include <lcthw/list_algos.h>
#include <lcthw/darray.h>
#include <assert.h>
#include <string.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
#define NUM_VALUES 5

#define NUM_ITERS 4000

typedef List* (*List_sort_method)(List* list, List_compare cmp);
//typedef DArray* (*DArray_sort_method)(DArray* array, DArray_compare cmp);

List* create_words()
{
    int i = 0;
    List* words = List_create();
    for(i = 0; i < NUM_VALUES; i++) {
        List_push(words, values[i]);
    }

    return words;
}

int List_timed_sort(List_sort_method list_cmp, char* sort_name, int iterations)
{
	List** list_array = calloc(iterations, sizeof(List*));
	List** list_res_array = calloc(iterations, sizeof(List*));
	int i = 0;

	for(i = 0; i < iterations; i++) {
		list_array[i] = create_words();
	}

	//time_t time_one = time(NULL);
	//mu_assert(time_one != ((time_t)-1), "Failed to read time.");
	struct timespec time_one;
	struct timespec time_two;
	int rc = clock_gettime(CLOCK_MONOTONIC, &time_one);
	check(rc == 0, "Failed to get clock time one.");

	for(i = 0; i < iterations; i++) {
		list_res_array[i] = list_cmp(list_array[i], (List_compare)strcmp);
		check(list_res_array[i] != NULL, "Bubble sort failed on list %p; index %i.",
												list_array[i], i);
	}

	//time_t time_two = time(NULL);
	//mu_assert(time_two != ((time_t)-1), "Failed to read time.");
	rc = clock_gettime(CLOCK_MONOTONIC, &time_two);
	check(rc == 0, "Failed to get clock time two.");

	for(i = 0; i < iterations; i++) {
		List_destroy(list_res_array[i]);
	}
	free(list_res_array);

	int res_stime = difftime(time_two.tv_sec, time_one.tv_sec);
	long double res_ntime = time_two.tv_nsec - time_one.tv_nsec;
	long double res_time = (res_ntime/1000000000) + res_stime;

	/*
	printf("Difference in seconds: %.0f\n", res_stime);
	printf("Difference in nanoseconds: %.0Lf\n", res_ntime);
	printf("Difference in accurate seconds: %.10f\n", res_time+res_stime);
	printf("Nanosecond one: %li\n", time_one.tv_nsec);
	printf("Nanosecond two: %li\n", time_two.tv_nsec);
	*/

	printf("Time to complete %i %ss:\t%.10Lf\n", iterations,
										sort_name, res_time);
	printf("Extrapolated time for one %s:\t%.10Lf\n", sort_name,
											res_time/iterations);

	if(list_array) {
		for(i = 0; i < iterations; i++) {
			List_destroy(list_array[i]);
		}
		free(list_array);
	}

	return 0;
error:
	if(list_array) {
		for(i = 0; i < iterations; i++) {
			List_destroy(list_array[i]);
		}
		free(list_array);
	}
	return -1;
}

char* list_bubble_sort_time()
{
	int rc = List_timed_sort((List_sort_method)List_bubble_sort,
									"list bubble sort", NUM_ITERS);
	mu_assert(rc == 0, "Error in timed list bubble sort.");

	return NULL;
}

char* list_merge_sort_time()
{
	int rc = List_timed_sort((List_sort_method)List_merge_sort,
									"list merge sort", NUM_ITERS);
	mu_assert(rc == 0, "Error in timed list merge sort.");

	return NULL;
}

/*
char* darray_bubble_sort_time()
{
	int rc = DArray_timed_sort((DArray_sort_method)DArray_bubble_sort,
									"darray bubble sort", NUM_ITERS);
	mu_assert(rc == 0, "Error in timed darray bubble sort.");

	return NULL;
}

char* darray_merge_sort_time()
{
	int rc = DArray_timed_sort((DArray_sort_method)DArray_merge_sort,
									"darray merge sort", NUM_ITERS);
	mu_assert(rc == 0, "Error in timed darray bubble sort.");

	return NULL;
}
*/

char* all_tests()
{
	mu_suite_start();

	mu_run_test(list_bubble_sort_time);
	mu_run_test(list_merge_sort_time);

	return NULL;
}

RUN_TESTS(all_tests);
