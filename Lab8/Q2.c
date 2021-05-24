#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
sem_t semaphore[2];

int buffer[100];
int bufferIndex = 0;
int sum = 0;
int producer_exit = 0;
void * Producer(void * arg);
void * Consumer(void * arg);

int main()
{

	sem_init(&semaphore[0],0,1);
	sem_init(&semaphore[1],0,1);

	pthread_t thread[2];
	
	srand(time(NULL));
	pthread_create(&thread[0],NULL,&Producer,NULL);
	pthread_create(&thread[1],NULL,&Consumer,NULL);
	
	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	
	sem_destroy(&semaphore[0]);
	sem_destroy(&semaphore[1]);
return 0;
}


void * Producer(void * arg) {

	for(int i = 0; bufferIndex != 100; i++) {
		int j = 0;

		//sem_post(&semaphore2);
		for(; j < rand()%9 + 1 && bufferIndex<100; j++){
			buffer[bufferIndex]=rand()%6 + 1; // it will generate number from 1 to 6
			printf("index: %d \t Number : %d \n",bufferIndex,buffer[bufferIndex]);
			bufferIndex++;
		}
		sem_post(&semaphore[1]);
		sem_wait(&semaphore[0]);
	}
	producer_exit = 1;
	sem_wait(&semaphore[0]);

}
void * Consumer(void * arg) {
	
	for(int i = 0; i< 100; i++){
		
		sem_wait(&semaphore[1]);
		
		if(!producer_exit){
			sum += buffer[i];
			printf(" Sum : %d \n",sum);
		}
		else{

			printf("\n\nProducer end\n\n");
			while(i<100){
				sum += buffer[i];
				printf(" Sum : %d \n",sum);
				i++;
			}

		}
		sem_post(&semaphore[0]);
	}
}