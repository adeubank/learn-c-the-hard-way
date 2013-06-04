#include <stdio.h>
#include <ctype.h>

void print_arguments(int argc, char *argv[])
{
  int i = 0;

  for(; i < argc; i++) {
    int j = 0;
    char *arg = argv[i];

    for(; arg[j] != '\0'; j++) {
      char ch = arg[j];

      if(isalpha(ch) || isblank(ch)) {
        printf("'%c' == %d ", ch, ch);
      }
    }

    printf("\n");
  }
}

int main(int argc, char *argv[])
{
  print_arguments(argc, argv);
  return 0;
}
