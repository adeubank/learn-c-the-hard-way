#ifndef lcthw_list_algos_h
#define lcthw_list_algos_h

typedef int (*List_compare)(ListNode *a, ListNode *b);
typedef int *(*sort_cb)(List *words, List_compare cmp);

int List_bubble_sort(List *words, List_compare cmp);
int List_merge_sort(List *words, List_compare cmp);

#endif

