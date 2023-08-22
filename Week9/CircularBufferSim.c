#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // allows for the sleep function
#include <stdbool.h>

pthread_mutex_t mutex;

int numSequences = 0;

// Structure to hold multiple parameters for the producerProduce function
struct ProducerParams {
    int burst_length;
    int buffer_size;
    int transaction_length;
};

// Structure to hold a single parameter for the consumerConsume function
struct ConsumerParams {
    int buffer_size;
    int transaction_length;
};

struct Node {
    int data;
    int index; // tracks the position of that buffer within the circular buffer
    struct Node* next;
};
struct Node** headNode;

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode != NULL) {
        newNode->data = data;
        newNode->next = NULL;
        newNode->index = -1;
    }
    return newNode;
}

void insertNode(int data) {
    struct Node* newNode = createNode(data);
    if (headNode != NULL) {
        newNode->next = *headNode;
        *headNode = newNode;
    } else {
        *headNode = newNode;
    }
}

// THIS CAUSES AN INFINITE LOOP - only for demonstration of the circular linked list
void displayList() {
    struct Node* current = *headNode;
    while (current != NULL) {
        printf("%d (index: %d) -> ", current->data, current->index);
        current = current->next;
    }
    printf("NULL\n");
}

void incrementIndexes() {
    struct Node* current = *headNode;
    // update index for each node in the linked list
    while (current != NULL) {
        current->index = current->index + 1;
        current = current->next;
    }
}

void* producerProduce(void* args) {
    struct ProducerParams* params = (struct ProducerParams*)args;
    int burst_length = params->burst_length;
    int buffer_size = params->buffer_size;

    struct Node* startNode; // keeps track of where we are starting from in the buffer
    struct Node* sequentialNode; // checks how many sequentialNodes there are
    struct Node* fillNode; // keeps track of where we are filling in the buffer
    int countSequential;
    // this loop iterates through each buffer slot to see if there is enough room

    while (numSequences < params->transaction_length) {
        startNode = *headNode;
        for (int i = 0; i < buffer_size; i++) { // go through check if something is empty
            countSequential = 0;
            if (startNode->data == -1) { // Check if big enough size
                countSequential = 1;
                sequentialNode = startNode->next;
                // this loop counts the number of sequential blocks while allowing for circular incrementation
                while (sequentialNode->data == -1 && sequentialNode != startNode) {
                    countSequential++;
                    sequentialNode = sequentialNode->next;
                }
                if (countSequential >= burst_length) {
                    // fill blocks
                    fillNode = startNode;
                    usleep(1);
                    pthread_mutex_lock(&mutex);
                    for (int j = 0; j < burst_length; j++) {
                        if (numSequences >= params->transaction_length) { // Check the condition before filling the blocks
                            pthread_mutex_unlock(&mutex);
                            return NULL;
                        } else{
                            fillNode->data = rand() % 101;
                            printf("Sequence #%d: Buffer Index %d, Producer Produced %d\n", numSequences, fillNode->index, fillNode->data);
                            fillNode = fillNode->next;
                            numSequences++;
                        }
                    }
                    pthread_mutex_unlock(&mutex);
                }
            } else{
                startNode = startNode->next;
            }
        }
        
       
    }

    return NULL;
}

void* consumerConsume(void* args) {
    struct ConsumerParams* params = (struct ConsumerParams*)args;
    int buffer_size = params->buffer_size;
    struct Node* currentNode;
    while (numSequences < params->transaction_length) {
        currentNode = *headNode;
        for (int i = 0; i < buffer_size; i++) {
            if(numSequences >= params->transaction_length){
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
            else if (currentNode->data != -1) {
                usleep(1);
                pthread_mutex_lock(&mutex);
                printf("Sequence #%d: Buffer Index %d, Consumer Consumed %d\n", numSequences, currentNode->index, currentNode->data);
                currentNode->data = -1;
                numSequences++;
                currentNode = currentNode->next;
                pthread_mutex_unlock(&mutex);
            } else {
                currentNode = currentNode->next;
            }
        }
    
        
    }

    return NULL;
}

int main() {
    // Part 1
    int buffer_size, producer_burst_length, transaction_length;
    printf("Enter a size of the Circular Buffer: \n");
    scanf("%d", &buffer_size);

    // Part 2
    printf("Enter a size Producer Burst Length: \n");
    scanf("%d", &producer_burst_length); // Producer will only produce data when empty buffers are equal to  the  amount of burst length are available

    // Part 3 and 4
    printf("Enter Transaction (Sequence) Length: \n");
    scanf("%d", &transaction_length);

    headNode = (struct Node**)malloc(sizeof(struct Node*));
    *headNode = NULL;

    printf("Creating Circular Buffer with Size: %d\n", buffer_size);
    printf("===================================== \n");
    // allocate size of the linked list;
    for (int i = 0; i < buffer_size; i++) {
        insertNode(-1);
        incrementIndexes();
        printf("Buffer Index: %d, Data: %d\n", i, -1);
    }
    struct Node* temp = *headNode;
    // create circular linked list by pointing the last node to the first node
    while (1) {
        if (temp->next == NULL) {
            temp->next = *headNode;
            break;
        } else {
            temp = temp->next;
        }
    }

    struct ProducerParams producerParams;
    struct ConsumerParams consumerParams;

    producerParams.burst_length = producer_burst_length;
    producerParams.buffer_size = buffer_size;
    producerParams.transaction_length = transaction_length;

    consumerParams.buffer_size = buffer_size;
    consumerParams.transaction_length = transaction_length;

    printf("Starting Threads \n");
    printf("===================================== \n");

    pthread_mutex_init(&mutex, NULL);
    pthread_t f_one, f_two;
    pthread_create(&f_one, NULL, producerProduce, (void*)&producerParams);
    pthread_create(&f_two, NULL, consumerConsume, (void*)&consumerParams);
    pthread_join(f_one, NULL);
    pthread_join(f_two, NULL);
    printf("Circular Buffer Simulation Complete\n");
    
    return 0;
}
