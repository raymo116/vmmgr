#include "vmmgr.h"

int main(int argc, char const *argv[]) {
    char* mem_fp = "BACKING_STORE.bin";
    char* output_fp = "output.csv";

    // Checks to make sure the input format is correct
    if(argc != 2) {
        printf("Usage: ./vmmgr <address file>\n");
        return -1;
    }
    FILE* address_fptr = fopen(argv[1], "r");
    if(address_fptr == NULL) {
        printf("You cannot open this file\n");
        exit(-1);
    }

    /* find addresses */
    addrArray* na = loadAdresses(address_fptr);

    /* create table */
    page_table* pt = init_page_table();
    physical_memory* pm = init_physical_mem();
    tlb* t = init_tlb();

    run_sim(mem_fp, output_fp, pt, pm, t, na);

    /* free memory */
    delete_page_table(pt);
    delete_physical_mem(pm);
    delete_tlb(t);

    return 0;
}
