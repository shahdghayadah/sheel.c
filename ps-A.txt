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
  struct dirent *ent;
  int i, fd;
  unsigned long time, stime;
  char flag;
  char name[256],  path[256], timeProc[256];
     FILE *fp;

  dir = opendir("/proc/");//open dir..
  printf("%5s %s\t%8s %5s\n", "PID", "TTY", "TIME", "CMD");

  while ((ent = readdir(dir)))//////to join and read from dire..
  {
  for (i = 0; ent->d_name[i]; i++)
  if (!isdigit(ent->d_name[i])){ 
break;
    }
  snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
        fp = fopen(path, "r");
            if (!fp) {///to avoid  file no need 
            continue;
        }
   fscanf(fp, "%d%s%5c", &i, name, &flag);///to read from file 
   name[strlen(name) - 1] = '\0';

  for (i = 0; i < 11; i++)////to get time 
  sscanf(path, "%lu", &time);
  sscanf(path, "%lu", &stime);
  time = ((time + stime) / sysconf(_SC_CLK_TCK));////change unit time 
  sprintf(timeProc, "%2lu:%2lu:%2lu",
  (time / 3600) % 3600, (time / 60) % 60, time % 60);////time equ...
 
  printf("%5s %s\t%8s %s\n", ent->d_name, ttyname(fd) + 5, timeProc, name + 1);/////print inform.. about proceess
 // fclose(file);
  
}
  closedir(dir);
return 0;
}
