
#include <stdbool.h>
#include "paged_memory.h"
//Part 1: The paged memory library

bool is_power_of_2(unsigned int value) {
    if (value == 0)
        return false;

    while (value != 1) { // takes value all the way to one; divides until it reaches one if the number at any point is not divisble by two it is not a power of 2
        if (value % 2 != 0)
            return false;
        value = value / 2;
    }
    return true;
}

int get_physical_address(unsigned int virtual_address, unsigned int offset_bits, unsigned int page_table_loc, int* physical_memory){
    unsigned int page_number = virtual_address >> offset_bits; //bit shifts to the right by the offset_bits; we want to get the page group number 
    unsigned int offset = virtual_address & ((1 << offset_bits) - 1); //gets the less signficant bits of the virtual address and stores in the offset
    int* page_entry = physical_memory +  page_number + page_table_loc; //references a specific page table entry in memory
    int physical_address = (*page_entry << offset_bits) | offset; // shifts the place of the page_entry to the left by the off_set bits and adds on the offset to create a complete memory location
    return physical_address;
}

//Takes a virtual address and returns the value at the corresponding physical address. The virtual address, the number of bits used for the offset, the starting location of the page table(s), and a pointer to the physical memory are also passed as parameters.
int read_value(int virtual_address, int page_mask, int page_table_loc, int* physical_memory){
    int page_number = virtual_address & page_mask; // getting the page number out of the virtual address w/ page_mask
    int physical_address = page_table_loc + page_number; //gather our physical address of the value
    int value = physical_memory[physical_address]; //extract the value out of the physical memory at the specific address
    return value;
}

void write_value(int value, int virtual_address, int page_mask, int page_table_loc, int* physical_memory){
    unsigned int page_number = virtual_address & page_mask; // getting the page number out of the virtual address w/ page_mask
    int physical_address = page_table_loc + page_number; //gather our physical address of the value
    physical_memory[physical_address] = value; // since we are already given the physical memory, we just the value in the physical_memory at location physical_address
}




