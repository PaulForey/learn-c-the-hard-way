#ifndef _lcthw_List_algos_h
#define _lcthw_List_algos_h
#include <lcthw/list.h>

typedef int (*List_compare)(const char* str1, const char* str2);

List* List_bubble_sort(List* list, List_compare compare);
List* List_merge_sort(List* list, List_compare compare);

#endif // _lcthw_List_algos_h

