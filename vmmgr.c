/*
    28 entries in the page table
    Page size of 28 bytes
    16 entries in the TLB
    Frame size of 28 bytes
    256 frames
    Physical memory of 65,536 bytes (256 frames × 256-byte frame size)
*/

#include "vmmgr.h"

#define PG_LEN 256
#define TLB_ENT 16
#define FRM_LEN 256
#define FRM_NUM 256
#define MEM_LEN 65536

typedef struct {
    BYTE* body;
    int len;
} frame;

typedef struct {
    frame* body;
    int len;
} TLB;

typedef struct {
    int pg_num;
    int frm_num;
} PTE;

typedef struct {
    PTE* body;
    int len;
} pg_tbl;

frame* createFrame();
void deleteFrame(frame*);
int loadPage(FILE*, int, frame*);
void printFrame(frame*);

TLB* createTLB();
void deleteTLB(TLB*);

void insertPTE(TLB* tlb, int pg_num, int frm_num) {
    free(&tlb->body[tlb->len-1]);

    int i;
    for(i = tlb->len-1; i > 0; --i) {
        tlb->body[i] = tlb->body[i-1];
    }

    tlb->body[0] = malloc(sizeof(PTE));
}


int main(int argc, char const *argv[]) {
    char* test_fp = "test.txt";
    char* mem_fp = "BACKING_STORE.bin";

    adrArray* na = loadAdresses(test_fp);
    FILE* mem_fptr = fopen(mem_fp, "r");

    frame* f = createFrame();
    loadPage(mem_fptr, 1, f);

    deleteFrame(f);

    // printAddresses(na);

    return 0;
}

frame* createFrame() {
    frame* mf = malloc(sizeof(frame));
    mf->len = FRM_LEN;
    mf->body = malloc(sizeof(BYTE)*mf->len);

    return mf;
}

void deleteFrame(frame* f) {
    free(f->body);
    free(f);
}

int loadPage(FILE* fptr, int pg, frame* f) {
    if(fseek(fptr, (pg*PG_LEN), SEEK_SET) == -1) return -1;
    if(fread (f->body, 8, PG_LEN, fptr) == -1) return -1;
    return 0;
}

void printFrame(frame* fr) {
    int i;
    for (i = 0; i < fr->len; ++i) {
        printf("%u\n", fr->body[i]);
    }
}

TLB* createTLB() {
    TLB* mt = malloc(sizeof(TLB));
    mt->len = FRM_LEN;
    mt->body = malloc(sizeof(PTE)*mt->len);

    return mt;
}

void deleteTLB(TLB* tlb) {
    free(tlb->body);
    free(tlb);
}
