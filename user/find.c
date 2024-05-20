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
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

char*
lastname(char *path)
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
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}

int
find(char* path, const char* filename)
{
  char buf[512], *p;
  int fd, count = 0;
  uchar result;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return 0;
  }

  if(fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return 0;
  }

  if (st.type != T_DIR)
  {
    fprintf(2, "find: error stat type %s, it's not a directory\n", path);
    close(fd);
    return 0;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    fprintf(2, "find: path too long\n");
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
      fprintf(2, "find: cannot stat %s\n", buf);
      continue;
    }

    if (st.type == T_FILE)
    {
      result = (strcmp(filename, lastname(buf)) == 0);
      if (result)
      {
        count++;
        // printf("%s %d %d %d\n", /*fmtname(buf)*/buf, st.type, st.ino, st.size);
        fprintf(1, "%s\n", buf);
      }
    }
    else if (st.type == T_DIR)
    {
      result = (strcmp(".", lastname(buf)) != 0 && strcmp("..", lastname(buf)) != 0);
      if (result)
      {
        count += find(buf, filename);
      }
    }
    
    // printf("%s %d %d %d\n", /*fmtname(buf)*/buf, st.type, st.ino, st.size);
  }
  return count;
}

int
main(int argc, char** argv)
{
  // int count = 0;

  if (argv[1] == 0 || argv[2] == 0)
  {
    fprintf(2, "find: find must has 3 parameters\n");
    exit(0);
  }
  find(argv[1], argv[2]);

  // fprintf(1, "totally find %d files named %s\n", count, argv[2]);
  exit(0);
}