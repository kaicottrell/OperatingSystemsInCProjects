#include <stdio.h>
#include <pthread.h> 
#include <unistd.h> // allows for sleep function

void *function1();
void *function2();

pthread_mutex_t mutex;
int x = 0;
int main (){
	pthread_mutex_init(&mutex, NULL);
	pthread_t f_one, f_two;
	pthread_create(&f_one, NULL, function1, NULL);
	pthread_create(&f_two, NULL, function2, NULL);
	pthread_join(f_one, NULL);
	pthread_join(f_two, NULL);

	return 0;
}
void *function1(){
	//put a lock on mutex (resources)
	while(x < 1000){
		sleep(100);
		pthread_mutex_lock(&mutex);
		printf("Thread one has acquired the mutex\n");
		x = x + 50;
		pthread_mutex_unlock(&mutex);
		printf("thread one has released the mutex\n");
	}
	
}

void *function2(){
	while(x < 1000){
		pthread_mutex_lock(&mutex);
		printf("Thread two has acquired the mutex\n");
		x = x + 40;
		sleep(1);
		pthread_mutex_unlock(&mutex);
		printf("thread two has released the mutex\n");
	}
	
}
