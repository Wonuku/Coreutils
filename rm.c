#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#define EXIT_SUCCESS 0;
#define EXIT_FAILURE 1;

#define DISPLAYHELPTEXT printf("Usage: %s [OPTIONS].. [File]..\nRemoves file(s)\n\n\t--recursive, -r\t\tRecursively removes a directory\n\t--help\t\t\tDisplays this help.\n", argv[0])
#define DISPLAYUSAGE printf("Usage: %s [File]...\nTry \'%s --help\' for more information\n", argv[0], argv[0])

int chopN(char *str, size_t n)
{
    if(!(n != 0 && str != 0)){
      return -1;
    }
    size_t len = strlen(str);
    if (n > len){
        return 0;  // Or: n = len;
    }
    memmove(str, str+n, len - n + 1);
    return 0;
}

void rRm(const char *path){
    DIR *d = opendir(path);
    struct dirent *dir;
    struct stat filemeta;
    char cwd[512];
    if (d){
    while ((dir = readdir(d)) != NULL)
    {
      if (!strcmp(".",dir->d_name)||!strcmp("..",dir->d_name)) {
        continue;
      }
      stat(dir->d_name,&filemeta);
      if (S_ISDIR(filemeta.st_mode)){
        rRm(dir->d_name);
      }
      getcwd(cwd, 512);
      strcat(&cwd[0],"/");
      strcat(&cwd[0],path);
      strcat(&cwd[0],dir->d_name);
      if(!remove(cwd)){
        printf("Can't remove %s\n", dir->d_name);
      }
    }

    closedir(d);
    }
    if(remove(path)){
      printf("Can't remove %s\n", path);
    }

}

void rm(char *files[],size_t filec,size_t force){
  struct stat filemeta;
  size_t warning = 0;
  for (size_t i = 0; i < filec; i++){
    if (!strcmp(files[i],"   IGNORE   ")) {
      continue;
    }
    stat(files[i],&filemeta);
    if (S_ISDIR(filemeta.st_mode) && force == 1){
      rRm(files[i]);
    } else if (S_ISDIR(filemeta.st_mode) && force == 0 && warning == 0){
      printf("Can't remove dirs -r is not passed\n");
      warning = 1;
    } else {
      if ((remove(files[i]) != 0)) {
        printf("Can't remove %s\n", files[i]);
      }
    }
  }
}

int main(int argc, char *argv[]){
    if (argc < 2) {
      DISPLAYUSAGE;
      return EXIT_SUCCESS;
    }

    char *files[512];
    size_t filec = 0, force = 0;
    for (size_t i = 1; i < argc; i++) {
      char *resArgv = NULL;

      if (!strncmp(argv[i], "--", 2)) {
        resArgv = argv[i];
        if(chopN(resArgv,2) == -1){
          printf("chopN - Panic!\n");
          return -1;
        }
        if (!strcmp(resArgv,"help")) {
          DISPLAYHELPTEXT;
          return EXIT_SUCCESS;
        } else if (!strcmp(resArgv,"recursive")) {
          force = 1;
        } else {
          printf("Invalid argument: %s\n", argv[i]);
          return EXIT_FAILURE;
        }

      } else if (!strncmp(argv[i], "-", 1)) {
        resArgv = argv[i];
        chopN(resArgv,1);
        if (!strcmp(resArgv,"r")) {
          force = 1;
        } else {
          printf("Invalid argument: %s\n", argv[i]);
          return EXIT_FAILURE;
        }

      } else {
        if (!strncmp(argv[i], "/", 1)) {
          printf("Executing this command on root is dangerous type an uppercase YES to continue ");
          char *in = NULL;
          scanf("%s\n", in);
          if (!strcmp(in,"YES")) {
            files[filec] = "   IGNORE   ";
            filec++;
            continue;
          }
        }
        files[filec] = argv[i];
        filec++;
      } //maybe add moar safeguards TODO find out why rm isn't removing nested dir's files

    }
    if ((force == 1) && filec == 0) {
      DISPLAYUSAGE;
      return EXIT_SUCCESS;
    }

    rm(files,filec,force);
    return 0;
}
