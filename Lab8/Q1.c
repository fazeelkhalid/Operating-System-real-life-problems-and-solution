#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
// initialization of sempahores  
sem_t semaphore[3];
pthread_t thread[3];

void* thread1(void* arg);
void* thread2(void* arg);
void* thread3(void* arg);

int main() {
    for(int i = 0; i< 3; i++){ //init semaphore
        sem_init(&semaphore[i], 0, 1);
    }

    pthread_create(&thread[0],NULL,thread1,NULL);
    pthread_create(&thread[1],NULL,thread2,NULL);
    pthread_create(&thread[2],NULL,thread3,NULL);

    for(int i = 0; i< 3; i++){ // join thread
        pthread_join(thread[i],NULL);
    }

    for(int i = 0; i< 3; i++){ // destory semaphore
        sem_destroy(&semaphore[i]);    
    }

    return 0;
}


// thread 1
void* thread1(void* arg) {
    while(1) {
        sem_post(&semaphore[2]);
        printf("aaa");
        sem_wait(&semaphore[1]);
    }  
}

// thread 2
void* thread2(void* arg) {
  
    while(1){  
        sem_wait(&semaphore[2]);
        printf("c");
        
        sem_post(&semaphore[0]); 
    }
}
void* thread3(void* arg) {
    while(1){  
        sem_wait(&semaphore[0]);
        printf("b");
        sem_post(&semaphore[1]);
    }
}
