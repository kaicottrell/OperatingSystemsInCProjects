#include <stdio.h>
#include <stdbool.h>


struct PageTableEntry{
    int page_number;
    bool currentlyInMemory; 
    bool referencedBit; // recently accessed by a process.
};

bool containsPage(struct PageTableEntry** mainMemory, int size, int value) {
    for (int i = 0; i < size; i++) {
        if(mainMemory[i] != NULL){
            if (mainMemory[i]->page_number == value) {
                mainMemory[i]->referencedBit = true;
                return true;
            }
        } 
    }
    return false;
}



bool isMainMemoryFull(struct PageTableEntry** mainMemory, int numFrames) {
    for (int i = 0; i < numFrames; i++) {
        if (mainMemory[i]->page_number == -1) {
            return false;  // Main memory is not full
        }
    }
    return true;  // Main memory is full
}

void resetMemory(int numPagesInPTable, struct PageTableEntry* pageTable, int numFrames, struct PageTableEntry** mainMemory){
    //initialize the pageTable with empty values
    for(int i = 0; i < numPagesInPTable; i++){
        pageTable[i].page_number = -1;
        pageTable[i].currentlyInMemory = false;
        pageTable[i].referencedBit = false;
    }

    for (int i = 0; i < numFrames; i++) {
        // if frame # > the # of pages in the page table that index is defaulted to our null values
        if(i < numPagesInPTable){
            mainMemory[i] = &pageTable[i]; // Point the mainMemory array to the PageTable array. -- when mainMemory updates so does the pageTable
        } else{
            mainMemory[i] = NULL;
        }
    }
}
// use after algorithm to see what pages are loaded into memory at finish
void printFaultPercentage(int faults, int numMemoryAccesses, int* referenceString){
    double faultRatePercentage;
    faultRatePercentage = ((float) faults / numMemoryAccesses) * 100;
    printf("The fault rate percentage for [ ");
    for (int i = 0; i < numMemoryAccesses; i++) {
        printf("%d ", referenceString[i]);
    }
    printf("] is: %.2f\n", faultRatePercentage);
    printf("\n");
}
void printMemoryFrames(int numFrames, struct PageTableEntry** mainMemory){
    for (int i = 0; i < numFrames; i++){
        if(mainMemory[i] != NULL){
            printf("[%d] ", mainMemory[i]->page_number);
        } else{
             printf("Frame: %d is empty \n", i);
        }
    }
    printf("\n");
}

void optimalPagingAlgorithm(struct PageTableEntry** mainMemory, int numFrames, int page, int* referenceString, int numMemoryAccesses, int indexInReferenceString) {
    // if there is an empty frame:
    if (!isMainMemoryFull(mainMemory, numFrames)) {
        // Use an empty frame in the main memory
        for (int i = 0; i < numFrames; i++) {
            if (mainMemory[i] != NULL && mainMemory[i]->page_number == -1) {
                mainMemory[i]->page_number = page; // Assign the new page to the empty frame
                mainMemory[i]->currentlyInMemory = true;
                return;
            }
        }
    } else { // no empty frame -- swap
        int maxNextAppearance = -1; // variable to track the highest next reference
        int swapIndex = -1; // variable to track the index of the page to be swapped
        int nextIndexInReferenceString = indexInReferenceString + 1;
        // for each page in memory, find the next reference to that page in the reference string
    
        for (int i = 0; i < numFrames; i++) { // what we have in memory
            if(mainMemory[i] == NULL){
                return;
            }
            int currentPage = mainMemory[i]->page_number;
            bool foundNextAppearance = false;
            
            // find the next appearance of the current page in the reference string
            
            for (int j = nextIndexInReferenceString; j < numMemoryAccesses; j++) {
                if (referenceString[j] == currentPage) {
                    foundNextAppearance = true;
                    if (j > maxNextAppearance) {
                        maxNextAppearance = j;
                        swapIndex = i;
                    }
                    break;
                }
            }
            
            // if the current page has no next appearance, select it for swapping
            if (!foundNextAppearance) {
                swapIndex = i;
                break;
            }
        }

        // select the page with the highest next reference and swap it with the new page
        mainMemory[swapIndex]->page_number = page;
        mainMemory[swapIndex]->currentlyInMemory = true;
    }
}



