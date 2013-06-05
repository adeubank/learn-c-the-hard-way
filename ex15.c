#include <stdio.h>

void print_alive(char *names[], int *ages);
void print_old(char *names[], int *ages);
void print_old_again(char *names[], int *ages);
void print_so_far(char *names[], int *ages);
void print_address(char *names[], int *ages);
int get_count(int *arr);

int main(int argc, char *argv[]) /* POINTERS DREADED POINTERS */
{
  // create two arrays we care about
  int ages[] = {23, 43, 12, 89, 2};
  char *names[] = {
    "Alan", "Frank",
    "Mary", "John", "Lisa"
  };

  // first way using indexing
  print_alive(names, ages);
  printf("---\n");

  // second way using pointers
  print_old(names, ages);

  printf("---\n");

  // third way, pointers are just arrays
  print_old_again(names, ages);

  printf("---\n");

  // fourth way with pointers in a stupid complex way
  print_so_far(names, ages);

  printf("---\n");

  // print the address of the strings
  print_address(names, ages);

  return 0;
}

/* returns size of array */
int get_count(int arr[]) {
  int i = 0;
  for(; i < 4; i++) {
    printf("%d\n", arr[i]);
  }
  int count = sizeof(arr) / sizeof(int);
  printf("count is %d", count);
  return count;
}

void print_alive(char *names[], int *ages) {
  // safely get the size of ages
  int count = get_count(ages); 
  int i = count-1;

  for(; i >= 0; i--) {
    printf("%s has %d years alive.\n",
            names[i], ages[i]);
  }
}

void print_old(char *names[], int *ages) {
  // safely get the size of ages
  int count = get_count(ages); 
  int i = count-1;

  // setup pointers to be start of the arrays
  int *cur_age = ages;
  char **cur_name = names;

  // second way using pointers
  for(i = count-1; i >= 0; i--) {
    printf("%s is %d years old.\n",
            cur_name[i], cur_age[i]);
  }
}
void print_old_again(char *names[], int *ages) {
  // safely get the size of ages
  int count = get_count(ages); 
  int i = count-1;

  // setup pointers to be start of the arrays
  int *cur_age = ages;
  char **cur_name = names;

  for(i = count-1; i >= 0; i--) {
    printf("%s is %d years old again.\n",
            cur_name[i], cur_age[i]);
  }
}

void print_so_far(char *names[], int *ages) {

  // safely get the size of ages
  int count = get_count(ages); 
  int i = count-1;

  // setup pointers to be start of the arrays
  int *cur_age = ages;
  char **cur_name = names;

  for(cur_name = names+(count-1), cur_age = ages+(count-1);
        (cur_age - ages) >= 0;
        cur_name--, cur_age--)
  {
    printf("%s lived %d years so far.\n",
            cur_name[0], cur_age[0]);
  }
}

void print_address(char *names[], int *ages) {
  // safely get the size of ages
  int count = get_count(ages); 
  int i = count-1;

  for(i = 0; i < count; i++) {
    printf("%s is stored at memory address %p\n", 
            names[i], &names[i]);
  }
}

