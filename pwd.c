#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main() {
    char wd[10000];
    char str[1000];
    printf("ENTER PWD\n");
    fgets(str, sizeof(str), stdin); ////to enter command pwd
    if (getcwd(wd, sizeof(wd)) != NULL&& strcmp(str,"pwd\n")==0) /// to Ensure that the entry is correct, and that there is no error
    fprintf(stdout," %s",getcwd(wd,sizeof(wd)));  //// to print direc…
    else
         perror("error");// to Avoid error
  
    return 0;
}

