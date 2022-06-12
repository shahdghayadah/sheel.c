#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    char s[100];
  char *input [1000] ;
    //Printing the current working directory
    printf("%s\n",getcwd(s,100));
     scanf("%[^\n]", input);   //  To enter directory 
     cd(input);//Changing the current working directory based on what has been entered
     printf("%s\n",getcwd(s,100));    //Printing the now current working directory


   
    return 0;
}
/////cd function
int cd(char *input[]){
 
  if (strcmp(input,"..")==0) { //Changing the current working directory to previous  directory
       chdir("..");
  }
 else if(input[0]=='\0'||input[0]=='~'){//Changing the current working directory to home directory
    chdir(getenv("HOME"));
    //chdir("..");
   }
    
  else if (input[0]=='.'){ //no change
      return 0;
    }
else if(input[0]=='/'){ //to change about path
      chdir(input);
    if(chdir(input)!=0){
        perror("chdir() to /temp failed");  
   }
  }  
  ///to change by use name of directory
  else{
        char wd[10000];
        char  path[1000]; 
        snprintf (path, sizeof (path), "%s%s%s",getcwd(wd,sizeof(wd)),"/",input);   ///to build path direc.. that you need 
        printf("%s\n",path);
       chdir(path);  
    //// to show error if input incorrect
       if(chdir(path)!=0)
          perror("chdir() ");
  }
return 0;
  }
