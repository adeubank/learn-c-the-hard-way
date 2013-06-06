/* import utility functions */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Define a struct that has a string, and 3 ints.
struct Person {
  char *name;
  int age;
  int height;
  int weight;
};

// constructor method for Person struct
struct Person Person_create(char *name, int age, int height, int weight)
{
  // Grab a block of memory the size of the person struct
  struct Person who;

  who.name = strdup(name); // Duplicate and copy  name
  who.age = age;           // Set age property
  who.height = height;     // Set height property
  who.weight = weight;     // Set weight property

  assert(&who != NULL); // Exit if memory block is bad
  return who;               // return the newly created struct.
}

// Destructor method to delete a Person struct
void Person_destroy(struct Person who)
{
  // Make sure struct is legit
  assert(&who != NULL);

  // Free up memory created by string array
  free(who.name);
  who.age = NULL; // Free up the rest of the struct
  who.height = NULL; // Free up the rest of the struct
  who.weight = NULL; // Free up the rest of the struct
}

// Print a Person struct
void Person_print(struct Person who)
{
  printf("Name: %s\n", who.name);        // Print the name of the person
  printf("\tAge: %d\n", who.age);        // Print the age of the person
  printf("\tHeight: %d\n", who.height);  // Pritn the height of the person
  printf("\tWeight: %d\n", who.weight);  // Print the weight of the person
}

// Start of the program
int main(int argc, char *argv[])
{
  // make two people structures
  struct Person joe = Person_create(
        "Joe Alex", 32, 64, 140);
  struct Person frank = Person_create(
        "Frank Blank", 20, 72, 180);

  // print them out and where they are in memory
  printf("Joe is at memory location %p:\n", &joe);
  Person_print(joe);

  printf("Frank is at memory location %p:\n", &frank);
  Person_print(frank);

  // make everyones age 20 years and print them again
  joe.age += 20;
  joe.height -= 2;
  joe.weight += 40;
  Person_print(joe);

  frank.age += 20;
  frank.weight += 20;
  Person_print(frank);

  // destroy them both so we clean up
  Person_destroy(joe);
  Person_destroy(frank);

  // BREAKING IT
  /*Person_destroy(NULL);*/
  /*Person_print(NULL);*/

  // EXTRA CREDIT
  printf("Size of Person struct: %lu\n", sizeof(struct Person));

  // Return 0 to operating system and exit
  return 0;
}
