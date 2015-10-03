#include <stdio.h>
#include <stdlib.h> //Needed by malloc(), free()
#include <string.h> //Needed by strtok(), strcmp()
#include <limits.h> // Needed by PATH_MAX
#include <unistd.h> // Needed by getcwd(), execvp()
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

int token(char *buf);
char* input();
int cd(char *token);
int sys(char **buf);

int token(char *buf){
    char *token = strtok(buf," ");
    int i=0;
    char **argList = (char**) malloc(sizeof(char*) * 10);
    while(token != NULL) {
        argList[i] = (char*)malloc(sizeof(char) * 10);
        strcpy(argList[i],token);
        token = strtok(NULL," ");
        i=i+1;
    }
    if (strcmp(argList[0], "cd") == 0){
        cd(argList[1]);
    }
    else if (strcmp(argList[0], "exit") == 0){
        exit(0);  
    }
    else {
        sys(argList);
    }
}

char* input(){
    char cwd[PATH_MAX+1];
    char *buf = malloc(255*sizeof(char));
    if(getcwd(cwd,PATH_MAX+1) != NULL){
        printf("[I am Shell] %s $:", cwd);
        fgets(buf,255,stdin);
        buf[strlen(buf)-1] = '\0';
        return buf;
    }
    else{
        printf("Error Occured!\n");
    }
    return NULL;
}

int cd(char *token){
    char buf[PATH_MAX+1];
    if(chdir(token) != -1){
        getcwd(buf,PATH_MAX+1);
        printf("Now it is %s\n",buf);}
    else{
        printf("Cannot Change Directory\n");
    }
    return 0;
}

int sys(char **token) {
    setenv("PATH","/bin:/usr/bin:.",1);
    pid_t child_pid;
    if(!(child_pid = fork())) {
        execvp(*token,token);
        if(errno == ENOENT)
            printf("No Command found...\n\n");
        else
            printf("I dont know...\n");
    }
    else{
        waitpid(child_pid,NULL,WUNTRACED);
    }
}

int main(){
    while (1) token(input());
}