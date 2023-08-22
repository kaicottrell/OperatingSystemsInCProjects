#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int numInput;
sem_t sem; //an instance of the semaphore object

void getInput(){
	for(int i=0; i<10; i++){
		printf("Enter a Number\n");
		scanf("%d", &numInput);
		sem_post(&sem); //increases sema
	}
//	usleep(1000);
}

void printInput(){
	for(int i=0; i<10; i++){
		sem_wait(&sem); //decreases the sema
		printf("Input: %d\n", numInput);
	}
}
int main(){ 
	pthread_t thread1; pthread_t thread2;
	
	//initalize semaphore
	sem_init(&sem, 0, 0);
	pthread_create(&thread1, NULL, (void *) getInput, NULL); //(Location of the thread, Default Attributes for thread, the function for the thread, and the parameters to the function)
	pthread_create(&thread2, NULL, (void *) printInput, NULL);	
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	printf("Completed\n");
	sem_destroy(&sem);
	
	return 0;

} 

