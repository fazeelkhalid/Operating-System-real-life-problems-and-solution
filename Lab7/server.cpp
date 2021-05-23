#include <iostream>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <queue>
#include <string.h>

using namespace std;

int main()
{


	char *buffer;

	int id_sem=shmget(990, 1024, 0666 | IPC_CREAT | IPC_EXCL);
	int id_cs= shmget(991, 1024,  0666| IPC_CREAT | IPC_EXCL);

	sem_t *semaphore;

	if (id_sem==-1 || id_cs==-1)
	{

	cout<<"ERROR!";
	return 1;

	}

	int readFd = open("input.txt", O_RDONLY);
	char * readData= new char(2);
	
	semaphore= (sem_t*) shmat(id_sem, NULL, 0);
	buffer= (char*) shmat(id_cs, NULL, 0);
	sem_init(semaphore, 1, 0); //1 means semaphore is used for process synchronization

	while(*readData!='$'&& *readData!='\n'){
		char temp[21];
		int i = 0;
		
		for(; i< 20 && *readData!='$'; i++){
			read(readFd,readData,1);
			temp[i] = readData[0];
		}
		temp[i] = '\0';
		strcpy(buffer, temp);
		char a;
		cout<<"Press any key to continue: ";
		cin>>a;
		cout<<temp;
		sem_post(semaphore); 	
		
	}
	close(readFd);
	sem_destroy(semaphore);
	shmdt(semaphore);
	shmdt(buffer);
	shmctl(id_sem, IPC_RMID, NULL);
	shmctl(id_cs, IPC_RMID, NULL);
}
