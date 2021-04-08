#include<iostream>
#include<string>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/wait.h>
#include <unistd.h>

using namespace std;

bool isSpecialChar(char match){ // check is given character is a special character or not
	if(match == '&'){
		return 1;
	}
	else if(match == '@'){
		return 1;
	}
	else if(match == '#'){
		return 1;
	}
	else if(match == '%'){
		return 1;
	}
	else if(match == '*'){
		return 1;
	}
	else if(match == '?'){
		return 1;
	}
	else if(match == '$'){
		return 1;
	}
	else if(match == '"'){
		return 1;
	}
	else if(match == '~'){
		return 1;
	}
	else if(match == '"'){
		return 1;
	}
	else {
		return 0;
	}
}

int arrSize(char*str){ // calculate string size
	int size = 0;
	for(; str[size]!='\0'; size++);
	return size;
}

void removeSpecialChar(char * str){ // remove special characters from the given array
	int size = arrSize(str) + 1;
	for(int i = 0; i<= size; i++){
		if(isSpecialChar(str[i])){
			for(int j = i; j <= size; j++){
				str[j] =str[j+1]; 
			}
			i--;
		}
	}
}


void readDataFromFd(int inputFd, char *str){ // read data from fd character byu character
	char tempChar = ' ';
	int i = 0;
	for(; tempChar !='\n' ;i++){
		read(inputFd,&tempChar,1); // read from file
		str[i] = tempChar;
	}
	str[i] ='\0';
	
}

int main(){
	
	char inputFile[15] = "file.txt";
	char outputFile[15] = "output.txt";
	char input[1000];//use to read data from file
	int pipeFd[2]; //Child to parent pipe
	int pToCFd[2]; //parent to child pipe
	pipe(pipeFd);
	pipe(pToCFd);
	
	int inputFd = open(inputFile, O_RDONLY);//open input file just for reading
	if(inputFd == -1){ // retrun 1 if input file not open
		return 1;
	}
	
	readDataFromFd(inputFd,input);// read from file	
	write(pToCFd[1],input,arrSize(input)); // write in pipe
	close(pToCFd[1]);
	
	/*---------------------------------------------
	----CREAT CHILD AND PARENT PROCESS BY FORK-----
	-----------------------------------------------*/
	
	int forkId = fork();
	if(forkId == -1){ // child process not created
		return 4;
	}
	wait(NULL);
	
	if(forkId == 0){ // child process
		char localStr[1000];
		close(pipeFd[0]);
		close(pToCFd[1]);
		readDataFromFd(pToCFd[0],localStr);// read string from pipe
		removeSpecialChar(localStr); // remove special char
		
		//insert into pipe
		write(pipeFd[1],localStr, arrSize(localStr));
		
		close(pipeFd[1]);
	}
	else if(forkId>0){ // parent process
		wait(NULL);
		char localStr[1000];
		close(pipeFd[1]);
		
		readDataFromFd(pipeFd[0],localStr);// read string from pipe
		//remove(outputFile);
		//creat and open file for reading purpose
		if(creat(outputFile, S_IRWXU) == -1){ // return 2 if creat file process not sucessfully process
			return 2;
		}
		
		
	}
	return 0;
}
