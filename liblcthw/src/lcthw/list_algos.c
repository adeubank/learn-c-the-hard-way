#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <stdbool.h>

typedef int (*List_compare)(ListNode *a, ListNode *b);
typedef int *(*sort_cb)(List *words, List_compare cmp);

void ListNode_swap(ListNode *a, ListNode *b) 
{
  check(a && b, "Won't swap a NULL value."); 

  void *temp = a->value; 
  check_mem(temp);

  a->value = b->value;
  b->value = temp;

error: 
  return;
}

/*
 * Works on the list, does not make a copy.
 * Implementation of the very inefficient bubble sort.
 */
int List_bubble_sort(List *words, List_compare cmp) 
{
  check(words != NULL, "Won't work with a null list");

  bool swapped = false;
  do {
    swapped = false;

    LIST_FOREACH(words, first, next, cur) {

      if (cur->next && cmp(cur->value, cur->next->value) > 0) {
        ListNode_swap(cur, cur->next);
        swapped = true;
      }
    }
  } while (swapped == true);

  return 0;

error:
  return 1;
}

/*
 * Works on the list, does not make a copy.
 * Implementation of the nlgn merge sort.
 */
int List_merge_sort(List *words, List_compare cmp) 
{
  log_warn("Not implemented");
  return 1;
}