void fifoPagingAlgorithm(struct PageTableEntry** mainMemory, int numFrames, int page, int* frame_pointer) {
    // if there is an empty frame use that:
    if (!isMainMemoryFull(mainMemory, numFrames)) {
        // Use an empty frame in the main memory
        for (int i = 0; i < numFrames; i++) {
            if (mainMemory[i] != NULL && mainMemory[i]->page_number == -1) {
                mainMemory[i]->page_number = page; // Assign the new page to the empty frame
                mainMemory[i]->currentlyInMemory = true;
                return;
            }
        }
    } else { // no empty frames -- needed swap
        mainMemory[*frame_pointer]->page_number = page;
        
        //Update the frame pointer in the bounds of the frame
        *frame_pointer = (*frame_pointer + 1) % numFrames;
    }
}

void secondChancePagingAlgorithm(struct PageTableEntry** mainMemory, int numFrames, int page, int* frame_pointer) {
    // if there is an empty frame use that:
    if (!isMainMemoryFull(mainMemory, numFrames)) {
        // Use an empty frame in the main memory
        for (int i = 0; i < numFrames; i++) {
            if (mainMemory[i] != NULL && mainMemory[i]->page_number == -1) {
                mainMemory[i]->page_number = page; // Assign the new page to the empty frame
                mainMemory[i]->currentlyInMemory = true;
                mainMemory[i]->referencedBit = true;
            }
        }
    } else { // no empty frames -- needed swap
       while (mainMemory[*frame_pointer]->referencedBit == true)
       {
            //unset reference bit
            mainMemory[*frame_pointer]->referencedBit = false;
            //increment frame_pointer modulo number of frames
            *frame_pointer = (*frame_pointer + 1) % numFrames;
       }
       // once we get to a referencedBit where it is false (0) -- we are going to swap it
        mainMemory[*frame_pointer]->page_number = page;
        mainMemory[*frame_pointer]->referencedBit = true;
    }
    // we dont want to look at the frame we just added so we advance the frame pointer
    *frame_pointer = (*frame_pointer + 1) % numFrames;
}

int main(int argc, char *argv[]){ //argc is the number of command line arguments
    if(argc != 1){ 
        printf("Error. Try again with no additional arguments.\n");
        return 1;
    }
    int numFrames, numPagesInPTable, numMemoryAccesses;
    //Assinging Values
    printf("Welcome to the page replacement simulator\n");
    printf("Enter the number of frames in main memory\n");
    scanf("%d", &numFrames);
    printf("Enter the number of pages in the page table\n");
    scanf("%d", &numPagesInPTable);
    printf("Enter the number of memory accesses in the reference string\n");
    scanf("%d", &numMemoryAccesses);

     // Read the reference string
    int referenceString[numMemoryAccesses];
    printf("Enter the reference string as a list of numbers separated by spaces:\n");
    for (int i = 0; i < numMemoryAccesses; i++) {
        scanf("%d", &referenceString[i]);
    }
    printf("\n");

    
    // an array of page_table_entries to represent the page table.
    struct PageTableEntry pageTable[numPagesInPTable];
    struct PageTableEntry* mainMemory[numFrames];

    resetMemory(numPagesInPTable, pageTable, numFrames, mainMemory);
    int faults;

    // Simulator
    // Optimal page algorithm
    faults = 0;
    for(int i = 0; i < numMemoryAccesses; i++){
        // if the page is not present in memory
        if (!containsPage(mainMemory, numFrames, referenceString[i])) {
            optimalPagingAlgorithm(mainMemory, numFrames, referenceString[i], referenceString, numMemoryAccesses, i);
            faults++;
        } 
    }
    
    printf("Optimal page algorithm:\n");
    printFaultPercentage(faults, numMemoryAccesses, referenceString);

    //reset
    resetMemory(numPagesInPTable, pageTable, numFrames, mainMemory);
    faults = 0;

    // FIFO page algorithm
    int framePointer = 0;
    
    for(int i = 0; i < numMemoryAccesses; i++){
        // if the page is not present in memory
        
        if (!containsPage(mainMemory, numFrames, referenceString[i])) {
            fifoPagingAlgorithm(mainMemory, numFrames, referenceString[i], &framePointer);
            faults++;
        }
    }

    printf("FIFO page algorithm:\n");
    printFaultPercentage(faults, numMemoryAccesses, referenceString);

    //reset
    resetMemory(numPagesInPTable, pageTable, numFrames, mainMemory);
    faults = 0;

    // Second Chance page algorithm
    framePointer = 0;
    
    for(int i = 0; i < numMemoryAccesses; i++){
        // if the page is not present in memory
        if (!containsPage(mainMemory, numFrames, referenceString[i])) {
            secondChancePagingAlgorithm(mainMemory,numFrames, referenceString[i], &framePointer);
            faults++;
        } 
    }
    
    printf("Second Chance page algorithm:\n");
    printFaultPercentage(faults, numMemoryAccesses, referenceString);

    return 0;
}



