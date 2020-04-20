/*
    28 entries in the page table
    Page size of 28 bytes
    16 entries in the TLB
    Frame size of 28 bytes
    256 frames
    Physical memory of 65,536 bytes (256 frames × 256-byte frame size)
*/

#include "vmmgr.h"

int main(int argc, char const *argv[]) {
    char* fp = "test.txt";
    adrArray* na = loadAdresses(fp);

    printAddresses(na);

    return 0;
}
