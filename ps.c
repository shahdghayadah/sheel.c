#include <stdio.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv){
  DIR *dir; //Definition of directory
  struct dirent *ent; //to difin.. pointer to dir
  int i, fd_self, fd;
  unsigned long time, stime;
  char flag;
  char name[256],  path[256], timeProc[256];
  

  dir = opendir("/proc");//to open dir..
  fd_self = open("/proc/self/fd/0", O_RDONLY);///to open file  you need
  printf("%5s %s\t%8s %5s\n", "PID", "TTY", "TIME", "CMD");

  while ((ent = readdir(dir)) != NULL)////to show files in dir..
  {
  flag = '!';
  for (i = 0; ent->d_name[i]; i++)
  if (!isdigit(ent->d_name[i]))
  { 
   flag = '\0';
   break;
  }

  if (flag!='\0')
  {
  sprintf(path, "/proc/%s/fd/0", ent->d_name);///to make true path 
  fd = open(path, O_RDONLY);///open file 
  if (ttyname(fd) && strcmp(ttyname(fd),  ttyname(fd_self)) == 0)///To make sure it's in the correct file 
  {
   sprintf(path, "/proc/%s/stat", ent->d_name);/////to make path about stat 
   fscanf(fopen(path, "r"), "%d%s%c%c%c", &i, name, &flag, &flag, &flag);////to read information about process from file 
   name[strlen(name) - 1] = '\0';

  for (i = 0; i < 11; i++)///to read time proce..
  sscanf(path, "%lu", &time);
  sscanf(path, "%lu", &stime);
    ///////////// to make time by use equation & change the unit of time to Hz 
  time = ((time + stime) / sysconf(_SC_CLK_TCK));
  sprintf(timeProc, "%2lu:%2lu:%2lu",
  (time / 3600) % 3600, (time / 60) % 60, time % 60);
 
  printf("%5s %s\t%8s %s\n", ent->d_name, ttyname(fd) + 5, timeProc, name + 1); ///to print information about proc..
  }
 close(fd);
}
}
close(fd_self);
return 0;
}
