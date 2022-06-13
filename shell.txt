#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/wait.h> 
#include <ctype.h> 
#include <dirent.h> 
#include <fcntl.h> 
#include <time.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
 
#define      DEBUG        1 
#define      MAXLINELEN   4096 
#define      MAXARGS             128 
#define      END_OF_LINE  0 
#define      SEQ_OP       ';' 
#define      SEQUENCE     1 
 
struct cmd { 
struct cmd   *next; 
int          terminator; 
char         *exe_path; 
int          nargs; 
char         *arg[MAXARGS]; 
}; 
 
void   *ck_malloc(size_t   size) 
{ 
void   *ret = malloc(size); 
if (!ret) { 
perror("dumbshell:ck_malloc"); 
exit(1); 
} 
return ret; 
} 
 
char   *skip_to_non_ws(char       *p) 
{ 
int    ch; 
while (ch = *p) { 
if (ch != ' ' && ch != '\t' && ch != '\n') return p; 
++p; 
} 
return 0; 
} 
 
char   *skip_to_ws_or_sep(char    *p) 
{ 
int    ch; 
while (ch = *p) { 
if (ch == ' ' || ch == '\t' || ch == '\n') return p; 
if (ch == SEQ_OP) return p; 
++p; 
} 
return 0; 
} 
 
struct cmd *parse_commands(char  *line) 
{ 
char         *ptr; int          ix; struct cmd   *cur; 
 
ptr = skip_to_non_ws(line); 
if (!ptr) return 0; 
cur = ck_malloc(sizeof *cur); 
cur->next = 0; 
cur->exe_path = ptr; 
cur->arg[0] = ptr; 
cur->terminator = END_OF_LINE; 
ix = 1; 
for (;;) { 
ptr = skip_to_ws_or_sep(ptr); 
if (!ptr) { 
break; 
} 
if (*ptr == SEQ_OP) { 
*ptr = 0; 
cur->next = parse_commands(ptr+1); 
if (cur->next) { 
cur->terminator = SEQUENCE; 
} 
break; 
} 
*ptr = 0; 
ptr = skip_to_non_ws(ptr+1);  
if (!ptr) { 
break; 
} 
if (*ptr == SEQ_OP) { 
/* found a sequence operator */ 
cur->next = parse_commands(ptr+1); 
if (cur->next) { 
cur->terminator = SEQUENCE; 
} 
break; 
} 
cur->arg[ix] = ptr; 
++ix; 
} 
cur->arg[ix] = 0; cur->nargs = ix; return cur; 
} 
 
 
 
////////////////////////////////////////////////// 
void   execute(struct cmd  *clist) 
{ 
int ex; 
ex =0; 
   ///////////////////////////////////////////////////////////////cd  
  if (strcmp(clist->exe_path,"cd")==0){ 
  ex =1; 
     if (clist->nargs == 1) { ///change to home
        chdir(getenv("HOME")); 
       } 
       else{ 
         char* change = clist->arg[1]; //to change prive.. dir..
          if (strcmp(change,"..")==0) { 
         chdir("..");} 
        else if(strcmp(change,"~")==0||strcmp(change,"")==0||strcmp(change," ")==0){ ///change to home 
        chdir(getenv("HOME")); 
        //chdir("..");
          } 
     
        else if (change[0]=='.'){ 
       } 
        else if(change[0]=='/'){ ///change about dir.. using path
        chdir(change); 
        if(chdir(change)!=0){ ///to avoid error if input incorrect path
        perror("chdir() to /temp failed");  } 
    
  }   
  else{ /////change using name of dir...
        char wd[10000]; 
        char  path[1000];  
        snprintf (path, sizeof (path), "%s%s%s",getcwd(wd,sizeof(wd)),"/",change);    
        printf("%s\n",path); 
       chdir(path);   
       if(chdir(path)!=0) ///to avoid error 
          perror("chdir() ");} 
  } 
   
    } 
 int    pid, npid, stat; 
   pid = fork(); 
if (pid == -1) { 
perror("dumbshell:fork"); 
exit(1); 
} 
if(!pid){ 
  ////////////////////////////////////////////////////////pwd 
if( strcmp(clist->exe_path,"pwd")==0){ 
ex =1; 
  char wd[10000];     
    if (getcwd(wd, sizeof(wd)) != NULL) /// to Ensure that no error 
    fprintf(stdout," %s\n",getcwd(wd,sizeof(wd)));  //// to print direc… 
    else 
         perror("error");// to Avoid error 
} 
 
  /////////////////////////////////////////////////////////////ps 
  if( strcmp(clist->exe_path,"ps")==0){ 
  if (clist->nargs == 1){ 
  ex =1; 
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
 
} 
/////////////////////////////////////////////////////////////////////ps -A 
else { 
 //char* dir = clist->arg[1]; 
     if (!strcmp(clist->arg[1], "-A")){ 
         ex =1; 
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
    }  
      //execvp(clist->exe_path,clist->arg); 
    } 
} 
  /////////////////////////////////////////////////////Kill 
   
     if( strcmp(clist->exe_path,"kill")==0){ 
    ex =1; 
    // to kill more one process 
    int x; 
    for (x =1;x<clist->nargs;x++){ 
         if (x==1){
         kill (atoi(clist->arg[1]), SIGKILL); 
         continue;
         }
       kill(atoi(clist->arg[x]),SIGSEGV); ///kill process 
       
    } 
    
        perror("error"); 
         /// to sho result
         
} 
if (ex ==0){ 
  execvp(clist->exe_path,clist->arg); 
    fprintf(stderr,"No such command: %s\n",clist->exe_path); 
  } 
   
 
 
 
 
 
exit(1);  
} 
do { 
  
 
 
npid = wait(&stat); 
printf("Process %d exited with status %d\n",npid,stat);  
} while (npid != pid); 
switch (clist->terminator) { 
case SEQUENCE: 
execute(clist->next); 
} 
  ///////////////////////////////////////////////////// 
   
} 
 
void   free_commands(struct cmd   *clist) 
{ 
struct cmd   *nxt; 
 
do { 
nxt = clist->next; 
free(clist); 
clist = nxt; 
} while (clist); 
} 
 
char	*get_command(char   *buf, int	size, FILE	*in) 
{ 
if (in == stdin) { 
fputs("@ ",stdout); /* prompt */ 
} 
return fgets(buf,size,in); 
} 
 
void   main(void) 
{ 
char         linebuf[MAXLINELEN]; 
struct cmd   *commands; 
 
while (get_command(linebuf,MAXLINELEN,stdin) != NULL) { 
commands = parse_commands(linebuf); 
if (commands) { 
execute(commands); 
free_commands(commands); 
} 
} 
} 
