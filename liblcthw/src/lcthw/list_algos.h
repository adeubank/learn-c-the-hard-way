#ifndef lcthw_list_algos_h
#define lcthw_list_algos_h

typedef int (*List_compare)(ListNode *a,LiastNode *b);
typedef int *(*sort_cb)(List *words, List_compare cmp);



List *List_bubble_sort(List *words, List_compare cmp);



#endif

