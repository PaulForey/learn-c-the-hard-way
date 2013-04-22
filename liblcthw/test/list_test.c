#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>
static List *list = NULL;
char* test1 = "test1 data";
char* test2 = "test2 data";
char* test3 = "test3 data";
char* test4 = "test4 data";
char* test5 = "test5 data";

char* test_create()
{
	list = List_create();
	mu_assert(list != NULL, "Failed to create list.");
	return NULL;
}

char* test_destroy()
{
	List_destroy(list);
	return NULL;
}

char* test_clear()
{
	List* new_list = List_create();
	int* one = calloc(1, sizeof(int));
	int* two = calloc(1, sizeof(int));
	int* three = calloc(1, sizeof(int));
	*(one) = 1;
	*(two) = 2;
	*(three) = 3;
	List_push(new_list, one);
	List_push(new_list, two);
	List_push(new_list, three);
	mu_assert(new_list->first->value == one, "Incorrect value before clear.");
	mu_assert(new_list->first->next->value == two, "Incorrect value before clear.");
	mu_assert(new_list->last->value == three, "Incorrect value before clear.");
	List_clear(new_list);
	List_destroy(new_list);
	return NULL;
}

char* test_clear_destroy()
{
	List* new_list = List_create();
	int* one = calloc(1, sizeof(int));
	int* two = calloc(1, sizeof(int));
	int* three = calloc(1, sizeof(int));
	*one = 1;
	*two = 2;
	*three = 3;
	List_push(new_list, one);
	List_push(new_list, two);
	List_push(new_list, three);
	mu_assert(*(int*)new_list->first->value == 1, "Incorrect value before clear.");
	mu_assert(*(int*)new_list->first->next->value == 2, "Incorrect value before clear.");
	mu_assert(*(int*)new_list->last->value == 3, "Incorrect value before clear.");
	List_clear_destroy(new_list);
	return NULL;
}

char* test_push_pop()
{
	List_push(list, test1);
	mu_assert(List_last(list) == test1, "Wrong last value.");
	List_push(list, test2);
	mu_assert(List_last(list) == test2, "Wrong last value.");
	List_push(list, test3);
	mu_assert(List_last(list) == test3, "Wrong last value.");
	mu_assert(List_count(list) == 3, "Wrong count on push.");

	char* val = List_pop(list);
	mu_assert(val == test3, "Wrong value on pop.");
	val = List_pop(list);
	mu_assert(val == test2, "Wrong value on pop.");
	val = List_pop(list);
	mu_assert(val == test1, "Wrong value on pop.");
	mu_assert(List_count(list) == 0, "Wrong count after pop.");
	return NULL;
}

char* test_fpush()
{
	List_fpush(list, test1);
	mu_assert(List_first(list) == test1, "Wrong first value.");
	List_fpush(list, test2);
	mu_assert(List_first(list) == test2, "Wrong first value.");
	List_fpush(list, test3);
	mu_assert(List_first(list) == test3, "Wrong first value.");
	mu_assert(List_count(list) == 3, "Wrong count on fpush.");
	return NULL;
}

char* test_remove()
{
	// We only test the middle case 'cause pop/fpop have already
	// tested the other cases.
	char* val = List_remove(list, list->first->next);
	mu_assert(val == test2, "Wrong removed element.");
	mu_assert(List_count(list) == 2, "Wrong count after remove.");
	mu_assert(List_first(list) == test3, "Wrong first after remove.");
	mu_assert(List_last(list) == test1, "Wrong last after remove.");
	return NULL;
}

char* test_fpop()
{
	mu_assert(List_count(list) != 0, "Wrong count before fpop.");
	char* val = List_fpop(list);
	mu_assert(val == test3, "Wrong value on fpop.");
	val = List_fpop(list);
	mu_assert(val == test1, "Wrong value on fpop.");
	mu_assert(List_count(list) == 0, "Wrong count after fpop.");
	return NULL;
}

char* test_copy()
{
	// Set up a list:
	List_push(list, test1);
	List_push(list, test2);
	List_push(list, test3);

	mu_assert(List_count(list) == 3, "Wrong count before copy");

	List* new_list = List_copy(list);

	mu_assert(List_first(new_list) == test1, "Wrong value after copy.");
	mu_assert(new_list->first->next->value == test2, "Wrong value after copy.");
	mu_assert(List_last(new_list) == test3, "Wrong value after copy.");

	mu_assert(List_first(list) == test1, "Wrong value after copy.");
	mu_assert(list->first->next->value == test2, "Wrong value after copy.");
	mu_assert(List_last(list) == test3, "Wrong value after copy.");

	List_destroy(new_list);

	return NULL;
}

char* test_join()
{
	List* list1 = List_create();
	List* list2 = List_create();

	char* this_data1 = "hello!";
	char* this_data2 = "names!";
	char* this_data3 = "games!";
	char* this_data4 = "gnarly";
	char* this_data5 = "what-up?";

	List_push(list1, this_data1);
	List_push(list1, this_data2);
	mu_assert(List_count(list1) == 2, "Incorrect count before join.");

	List_push(list2, this_data3);
	List_push(list2, this_data4);
	mu_assert(List_count(list2) == 2, "Incorrect count before join.");

	List_join(list1, list2);
	mu_assert(List_count(list1) == 4, "Incorrect count after join.");
	mu_assert(List_last(list1) == this_data4, "Incorrect value after join.");

	List_push(list2, this_data5);
	List_join(list1, list2);
	mu_assert(List_count(list1) == 7, "Incorrect count after second join.");
	mu_assert(List_last(list1) == this_data5, "Incorrect value after second join.");

	List_destroy(list1);
	List_destroy(list2);

	return NULL;
}

char* test_split()
{
	List* list1 = List_create();
	List_push(list1, test1);
	List_push(list1, test2);
	List_push(list1, test3);
	List_push(list1, test4);
	List_push(list1, test5);
	mu_assert(List_count(list1) == 5, "Incorrect count before split.");

	List* list2 = List_split(list1, list1->first->next->next);
	debug("list1 count: %i", List_count(list1));
	mu_assert(List_count(list1) == 2, "Incorrect count after split.");
	debug("list2 count: %i", List_count(list2));
	mu_assert(List_count(list2) == 3, "Incorrect count after split.");

	List_destroy(list1);
	List_destroy(list2);

	return NULL;
}

char* test_split_2()
{
	List* list1 = List_create();
	List_push(list1, test1);
	List_push(list1, test2);
	List_push(list1, test3);

	mu_assert(List_count(list1) == 3, "Incorrect count before split.");
	List* list2 = List_split(list1, list1->first);

	mu_assert(List_count(list1) == 0, "Incorrect count after split.");
	mu_assert(List_count(list2) == 3, "Incorrect count after split.");

	List_destroy(list1);
	List_destroy(list2);

	return NULL;
}

char* test_print()
{
	printf("List_print...\n");
	List_print(list);
	printf("List_print...success\n");
	return NULL;
}

char* all_tests()
{
	mu_suite_start();
	mu_run_test(test_create);
	mu_run_test(test_push_pop);
	mu_run_test(test_fpush);
	mu_run_test(test_print);
	mu_run_test(test_remove);
	mu_run_test(test_fpop);
	mu_run_test(test_copy);
	mu_run_test(test_destroy);
	mu_run_test(test_clear);
	mu_run_test(test_clear_destroy);
	mu_run_test(test_join);
	mu_run_test(test_split);
	mu_run_test(test_split_2);
	return NULL;
}

RUN_TESTS(all_tests);
