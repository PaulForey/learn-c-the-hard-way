#ifndef lcthw_List_h
#define lcthw_List_h
#include <stdlib.h>

// An internal macro to check the validity of a List:
#define _CHECK_LIST(A) \
	check((A) != NULL, "list must not be NULL."); \
	check((A)->count >= 0, "Invalid list. A list can't have a negative count."); \
	if((A)->count > 0) \
		check((A)->first != NULL, "Invalid list. A list can't have a positive count without a first element.");

struct ListNode;
typedef struct ListNode {
	struct ListNode* next;
	struct ListNode* prev;
	void* value;
} ListNode;

typedef struct List {
	int count;
	ListNode* first;
	ListNode* last;
} List;

/*
 * List_create -- Allocates memory in which to make a new List.
 * returns:	List* or NULL if there were errors.
 */
List* List_create();

/*
 * List_destroy -- Frees all the ListNodes from a List (DOES NOT free the ListNode's values).
 */
void List_destroy(List* list);

/*
 * List_clear -- Frees all the given List's ListNodes' values.
 */
void List_clear(List* list);

/*
 * List_clear_destroy -- Frees all the given List's ListNode's values and all the ListNodes
 * 						   themselves. Complete destruction of the List.
 */
void List_clear_destroy(List* list);

/*
 * List_count -- A macro to return the count of the given list.
 * args:	List*
 * returns:	int
 */
#define List_count(A)	((A)->count)

/*
 * List_first -- A macro to return the first value of the given list.
 * args:	List*
 * returns:	the value or NULL if no value.
 */
#define List_first(A)	((A)->first != NULL ? (A)->first->value : NULL)

/*
 * List_last -- A macro to return the last value of the given list.
 * args:	List*
 * returns:	the value or NULL if no value.
 */
#define List_last(A)	((A)->last != NULL ? (A)->last->value : NULL)

/*
 * List_push -- Adds a value to the end of the list.
 */
void List_push(List* list, void* value);

/*
 * List_pop -- Removes and returns the value from the end of the list.
 */
void* List_pop(List* list);

/*
 * List_unshift -- Adds a value to the start of the list.
 */
void List_unshift(List* list, void* value);

/*
 * List_shift -- Removes and returns the value from the start of the
 * 				 list.
 */
void* List_shift(List* list);

/*
 * List_remove -- Removes (and then frees) the specified ListNode from
 * 				  the list.
 * returns:	The ListNode's value.
 */
void* List_remove(List* list, ListNode* node);

/*
 * LIST_FOREACH -- A macro for iterating through the list.
 * args:	List*, (first | last), (next | prev), cur
 * method:	Using the List* L, this will access L->S and then iterate
 * 			from there to the end of the list, using ListNode->M to
 * 			access the next node. On each iteration V is set to the 
 * 			current node.
 */
#define LIST_FOREACH(L, S, M, V)	ListNode* _node = NULL; \
	ListNode *V = NULL; \
	for(V = _node = L->S; _node != NULL; V = _node = _node->M)


/* * * * * * * * * * * * * * */
/*     EXTRA CREDIT ZONE     */
/* * * * * * * * * * * * * * */

/*
 * List_copy -- Returns a new list which has exactly the same values
 * 				as the last list.
 * returns:	List* or NULL if there were errors.
 */
List* List_copy(List* list);

/*
 * List_join -- Adds the values from the second list onto the end of
 * 				the first list.
 */
void List_join(List* list1, List* list2);

/*
 * List_split -- The given list is split at the given node. The
 * 				 supplied list's ListNodes are removed from the given
 * 				 node onwards (the given node is also removed), and
 * 				 a new list is returned containing the removed part
 * 				 of the given list.
 * returns:	List* or NULL if there were errors.
 */
List* List_split(List* list, ListNode* node);

/*
 * List_print -- Prints the list to stdout in a nice(ish) format.
 */
void List_print(List* list);

void List_debug(List* list);

#endif // lcthw_List_h
