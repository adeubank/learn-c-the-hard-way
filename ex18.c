#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

void print_array(int *numbers, int count);
/** Our old friend die from ex17. */
void die(const char *message)
{
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

// a typedef creates a fake type, in this
// case for a function pointer.
typedef int (*compare_cb)(int a, int b);
typedef int *(*sort_cb)(int *numbers, int count, compare_cb cmp);

/**
 * A classic bubble sort function that uses the
 * compare_cb to do the sorting.
 */
int *bubble_sort(int *numbers, int count, compare_cb cmp)
{
  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if (!target) die("Memory error.");

  memcpy(target, numbers, count * sizeof(int));

  for(i = 0; i < count; i++) {
    for(j = 0; j < count - 1; j++) {
      if (cmp(target[j], target[j+1]) > 0) {
        temp = target[j+1];
        target[j+1] = target[j];
        target[j] = temp;
      }
    }
  }

  return target;
}

void sift_down(int numbers[], int root, int bottom, compare_cb cmp) {
  int maxChild = root * 2 + 1;

  // Find the bigger child
  if (maxChild < bottom) {
    int otherChild = maxChild + 1;
    // Reversed for stability
    maxChild = (cmp(numbers[otherChild], numbers[maxChild]) > 0)?otherChild:maxChild;
  } else {
    // Don't overflow
    if (maxChild > bottom) return;
  }

  // If we have the correct ordering, we are done.
  if (cmp(numbers[root], numbers[maxChild]) > 0) return;

  // Swap
  int temp = numbers[root];
  numbers[root] = numbers[maxChild];
  numbers[maxChild] = temp;

  // Tail queue recursion. Will be compiled as a loop with correct compiler
  // switches.
  sift_down(numbers, maxChild, bottom, cmp);
}

/* Operates on a copy of the input array, not on the original. */
int *heap_sort(int *numbers, int count, compare_cb cmp)
{
  int *target, tmp, i;

  if (!(target = malloc(count * sizeof(int))))
    die("Memory Error.");
  memcpy(target, numbers, count * sizeof(int));

  for (i = (count / 2); i >= 0; i--) {
    sift_down(target, i, count-1, cmp);
  }

  for (i = count-1; i >= 1; i--) {
    // Swap
    tmp = target[0];
    target[0] = target[i];
    target[i] = tmp;

    sift_down(target, 0, i-1, cmp);
  }
  return target;
}

int sorted_order(int a, int b)
{
  return a - b;
}

int reverse_order(int a, int b)
{
  return b - a;
}

int strange_order(int a, int b)
{
  if (a == 0 || b == 0) {
    return 0;
  } else {
    return a % b;
  }
}

/**
 * Used to test that we are sorting things correctly
 * by doing the sort and printing it out.
 */
void test_sorting(int *numbers, int count, compare_cb cmp, sort_cb sort)
{
  int *sorted = sort(numbers, count, cmp);

  if (!sorted) die("Failed to sort as requested");

  print_array(sorted, count);

  free(sorted);

}

void print_array(int *numbers, int count)
{
  int i = 0;
  for (i = 0; i < count; i++) {
    printf("%d ", numbers[i]);
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  if (argc < 2) die("USAGE: ex18 4 3 1 5 6");

  int count = argc - 1;
  int i = 0;
  char **inputs = argv + 1;

  int *numbers = malloc(count * sizeof(int));
  if (!numbers) die("Memory error.");

  for (i = 0; i < count; i++) {
    numbers[i] = atoi(inputs[i]);
  }

  printf("Bubble sort\n");
  test_sorting(numbers, count, sorted_order, bubble_sort);
  test_sorting(numbers, count, reverse_order, bubble_sort);
  test_sorting(numbers, count, strange_order, bubble_sort);

  printf("Heap sort\n");
  test_sorting(numbers, count, sorted_order, heap_sort);
  test_sorting(numbers, count, reverse_order, heap_sort);
  test_sorting(numbers, count, strange_order, heap_sort);

  free(numbers);

  return 0;
}

