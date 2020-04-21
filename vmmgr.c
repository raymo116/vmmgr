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

int main(int argc, char const *argv[]) {
    char* test_fp = "address.txt";
    char* mem_fp = "BACKING_STORE.bin";

    addrArray* na = loadAdresses(test_fp);
    printAddresses(na);

    return 0;
}
