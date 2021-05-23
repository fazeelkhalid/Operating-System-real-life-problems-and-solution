#include <iostream>
using namespace std;
#include <semaphore.h>
#include <sys/shm.h>
#include <queue>
#include <string.h>

bool isDollarSignPresent(char* arr){
	for(int i = 0; arr[i]!+'\0'; i++){
		if(arr[i] == '$'){
			arr[i] = '\0';
			return 1;
		}
	}
	return false;
}


int main() {
	char *buffer;

	int id_sem=shmget(990, 1024, 0);
	int id_cs= shmget(991, 1024,  0);

	sem_t *semaphore;

	if (id_sem==-1 || id_cs==-1)
	{
		cout<<"ERROR!";
		return 1;
	}
	semaphore= (sem_t*) shmat(id_sem, NULL, 0);
	buffer= (char*) shmat(id_cs, NULL, 0);
	
	bool is_terminated = false;
	while(!is_terminated ){
		int writeFd = open("output.txt", O_WRONLY);
		sem_wait(semaphore);
		
		is_terminated = isDollarSignPresent(buffer);
		write(writeFd,buffer,1);
		sem_post(semaphore); 
	}
	shmdt(semaphore);
	shmdt(buffer);

}
