#include <dbg.h>
#include "list_algos.h"

void swap_node_data(ListNode* node1, ListNode* node2)
{
	void* temp = node1->value;
	node1->value = node2->value;
	node2->value = temp;
}

int List_bubble_sort(List* list, List_compare compare)
{
    _CHECK_LIST(list);
    int i = 0;
    int swapped = 0;
    int is_sorted = 0;

    //List_debug(list);

    if(List_count(list) < 2) { //
        log_warn("given list is too small to sort.");
        return 0;
    }

    for(i = 0; i < List_count(list); i++) {
        swapped = 0;
        //debug("starting inner loop...");
        LIST_FOREACH(list, first, next, cur) {
            if(cur->next) {
                if(compare(cur->value, cur->next->value) > 0) {
                    //debug("swapping nodes %p and %p", cur, cur->next);
                    swap_node_data(cur, cur->next);
                    swapped = 1;
                }
            }
        }
        //debug("i = %i\tswapped = %i", i, swapped);
        if(swapped == 0) { // no swaps were made; the list is sorted
            is_sorted = 1;
            break;
        }
    }

    check(is_sorted == 1, "The list is still not sorted! This is an error.");

    //List_debug(list);

    return 0;
error:
    return 1;
}

List* merge_lists(List* list1, List* list2, List_compare compare)
{
	List* result = List_create();
	void* val = NULL;
    while(List_count(list1) > 0 || List_count(list2) > 0) {
        if(List_count(list1) > 0 && List_count(list2) > 0) {
            if(compare(list1->first->value, list2->first->value) <= 0) {
                val = List_fpop(list1);
            } else {
                val = List_fpop(list2);
            }
			List_push(result, val);
        } else if(List_count(list1) > 0) {
			val = List_fpop(list1);
			List_push(result, val);
        } else if(List_count(list2) > 0) {
			val = List_fpop(list2);
			List_push(result, val);
        } else sentinel("merge_lists shouldn't have got here.");
    }

    List_destroy(list1);
    List_destroy(list2);
    //List_debug(result);
	return result;
error:
    return NULL;
}

List* merge_sort(List* list, List_compare compare)
{
    debug("List_count of list %p is: %i", list, List_count(list));
    if(List_count(list) == 1) {
        return list;
    } else if(List_count(list) < 1) {
		sentinel("List_count of list %p is too small: %i",
								list, List_count(list));
	}
    //List_debug(list);
    List* left = List_create();
    List* right = List_create();
    int middle = List_count(list) / 2;
    int position = 0;
    LIST_FOREACH(list, first, next, cur) {
        if(position < middle) {
            List_push(left, cur->value);
        } else {
            List_push(right, cur->value);
        }
        position++;
    }
    check((List_count(left) + List_count(right)) == List_count(list),
                    "Incorrect split-list sizes");

    List* sort_left = merge_sort(left, compare);
    check(sort_left != NULL, "Error in left merge sort.");

    List* sort_right = merge_sort(right, compare);
    check(sort_right != NULL, "Error in right merge sort.");

	if(sort_left != left) List_destroy(left);
	if(sort_right != right) List_destroy(right);

    return merge_lists(sort_left, sort_right, compare);
error:
    return NULL;
}

List* List_merge_sort(List* list, List_compare compare)
{
    _CHECK_LIST(list);

    if(List_count(list) <= 1) {
        log_warn("List to merge sort is too small!");
        return list;
    }

    return merge_sort(list, compare);
error:
    return NULL;
}
