/*
    28 entries in the page table
    Page size of 28 bytes
    16 entries in the TLB
    Frame size of 28 bytes
    256 frames
    Physical memory of 65,536 bytes (256 frames × 256-byte frame size)
*/

#include "vmmgr.h"
#include "cust_data_types.h"

/*
mode of operation

Get value
Extract frame number and offset
If not in page table, add to page table
- Add page/frame pair
- Pull data from BACKING_STORE.bin
- Save data to physical memory
- Save page number and frame number to page table
If in page table
- Get frame number
- Pull from memory
*/

int find_padr(FILE*, page_table*, physical_memory*, tlb*, int);
void run_sim(FILE*, page_table*, physical_memory*, tlb*, addrArray*);

int main(int argc, char const *argv[]) {
    char* test_fp = "test.txt";
    char* mem_fp = "BACKING_STORE.bin";
    FILE* fptr = fopen(mem_fp, "r+");

    /* find addresses */
    addrArray* na = loadAdresses(test_fp);
    /*printAddresses(na);*/

    /* create table */
    page_table* pt = init_page_table();
    physical_memory* pm = init_physical_mem();
    tlb* t = init_tlb();

    run_sim(fptr, pt, pm, t, na);

    /* free memory */
    delete_page_table(pt);
    delete_physical_mem(pm);
    delete_tlb(t);

    return 0;
}

void run_sim(FILE* fptr, page_table* pt, physical_memory* pm, tlb* t, addrArray* na) {
    int i, padr, offset, value;
    for(i = 0; i < na->len; ++i) {
        padr = find_padr(fptr, pt, pm, t, na->arr[i].pg_num);
        offset = na->arr[i].pg_offset;

        value = pm->arr[padr]->arr[offset];
        printf("%d\n", value);
    }
}

int find_padr(FILE* fptr, page_table* pt, physical_memory* pm, tlb* t, int curr) {
    int r = is_in_tlb(curr, t);

    if(r != -1) return r;

    if(pt->arr[curr] == -1) {
        frame* f = init_frame(fptr, curr);
        int b = add_physical_mem(pm, f);
        insert_tbl_entry(pt, curr, b);
    }

    insert_tlb_entry(t, curr, pt->arr[curr]);

    return pt->arr[curr];
}
