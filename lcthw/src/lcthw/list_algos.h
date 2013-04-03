#ifndef _List_algos_h
#define _List_algos_h
#include <lcthw/list.h>

typedef int (*List_compare)(const char* str1, const char* str2);

int List_bubble_sort(List* list, List_compare compare);
int List_merge_sort(List* list, List_compare compare);

#endif // _List_algos_h

