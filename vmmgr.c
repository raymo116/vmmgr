/*
* @name: vmmgr.c
* @author: Matt Raymond
* @email: raymo116@mail.chapman.edu
* @id: 2270559
* @date: 04/21/2020
* @version: 1.0
*/

/* import functions */
#include "vmmgr.h"

/*
 * The objective of this assignment consists of writing a C/C++ program that
 * translates logical to physical addresses for a virtual address space of size
 * 216 = 65,536 bytes. Your program will read from a file containing logical
 * addresses and, using a TLB as well as a page table, will translate each
 * logical address to it's corresponding physical address and output the value
 * of the byte stored at the translated physical address. The goal behind this
 * project is to simulate the steps involved in translating logical to physical
 * addresses.
 */
int main(int argc, char const *argv[]) {
    /* declaring file names */
    char* mem_fp = "BACKING_STORE.bin";
    char* output_fp = "output.csv";

    /* checks to make sure the input format is correct */
    if(argc != 2) {
        printf("Usage: ./vmmgr <address file>\n");
        return -1;
    }
    /* opens the file to read */
    FILE* address_fptr = fopen(argv[1], "r");
    if(address_fptr == NULL) {
        printf("You cannot open this file\n");
        exit(-1);
    }

    /* load addresses into memory */
    addrArray* na = loadAdresses(address_fptr);

    fclose(address_fptr);

    /* initialize data structures */
    page_table* pt = init_page_table();
    physical_memory* pm = init_physical_mem();
    tlb* t = init_tlb();

    /* Run the simulation */
    run_sim(mem_fp, output_fp, pt, pm, t, na);

    /* free memory */
    delete_page_table(pt);
    delete_physical_mem(pm);
    delete_tlb(t);

    return 0;
}
