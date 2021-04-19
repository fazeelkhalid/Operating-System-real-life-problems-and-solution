#include<iostream>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

key_t key = 9876;

int main(){
    int shmId;

    pid_t fid = fork(); // creat child and parent process
    if(fid == -1){
        perror("fork");
        return 0;
    }
    if(fid == 0){ //child Process
        cout<<"\nChild Process \n";
        shmId = shmget(key, sizeof(int) * 5, 0666 | IPC_CREAT);
        int *data = (int *)shmat(shmId, (void*)0,0);
        
        for(int i = 0; i< 5; i++){
            cout<<"Enter number in "<<i<<" index : ";
            cin>>data[i];
        }
    }
    else{
        wait(NULL); // wait untill child process not terminate
        cout<<"\nparent Process";
        shmId = shmget(key, sizeof(int) * 5, 0666 | IPC_CREAT);
        int *data = (int *)shmat(shmId, (void*)0,0);
        int *temp = data;

        for(int i = 0; i< 5; i++, temp++ ){
            cout<<endl<<"Square of " << (*temp) <<" : "<<(*temp) * (*temp) ;
        }
        cout<<endl;
    }

    return 0;
}