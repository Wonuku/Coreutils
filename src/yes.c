#include <stdio.h>
#include <strings.h>

int main(int argc, char const *argv[]) {
  if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-?")) ) {
    printf("Usage: %s: STRING(S)\n", argv[0]);
    return 0;
  }

  if (argc > 2) {
    char buf[512] = {};
    for (size_t i = 1; i < argc; i++) {
      strcat(buf, argv[i]);
    }
    while (1) {
      printf("%s\n", buf);
    }
  } else {
    while (1) {
      puts("y");
    }
  }

  return 0;
}
