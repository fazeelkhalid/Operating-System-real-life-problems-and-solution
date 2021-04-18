#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;
struct command{
    char*cmd_name;
    char** argv;
    command(){
        cmd_name = new char[50];
        argv = new char*[4];
        argv[3] = NULL;
    }
    void setCmd_command(string str){
        for(int i = 0; str[i] != '\0'; i++){
            cmd_name[i] = str[i];
        }
    }
    ~command(){
        if(cmd_name!=NULL) delete []cmd_name;
        for(int i = 0; i< 4; i++){
            if(argv[i]!=NULL) delete []argv[i];
        }
    }
};

char* strToArr(string str2){
    char * temp = new char[50];
    int i = 0;
    for(; str2[i] != '\0'; i++){
        temp[i] = str2[i];
    }
    temp[i] = '\0';
    return temp;
}


void creat2Folder(){//it will creat 2 folders with name "myFolder_1" nad "my_Folder2"; 
    command exeCommand;
    exeCommand.setCmd_command("mkdir");
    exeCommand.argv[0] = strToArr("./mkdir");    
    exeCommand.argv[1] = strToArr("./myFolder_1");
    exeCommand.argv[2] = strToArr("./my_Folder1");
    execvp(exeCommand.cmd_name,exeCommand.argv);

}
void creatSubFolder(){ // it will creat sub folder in "myFolder_1"
    command exeCommand;
    exeCommand.setCmd_command("mkdir");
    exeCommand.argv[0] = strToArr(".mkdir");
    exeCommand.argv[1] = strToArr("./my_Folder1/fazedsadel");;
    execvp(exeCommand.cmd_name,exeCommand.argv);
}
void executeLS(){ // it will execute LS command with argument -S
    command exeCommand;
    exeCommand.setCmd_command("ls");
    exeCommand.argv[0] = strToArr("-S");
    exeCommand.argv[1] = NULL;
    execvp(exeCommand.cmd_name,exeCommand.argv);
}
int main(){
    pid_t fFID;// first Fork ID
	fFID = fork(); // creat child and parent processes

	if(fFID==0){ // first child process
		cout<<"\nEnter in first child process";
        cout<<"\ntwo folder created";
        cout<<"\n\n\n";
        creat2Folder();
	}

	else if(fFID>0){ // PARENT PROCESS
		pid_t fSID = fork();

		if(fSID == 0){ // second child
			cout<<"\nEnter in second child process";
			cout<<"\nSub folder created";
            cout<<"\n\n\n";
            creatSubFolder();
		}
		else if(fSID>0) {// parent process
			wait(NULL);
			pid_t fTID = fork(); // third child;
			if(fTID == 0 ){ // third child
				cout<<"\nEnter in third child process";
				cout<<"\nExecute Ls command with argument -S";
                cout<<"\n\n\n";
                executeLS();
			} 
		}
	}	
	return 0;
}
