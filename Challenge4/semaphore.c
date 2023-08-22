#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int input1, input2, input3;

int sum = 0;

int product = 0;
sem_t sem; // an instance of the semaphore object
void getInput(){

            for(int i=0;i<10;i++){
                printf("Enter Number 1:\n");
                scanf("%d", &input1);
                printf("Enter Number 2:\n");
                scanf("%d", &input2);
                printf("Enter Number 3:\n");
                scanf("%d", &input3);
                printf("\nInput1: %d Input2: %d Input3: %d\n", input1, input2, input3);
                sem_post(&sem); //increases semaphore
                usleep(500);
                
        }
}

void outputSumProduct(){

        for(int i=0;i<10;i++){
            sem_wait(&sem); //decrease semaphore
            sum = input1 + input2 +input3;
            product = input1 * input2 * input3;
            printf("Sum: %d\n", sum);
            printf("Product: %d\n\n", product);
        }

}

int main(){
        pthread_t thread1;
        pthread_t thread2;

        //initialize semaphore
        sem_init(&sem, 0, 0);

        pthread_create(&thread1, NULL, (void *)getInput, NULL);
        pthread_create(&thread2, NULL, (void *)outputSumProduct, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        printf("Completed\n");
        sem_destroy(&sem);

        return 0;
}