#include <lcthw/dbg.h>
#include <lcthw/list_algos.h>

inline void ListNode_swap(ListNode *a, ListNode *b) 
{
  void *temp = a->value; 
  a->value = b->value;
  b->value = temp;
}

int List_bubble_sort(List *words, List_compare cmp) 
{
  int sorted = 1;

  if(List_count(words) <= 1) {
    return 0;
  }

  do {
    sorted = 1;

    LIST_FOREACH(words, first, next, cur) {
      if (cur->next && cmp(cur->value, cur->next->value) > 0) {
        ListNode_swap(cur, cur->next);
        sorted = 0;
      }
    }
  } while (!sorted);

  return 0;
}

/*
 * Given two lists, return a list that is sorted by the compare function
 */
inline List *List_merge(List *left, List *right, List_compare cmp)
{
  // receive the left and right sublist as arguments.
  // 'result' variable for the merged result of two sublists.
  // assign the element of the sublists to 'result' variable until 
  // there is no element to merge. 
  List *result = List_create();
  void *val = NULL;

  while(List_count(left) > 0 || List_count(right) > 0)
  {
    if(List_count(left) > 0 && List_count(right) > 0) {
     // compare the first two element, which is the small one, of each two sublists.
      if(cmp(List_first(left), List_first(right)) <= 0) {
        // the small element is copied to 'result' variable.
        // delete the copied one(a first element) in the sublist.
        val = List_shift(left);
      } 
      else {
        // same operation as the above(in the right sublist).
        val = List_shift(right);
      }
      List_push(result, val);
    }
    else if(List_count(left)> 0) {
      // copy all of remaining elements from the sublist to 'result' variable,
      // when there is no more element to compare with.
      val = List_shift(left);
      List_push(result, val);
    }
    else if(List_count(right) > 0) {
      // same operation as the above(in the right sublist).
      val = List_shift(right);
      List_push(result, val);
    }
  }

  // return the result of the merged sublists(or completed one, finally).
  // the length of the left and right sublists will grow bigger and bigger, 
  // after the next call of this function.
  return result;

}

/*
 * Works on the list, does not make a copy.
 * Implementation of the nlgn merge sort.
 */
List *List_merge_sort(List *words, List_compare cmp) 
{
  // if list size is 0 (empty) or 1, consider it sorted and return it
  // (using less than or equal prevents infinite recursion for a zero length m)
  if(List_count(words) <= 1) {
    return words;
  }
  // else list size is > 1, so split the list into two sublists
  // 1. DIVIDE Part...
  List *left = List_create();
  List *right = List_create();
  int middle = List_count(words) / 2;

  LIST_FOREACH(words, first, next, cur) {
    if(middle > 0) {
      // before middle, add to left
      List_push(left, cur->value);
    }
    else {
      // after or equal to middle, add to right
      List_push(right, cur->value);
    }

    middle--;
  }

  // recursively call merge_sort() to further split each sublist
  // until sublist size is 1
  List *sort_left = List_merge_sort(left, cmp);
  List *sort_right = List_merge_sort(right, cmp);

  if(sort_left != left) List_destroy(left);
  if(sort_right != right) List_destroy(right);

  // merge the sublists returned from prior calls to merge_sort()
  // and return the resulting merged sublist
  // 2. CONQUER Part...
  return List_merge(sort_left, sort_right, cmp);
}

