#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void *function1();
void *function2();
void *function3();

pthread_mutex_t mutex1,mutex2, mutex3; // resources to be shared

int main() {
        pthread_mutex_init(&mutex1,NULL);
        pthread_mutex_init(&mutex2,NULL);
        pthread_mutex_init(&mutex3,NULL);
        pthread_t f_one, f_two, f_three;
        pthread_create(&f_one, NULL, function1, NULL);
        pthread_create(&f_two, NULL, function2, NULL);
        pthread_create(&f_three, NULL, function3, NULL);
        pthread_join(f_one, NULL);
        pthread_join(f_two, NULL);
        pthread_join(f_three, NULL);
        printf("Application Complete\n");
}

//requires resources 1 and 2
void *function1( ) {
        //put a lock on mutex (resources)
        printf("Function 1 waiting on mutex 1\n");
        pthread_mutex_lock(&mutex1);
        printf("Function 1 has acquired mutex 1\n");
        sleep(5);
        printf("Function 1 waiting on mutex 2\n");
        pthread_mutex_lock(&mutex2);
        printf("Function 1 has acquired mutex 2\n");
        sleep(5);
        pthread_mutex_unlock(&mutex1);
        printf("Function 1 has released mutex 1\n");
        pthread_mutex_unlock(&mutex2);
        printf("Function 1 has released mutex 2\n");
        sleep(1);
}
//requires resources 1,2,3
void *function2( ) {
        //put a lock on mutex (resources)
        printf("Function 2 waiting on mutex 2\n");
        pthread_mutex_lock(&mutex2);
        printf("Function 2 has acquired mutex 2\n");
        sleep(5);
        printf("Function 2 waiting on mutex 3\n");
        pthread_mutex_lock(&mutex3);
        printf("Function 2 has acquired mutex 3\n");
        sleep(5);
        printf("Function 2 waiting on mutex 1\n");
        pthread_mutex_lock(&mutex1);
        printf("Function 2 has acquired mutex 1\n");
        sleep(5);
        pthread_mutex_unlock(&mutex2);
        printf("Function 2 has released mutex 2\n");
        pthread_mutex_unlock(&mutex1);
        printf("Function 2 has released mutex 1\n");
        pthread_mutex_unlock(&mutex3);
        printf("Function 2 has released mutex 3\n");
        sleep(1);
}
//requires resource 2
void *function3( ) {
        printf("Function 3 waiting on mutex 2\n");
        pthread_mutex_lock(&mutex2);
        printf("Function 3 has acquired mutex 2\n");
        sleep(5);
        pthread_mutex_unlock(&mutex2);
        printf("Function 3 has released mutex 2\n");
        sleep(1);
}


// NOTE: use -pthread flag when compiling your code eg.
// gcc deadlock.c -o deadlock -pthread