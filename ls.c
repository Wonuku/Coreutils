#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define EXIT_SUCCESS 0;
#define EXIT_FAILURE 1;

#define DISPLAYHELPTEXT printf("Usage: %s [OPTIONS].. [File]...\nLists information about files (Targets current dir by default)\n\n\t-a, --all\t\tList files starting with .\n\t-A, --almost-all\tShow implied . and ..\n\t-i\t\tList file inode numbers\n\t-l\t\tList one file per line with details\n\t--help\t\tDisplays this help.\n", argv[0])

int chopN(char *str, size_t n)
{
    if(!(n != 0 && str != 0)){
      printf("Fatal chopN error! Exitting...\n");
      return -1;
    }
    size_t len = strlen(str);
    if (n > len){
        return 0;  // Or: n = len;
    }
    memmove(str, str+n, len - n + 1);
    return 0;
}

struct arguments{
  size_t all, almostAll, inode, longList;
};

int resolveArg(char *str, struct arguments *argz){
    size_t count = strlen(str);
    for (size_t i = 0; i < count; i++) {
      char cur = str[i];
      if (cur == 'a') {
        argz->all = 1;
      } else if (cur == 'A') {
        argz->almostAll = 1;
      } else if (cur == 'i') {
        argz->inode = 1;
      } else if (cur == 'l') {
        argz->longList = 1;
      } else {
        printf("Invalid argument: %s\n", str);
        return -1;
      }
    }
    return 0;
}

void printPerms(struct stat *fileStat) {
  printf( (S_ISDIR(fileStat->st_mode)) ? "d" : "-");
  printf( (fileStat->st_mode & S_IRUSR) ? "r" : "-");
  printf( (fileStat->st_mode & S_IWUSR) ? "w" : "-");
  printf( (fileStat->st_mode & S_IXUSR) ? "x" : "-");
  printf( (fileStat->st_mode & S_IRGRP) ? "r" : "-");
  printf( (fileStat->st_mode & S_IWGRP) ? "w" : "-");
  printf( (fileStat->st_mode & S_IXGRP) ? "x" : "-");
  printf( (fileStat->st_mode & S_IROTH) ? "r" : "-");
  printf( (fileStat->st_mode & S_IWOTH) ? "w" : "-");
  printf( (fileStat->st_mode & S_IXOTH) ? "x " : "- ");
}

int ls(struct arguments args,char *files[],const size_t filec){
  size_t in2 = 0, in1 = 0, in0 = 0;
  if (filec > 1) {
    in2 = 1;
  }
  for (size_t i = 0, co = 0; i < filec; i++) {
    struct stat filemeta;
    stat(files[i],&filemeta);
    in2 ? printf("%s:\n",files[i]) : 0;

 if (S_ISDIR(filemeta.st_mode)) {
    DIR *d = opendir(files[i]);
    struct dirent *dir;

  if (d){
      while ((dir = readdir(d)) != NULL){
        if ((((args.all != 1)||(args.almostAll == 1)) && !strncmp(".",dir->d_name,1))||(!strcmp(".",dir->d_name)||(!strcmp("..",dir->d_name)))){
          if ((args.almostAll == 1) && ((!strcmp(".",dir->d_name))||(!strcmp("..",dir->d_name)))) {
            goto lolno;
          }
          continue;
        }
        lolno:;
        struct group *grp = getgrgid(filemeta.st_gid);
        struct passwd *pwd = getpwuid(filemeta.st_uid);

        char *date = ctime(&filemeta.st_mtime);
        date[strlen(date) - 1] = '\0';

        if (args.longList == 1 && args.inode == 0) {
          stat(dir->d_name,&filemeta);
          printPerms(&filemeta);
          printf(" %lu %s %s %ld %s %s\n", filemeta.st_nlink, pwd->pw_name, grp->gr_name, filemeta.st_size, date,dir->d_name);
        } else if (args.longList == 1 && args.inode == 1) {
          stat(dir->d_name,&filemeta);
          printPerms(&filemeta);
          printf("%lu %lu %s %s %ld %s %s\n", filemeta.st_ino, filemeta.st_nlink, pwd->pw_name, grp->gr_name, filemeta.st_size, date,dir->d_name);
        } else if (args.inode == 1) {
          stat(dir->d_name,&filemeta);
          printf("%lu %s  ", filemeta.st_ino, dir->d_name);
          in1 = 1;
        } else {
          printf("%s ", dir->d_name);
          in1 = 1;
        }
        co++;
      }

      closedir(d);

      if (in1 == 1 && in2 == 1) {
        if (in0 == filec -1)
          break;
        printf("\n");
        in0++;
      }
      if (!(co == 0)) {
          co = 0;
          printf("\n");
      }
  }
  } else { // aka #~$ls regularFile
  printf("%s\n", files[i]);
  }

  }
  return 0;
} //permissions are broken TODO some formatting to make it pretty TODO

int main(int argc, char *argv[]){

  char *files[512];
  size_t filec = 0;
  struct arguments args;
  args.all = 0;args.almostAll = 0;args.inode = 0;args.longList = 0; //sorry return key is broken
  for (size_t i = 1; i < argc; i++) {
    char *resArgv = NULL;

    if (!strncmp(argv[i], "--", 2)) {
      resArgv = argv[i];
      chopN(resArgv,2);
      if (!strcmp(resArgv,"help")) {
        DISPLAYHELPTEXT;
        return EXIT_SUCCESS;
      } else if (!strcmp(resArgv,"all")) {
        args.all = 1;
      } else if (!strcmp(resArgv,"almost-all")) {
        args.almostAll = 1;
      } else {
        printf("Invalid argument: %s\n", argv[i]);
        return EXIT_FAILURE;
      }

    } else if (!strncmp(argv[i], "-", 1)) {
      resArgv = argv[i];
      chopN(resArgv,1);

      if(resolveArg(resArgv,&args) == -1){
        return -1;
      }

    } else {
      files[filec] = argv[i];
      filec++;
    }
   }

   if (filec == 0) {
     files[0] = ".";
     filec = 1;
   }
   ls(args,files,filec);

   return 0;
}
