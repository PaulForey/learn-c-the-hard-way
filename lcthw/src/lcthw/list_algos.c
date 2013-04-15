#include <dbg.h>
#include "list_algos.h"

void swap_nodes(List* list, ListNode* node1, ListNode* node2)
{
    if(!node1->prev && !node2->next) { // we have a list of only two elements
        list->first = node2;
        node2->prev = NULL;

        list->last = node1;
        node1->next = NULL;

    } else if(!node1->prev) { // we are at the start of the list
        list->first = node2;
        node2->prev = NULL;

        node2->next->prev = node1;
        node1->next = node2->next;

    } else if(!node2->next) { // we are at the end of the list
        node1->prev->next = node2;
        node2->prev = node1->prev;

        list->last = node1;
        node1->next = NULL;

    } else { // we are in the middle of the list
        node1->prev->next = node2;
        node2->prev = node1->prev;

        node2->next->prev = node1;
        node1->next = node2->next;
    }
    // These happen no matter where in the list we are:
    node1->prev = node2;
    node2->next = node1;
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
                    swap_nodes(list, cur, cur->next);
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
    while(List_count(list1) > 0 || List_count(list2) > 0) {
        if(List_count(list1) > 0 && List_count(list2) > 0) {
            if(compare(list1->first->value, list2->first->value) <= 0) {
                List_push(result, List_fpop(list1));
            } else {
                List_push(result, List_fpop(list2));
            }
        } else if(List_count(list1) > 0) {
            List_push(result, List_fpop(list1));
        } else if(List_count(list2) > 0) {
            List_push(result, List_fpop(list2));
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
    if(List_count(list) <= 1) {
        return list;
    }
    debug("List_count of list %p is: %i", list, List_count(list));
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
    check((List_count(left) + List_count(right)) == List_count(list), "Incorrect split-list sizes");

    left = merge_sort(left, compare);
    check(left != NULL, "Error in left merge sort.");
    right = merge_sort(right, compare);
    check(right != NULL, "Error in right merge sort.");

    if(list)
        free(list);

    return merge_lists(left, right, compare);
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
