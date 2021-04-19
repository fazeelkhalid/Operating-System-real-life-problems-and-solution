#include<stdio.h>
#include<iostream>
#include<unistd.h>

using namespace std;


void convertCase(char*str){
    for(int i = 0;str[i] != '\0'; i++){
        if(str[i]>='A' && str[i]<='Z') { 
            str[i] +=32; // if character in upper case add 32
        }
        else if(str[i]>='a' && str[i]<='z') { //if case in lower case negative 32
            str[i] -=32;
        }
    }
}

int main() {
    int pipe1[2]; // parent can write but child only can read
    int pipe2[2]; // parent can read but child only can write
    char str[100];

    cout<<"Enter string : "; // take input from user
    cin.getline(str,100);
    
    if (pipe(pipe1) == -1) {
       perror("pipe");
       return 1;
    }
   
   if (pipe(pipe2) == -1) {
      perror("pipe");
      return 1;
   }

    pid_t pid = fork();
   
    if(pid == 0){ // child process call
        close(pipe1[1]); // Close the writing end of pipe1
        close(pipe2[0]); // Close the reading end of pipe2
        cout<<"\nchild Process\n";
        char receiveMessage[100];
        read(pipe1[0], receiveMessage, 100);
        convertCase(receiveMessage); // reverse each case
        write(pipe2[1], receiveMessage, sizeof(receiveMessage));
    }
    else{ // Parent process
        char finalAnswer[100];
        close(pipe1[0]); // Close the reading end of pipe1
        close(pipe2[1]); // Close the writing end of pipe2
        cout<<"\nParentProcess\n"; 
        write(pipe1[1], str, sizeof(str));// writting in pipe1
        read(pipe2[0], finalAnswer, sizeof(finalAnswer)); // reading from pipe2;
        cout<<"final answer is :"<<finalAnswer;
    }
    
   return 0;
}