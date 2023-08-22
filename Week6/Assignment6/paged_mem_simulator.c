#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "paged_memory.h"


unsigned int get_number_of_bits(unsigned int value) {
    return (unsigned int)(log2(value) + 1);
}

void print_help() {
    printf("Available commands:\n");
    printf("t: Translate virtual address to physical address\n");
    printf("r: Read value at address\n");
    printf("w: Write value to address\n");
    printf("q: Quit the simulator\n");
}

//You may find the following functions helpful as you implement this program.  log2, fscanf, sscanf
//Part 3: Simulation Setup
void paged_simulator(const char* memory_file){
    //Read file
    FILE* file = fopen(memory_file, "r");

    //test if file is located
    if(file == NULL){
        printf("Error. File not found\n");
        return;
    }

    // Read the inputs from the memory file
    unsigned int num_words_virtual;
    unsigned int num_words_physical;
    unsigned int num_words_per_page_frame;
    unsigned int page_table_loc;

    fscanf(file, "%u", &num_words_virtual); //read and assign num_words_virtual from file
    fscanf(file, "%u", &num_words_physical); //read and assign  num_words_physical from file
    fscanf(file, "%u", &num_words_per_page_frame); //read and assign num_words_per_page_frame from file
    fscanf(file, "%u", &page_table_loc); //read and assign num_words_virtual from file

    //Make sure inputs are correct
    if(!is_power_of_2(num_words_virtual) ||!is_power_of_2(num_words_physical) ||
        !is_power_of_2(num_words_per_page_frame) || (page_table_loc % num_words_per_page_frame != 0)){
        printf("One of the inputs incorrect! Invalid format/inputs\n");
        fclose(file);
        return; 
    }

    unsigned int address_size = get_number_of_bits(num_words_virtual);
    unsigned int num_bits_page_num = get_number_of_bits(num_words_virtual / num_words_per_page_frame);
    unsigned int num_bits_offset = get_number_of_bits(num_words_per_page_frame);
    unsigned int num_bits_frame_number = get_number_of_bits(num_words_physical / num_words_per_page_frame);

    int* main_memory = (int*)malloc(num_words_physical * sizeof(int)); //allocate memory in bytes for main_memory based on the number of words * number of bytes in the int data tyoe

    for(unsigned int index = 0; index < num_words_physical; index++){
        fscanf(file,"%d", &main_memory[index]); //feeding ints into memory for each word in the phyiscal memory
    }

    fclose(file);

    //Print address_size, num_bits_page_num, num_bits_offset
    printf("Allocated all memory from file.\n");
    printf("Calculated address_size: %u\n", address_size);
    printf("Calculated number of bits for page number: %u\n", num_bits_page_num);
    printf("Calculated number of bits for offset: %u\n", num_bits_offset);
    printf("Calculated number of bits per frame number: %u\n", num_bits_frame_number);

    //Part 4 Simulator loop
    char cmd;
    unsigned int virtual_address;
    int value;

    while (true) {
        printf("Enter a command (t/r/w/q) or h for help: ");
        scanf(" %c", &cmd);

        if (cmd == 't') {
            printf("Enter virtual address: ");
            scanf("%u", &virtual_address);
            int physical_address = get_physical_address(virtual_address, num_bits_offset, page_table_loc, main_memory);
            printf("Translated physical address: %d\n", physical_address);
        } else if (cmd == 'r') {
            printf("Enter virtual address: ");
            scanf("%u", &virtual_address);
            int result = read_value(virtual_address, num_words_per_page_frame - 1, page_table_loc, main_memory);
            printf("Value at address %u: %d\n", virtual_address, result);
        } else if (cmd == 'w') {
            printf("Enter virtual address: ");
            scanf("%u", &virtual_address);
            printf("Enter value: ");
            scanf("%d", &value);
            write_value(value, virtual_address, num_words_per_page_frame - 1, page_table_loc, main_memory);
            printf("Value %d written to address %u\n", value, virtual_address);
        } else if (cmd == 'q') {
            break;
        } else if (cmd == 'h') {
            print_help();
        } else {
            printf("Invalid command. Enter 'h' for help.\n");
        }
    }

    free(main_memory);


}
//Part 2: Command Line Usage
int main(int argc, char *argv[]){ //argc is the number of command line arguments
    if(argc != 2){ // needs to be two because *memory_file[0] is the command itself, then we need the file to read
        printf("Error. Try again with two arguments.\n");
        return 1;
    }

    char *memory_file = argv[1];
    printf("File: %s has been retrieved\n", memory_file);

    paged_simulator(memory_file);
    return 0;
}


