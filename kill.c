    if( strcmp(clist->exe_path,"kill")==0){ 
    //ex =1; 
    if((clist->nargs)==5){ // to kill more one process 
    int sh = clist->nargs; ///get num of proc.. input
    int x; 
    for (x =2;x<sh;x++){ 
       kill(atoi(clist->arg[x]),atoi(clist->arg[1])); ///kill process 
       
    } 
    }else{ 
     //   ex =1; 
    int x =   kill (atoi(clist->arg[1]), SIGKILL); 
    } 
        perror("error"); 
         /// to show result
} 