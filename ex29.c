#include <stdio.h>
#include "dbg.h"
#include <dlfcn.h>

typedef int (*lib_function)(const char *data);

int main(int argc, char *argv[])
{
  int rc = 0;
  char *lib_file = "build/libex29.so";

  void *lib = dlopen(lib_file, RTLD_NOW);
  check(lib != NULL, "Failed to open the library %s: %s", lib_file, dlerror());

  // print_a_message
  lib_function func = dlsym(lib, "print_a_message");
  check(func != NULL, "Did not find %s function in the library %s: %s", "print_a_message", lib_file, dlerror());

  rc = func("hello there");
  check(rc == 0, "Function %s return %d for data: %s", "print_a_message", rc, "hello there");

  // uppercase
  func = dlsym(lib, "uppercase");
  check(func != NULL, "Did not find %s function in the library %s: %s", "uppercase", lib_file, dlerror());

  rc = func("hello there");
  check(rc == 0, "Function %s return %d for data: %s", "uppercase", rc, "hello there");

  // uppercase
  func = dlsym(lib, "lowercase");
  check(func != NULL, "Did not find %s function in the library %s: %s", "lowercase", lib_file, dlerror());

  rc = func("HELLO THERE");
  check(rc == 0, "Function %s return %d for data: %s", "lowercase", rc, "HELLO THERE");

  // fail_on_purpose
  func = dlsym(lib, "fail_on_purpose");
  check(func != NULL, "Did not find %s function in the library %s: %s", "fail_on_purpose", lib_file, dlerror());

  rc = func("HELLO THERE");
  check(rc == 1, "Function %s return %d for data: %s", "fail_on_purpose", rc, "HELLO THERE");

  rc = dlclose(lib);
  check(rc == 0, "Failed to close %s", lib_file);

  return 0;

error:
  return 1;
}
