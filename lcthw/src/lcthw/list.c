#include <lcthw/list.h>
#include <dbg.h>

List* List_create()
{
	List* new_list = calloc(1, sizeof(List));
	check_mem(new_list);
	return new_list;
error:
	return NULL;
}

void List_destroy(List* list)
{
	_CHECK_LIST(list);
	LIST_FOREACH(list, first, next, cur) {
		if(cur->prev) {
			free(cur->prev);
		}
	}
	free(list->last);
	free(list);
error:
	return;
}

void List_clear(List* list)
{
	_CHECK_LIST(list);
	LIST_FOREACH(list, first, next, cur) {
		free(cur->value);
	}
error:
	return;
}

void List_clear_destroy(List* list)
{
	_CHECK_LIST(list);
	LIST_FOREACH(list, first, next, cur) {
		if(cur->prev)
			free(cur->prev);
		free(cur->value);
	}
	free(list->last);
	free(list);
error:
	return;
}

void List_push(List* list, void* value)
{
	_CHECK_LIST(list);
	ListNode* node = calloc(1, sizeof(ListNode));
	check_mem(node);
	node->value = value;

	if(list->last == NULL) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;
error:
	return;
}

void* List_pop(List* list)
{
	_CHECK_LIST(list);
	ListNode* node = list->last;
	return node != NULL ? List_remove(list, node) : NULL;
error:
	return NULL;
}

void List_unshift(List* list, void* value)
{
	_CHECK_LIST(list);
	ListNode* node = calloc(1, sizeof(ListNode));
	check_mem(node);
	node->value = value;

	if(list->first == NULL) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}

	list->count++;
error:
	return;
}

void* List_shift(List* list)
{
	_CHECK_LIST(list);
	ListNode* node = list->first;
	return node != NULL ? List_remove(list, node) : NULL;
error:
	return NULL;
}

void* List_remove(List* list, ListNode* node)
{
	void* result = NULL;
	_CHECK_LIST(list);
	check(list->first && list->last, "List is empty.");
	check(node, "node can't be NULL.");

	if(node == list->first && node == list->last) {
		list->first = NULL;
		list->last = NULL;
	} else if(node == list->first) {
		list->first = node->next;
		check(list->first != NULL,
				"Invalid list. Somehow got a first that is NULL.");
		list->first->prev = NULL;
	} else if(node == list->last) {
		list->last = node->prev;
		check(list->last != NULL,
				"Invalid list. Somehow got a last that is NULL.");
		list->last->next = NULL;
	} else {
		ListNode* before = node->prev;
		ListNode* after = node->next;
		before->next = after;
		after->prev = before;
	}

	list->count--;
	result = node->value;
	free(node);
error:
	return result;
}

List* List_copy(List* list)
{
	_CHECK_LIST(list);
	List* new_list = List_create();
	LIST_FOREACH(list, first, next, cur) {
		List_push(new_list, cur->value);
	}
	return new_list;
error:
	return NULL;
}

void List_join(List* list1, List* list2)
{
	_CHECK_LIST(list1);
	_CHECK_LIST(list2);
	LIST_FOREACH(list2, first, next, cur) {
		List_push(list1, cur->value);
	}
error:
	return;
}

List* List_split(List* list, ListNode* node)
{
	_CHECK_LIST(list);
	List* new_list = List_create();
	int start_copying = 0;
	LIST_FOREACH(list, first, next, cur) {
		debug("List_split: started copying: %d", start_copying);
		if(start_copying == 1) {
			List_push(new_list, List_remove(list, cur));
		} else if(cur == node) {
			start_copying = 1;
			List_push(new_list, List_remove(list, cur));
		}
	}
	return new_list;
error:
	return NULL;
}

void List_print(List* list)
{
	_CHECK_LIST(list);
	int i = 0;
	LIST_FOREACH(list, first, next, cur) {
		if(cur->value)
			printf("%d:\t%s\n", i, (char*)cur->value);
		else
			printf("%d:\tNULL\n", i);
		i++;
	}

error:
	return;
}




