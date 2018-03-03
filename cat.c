#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    printf("Usage: %s [OPTIONS].. [File]...\nTry \'%s --help\' for more information\n", argv[0], argv[0]);
    return EXIT_SUCCESS;
  }

  return 0;
}
