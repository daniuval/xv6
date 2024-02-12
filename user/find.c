#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path, char* name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  fd = open(path, 0);

  fstat(fd, &st);

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      if (strcmp(fmtname(buf), name) == 0) {
        printf("%s/%s\n", path, fmtname(buf));
      }
      if ((strcmp(fmtname(buf), ".") != 0) && (strcmp(fmtname(buf), "..") != 0)) {
        char new_path[strlen(fmtname(buf)) + strlen(path) + 2];
        for (int i = 0; i < strlen(path); ++i) {
            new_path[i] = path[i];
        }
        new_path[strlen(path)] = '/';
        for (int i = 0; i < strlen(fmtname(buf)) + 1; ++i) {
            new_path[strlen(path) + 1 + i] = fmtname(buf)[i];
        }
        find(new_path, name);
      }
    }
    break;
  }
  close(fd);
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("no no\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}