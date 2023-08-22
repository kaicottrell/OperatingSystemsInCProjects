#include <stdio.h>
#include <pthread.h>

int x = 0;

void* my_thread_start(void* arg){
    printf("Hello from the thread\n");
    x = 5;
}
int main(){
    pthread_t thread;
    printf("Hello C World\n");
    pthread_create(&thread, NULL, my_thread_start, NULL); // give the thread we create a starting point(function to call)
    pthread_join(thread, NULL); //stops our main function from existing until this thread has finished
    printf("Value of x from main: %i\n", x); //thread does affect variables
    return 0;
}
