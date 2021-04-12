#include<iostream>
#include<string>
#include <unistd.h>
#include<string.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
struct command_t{
    char** ptr;
    char** pCmd; // hold array parse by space
    char** pPipe; //hold array parse by pipe symbol
    char** pOut; // hold array parse by output operator
    command_t(){
        ptr = NULL;
        pCmd = NULL;
        pPipe = NULL;
        pOut = NULL;
    }
    ~command_t(){
        for(int i = 0;i< 3; i++){
            if(ptr!=NULL){
                delete[]ptr;
            }
        }
        for(int i = 0;i< 10; i++){
            if(pCmd!=NULL){
                delete[]pCmd;
            }
        }
        for(int i = 0;i< 10; i++){
            if(pPipe!=NULL){
                delete[]pPipe;
            }
        }
        for(int i = 0; i< 10; i++){
            if(pOut!=NULL){
                delete[]pOut;
            }
        }
    }
};
using namespace std;
int countPipeSymbol(char * str){
    int count = 0;//count pipe symbol occurance
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == '|'){
            count ++;
        }
    }
    return count;
}
  
char ** parsePipe(char *str){
    char **ptr = new char*[3];
    int ptrIndex = 0; // control ptr index

    if(countPipeSymbol(str) == 0){ // if pipe symbol not present
        return NULL;
    }
    else {
        int i = 0; // control str array index
        for(; i< str[i]!= '\0'; i++){
            char *arr = new char[20];
            int arrIndex = 0; // control arr array index
            bool isRun = false;
            while( str[i] != '\0' && str[i] != '|') {
                arr[arrIndex] = str[i];
                arrIndex ++;
                isRun = true;
                i++;
            }
            arr[arrIndex] = '\0';
            if(isRun){
                ptr[ptrIndex] = arr;
                ptrIndex++;
            }
        }
        ptr[ptrIndex] = NULL;
        return ptr;
    }
}

char** parseSpace(char*str){
    char **ptr = new char*[10];
    int ptrIndex = 0; // control ptr index

    int i = 0; // control str array index
    for(; i< str[i]!= '\0'; i++){
        char *arr = new char[20];
        int arrIndex = 0; // control arr array index
        bool isRun = false;
        
        while( str[i] != '\0' && str[i] != ' ') {
            arr[arrIndex] = str[i];
            arrIndex ++;
            isRun = true;
            i++;
        }
        
        arr[arrIndex] = '\0';
        if(isRun){
            ptr[ptrIndex] = arr;
            ptrIndex++;
        }
    }
    ptr[ptrIndex] = NULL;
    return ptr;

}

char** parseOut(char* str){ // parse by output operator
   char **ptr = new char*[10];
    int ptrIndex = 0; // control ptr index

    int i = 0; // control str array index
    for(; i< str[i]!= '\0'; i++){
        char *arr = new char[20];
        int arrIndex = 0; // control arr array index
        bool isRun = false;
        
        while( str[i] != '\0' && str[i] != '>') {
            arr[arrIndex] = str[i];
            arrIndex ++;
            isRun = true;
            i++;
        }
        
        arr[arrIndex] = '\0';
        if(isRun){
            ptr[ptrIndex] = arr;
            ptrIndex++;
        }
    }
    ptr[ptrIndex] = NULL;
    return ptr;
    
}

void executePipe(char** parsed, char** parsedpipe) {
    int pipefd[2]; 
    pipe(pipefd); // pipe initialzed
  
    pid_t fd1 = fork();
    if (fd1 < 0) {
        cout<<"\nerror occur during fork";
        return;
    }
  
    if (fd1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        if (execvp(parsed[0], parsed) < 0) {
            for(char **temp = parsed; temp!=NULL; temp++){
                cout<<*temp;
            }
            cout<<": Command not execute.....";
            exit(0);
        }
    }
    else if(fd1 > 0){ //parent process. Here we will execute pipe
        pid_t fd2 = fork();
        
        if (fd1 < 0) {
            cout<< "\nerror occur during fork";
            return;
        }
  
        if (fd2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                for(char **temp = parsed; temp!=NULL; temp++){
                    cout<<*temp;
                }
                cout<<": Command not execute.....";
                exit(0);
            }
        } 
        else {
            wait(NULL);
            wait(NULL);
        }
    }
}
int main(){
    command_t command;
    char inputCommand[50] = "man ls | grep ls > file.txt";
    command.pOut = parseOut(inputCommand); // pars output symbol ">"
    
    int oFds = open(command.pOut[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); //open file in writing mode
    close(1);
    dup(oFds);
    command.ptr = parsePipe(command.pOut[0]);
    
    if(command.ptr !=NULL){ // if pipe exist in input command
        command.pCmd = parseSpace(command.ptr[0]);
        executePipe(command.pCmd,command.pPipe);
    }
    else{
        cout<<"Pipe symbol not present \n";
    }

    close(oFds);
  
    close(1);
    return 0;
}