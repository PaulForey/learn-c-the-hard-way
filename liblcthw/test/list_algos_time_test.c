#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <string.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};
#define NUM_VALUES 5

typedef List* (*List_sort_method)(List* list, List_compare cmp);

List* create_words()
{
    int i = 0;
    List* words = List_create();
    for(i = 0; i < NUM_VALUES; i++) {
        List_push(words, values[i]);
    }

    return words;
}

int timed_sort(List_sort_method list_cmp, int iterations)
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

	double res_stime = difftime(time_two.tv_sec, time_one.tv_sec);
	long double res_ntime = time_two.tv_nsec - time_one.tv_nsec;
	float res_time = res_ntime/1000000000;
	printf("Difference in seconds: %.0f\n", res_stime);
	printf("Difference in nanoseconds: %.0Lf\n", res_ntime);
	printf("Difference in accurate seconds: %.10f\n", res_time+res_stime);
	//printf("Nanosecond one: %li\n", time_one.tv_nsec);
	//printf("Nanosecond two: %li\n", time_two.tv_nsec);

	printf("Time to complete %i bubble sorts: %.10f\n", iterations,
															res_time+res_stime);
	printf("Extrapolated time for one bubble sort: %.10f\n",
								(res_time+res_stime)/iterations);

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

char* bubble_sort_time()
{
	int rc = timed_sort((List_sort_method)List_bubble_sort, 40000);
	mu_assert(rc == 0, "Error in timed bubble sort.");

	return NULL;
}

char* merge_sort_time()
{
	int rc = timed_sort((List_sort_method)List_merge_sort, 40000);
	mu_assert(rc == 0, "Error in timed bubble sort.");

	return NULL;
}

char* all_tests()
{
	mu_suite_start();

	mu_run_test(bubble_sort_time);
	mu_run_test(merge_sort_time);

	return NULL;
}

RUN_TESTS(all_tests);
