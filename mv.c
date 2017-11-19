#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>

#define DISPLAYHELPTEXT printf("Usage: %s [OPTIONS].. [File]...\nMoves file(s) to the destination\n\n\t-f, --force\tDon't ask before overwriting\n\t--help\tDisplays this help.\n", argv[0])

int chopN(char *str, size_t n)
{
    if(!(n != 0 && str != 0)){
      printf("Fatal chopN error! Exitting...\n"); //remember to remove this message form the rest of the corelibs
      exit(-1);
    }
    size_t len = strlen(str);
    if (n > len){
        return 0;  // Or: n = len;
    }
    memmove(str, str+n, len - n + 1);
    return 0;
}

int getCharNo(char *str){
  for (size_t i = strlen(str); i > 0; i--) {
    if (str[i] == '/') {
      if (i == strlen(str)) {
        continue;
      }
      return i;
    }
  }
  return -1;
}

int mv(int force, char *files[], int filec){
  struct stat filemeta, filemeta1;
  int fis[512]; //0 == regular file, 1 == dir 2 == other types

  for (size_t i = 0; i < filec; i++) {
    stat(files[i], &filemeta);
    if (S_ISREG(filemeta.st_mode)) {
      fis[i] = 0;
    } else if (S_ISDIR(filemeta.st_mode)) {
      fis[i] = 1;
    } else {
      fis[i] = 2;
    }
  }

  for (size_t i = 0; i < filec - 1; i++) {
      link(files[i], files[filec]);
      
  }

  return -1; //should not get to this point
}

int main(int argc, char *argv[]){
  if (argc < 2) {
    printf("Usage: %s [OPTIONS].. [File]...\nTry \'%s --help\' for more information\n", argv[0], argv[0]);
    return EXIT_SUCCESS;
  }

  char *files[512];
  size_t filec = 0, force = 0;
  for (size_t i = 1; i < argc; i++) {
    char *resArgv = NULL;

    if (!strncmp(argv[i], "--", 2)) {
      resArgv = argv[i];
      chopN(resArgv,2);
      if (!strcmp(resArgv,"help")) {
        DISPLAYHELPTEXT;
        return EXIT_SUCCESS;
      } else if (!strcmp(resArgv,"force")) {
        force = 1;
      } else {
        printf("Invalid argument: %s\n", argv[i]);
        return EXIT_FAILURE;
      }

    } else if (!strncmp(argv[i], "-", 1)) {
      resArgv = argv[i];
      chopN(resArgv,1);

      if (!strcmp(argv[i],"f")) {
        force = 1;
      }

    } else {
      files[filec] = argv[i];
      filec++;
    }
   }

   mv(force,files,filec);

   return 0;
}
