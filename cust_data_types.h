#ifndef CUST_DATA_TYPES
#define CUST_DATA_TYPES

#define PG_LEN 256
#define PG_NUM 256
#define TLB_ENT 16
#define FRM_LEN 256
#define FRM_NUM 256
#define MEM_LEN 65536

#include "vmmgr.h"

/* frame */
typedef struct {
    int len;
    BYTE* arr;
} frame;

frame* init_frame(FILE* fptr, int frame_no) {
    frame* f = malloc(sizeof(frame));
    f->len = FRM_LEN;
    f->arr = malloc(sizeof(BYTE)*FRM_LEN);

    fseek(fptr, frame_no*FRM_LEN, SEEK_SET);
    fread(f->arr, 1, FRM_LEN, fptr);

    return f;
}

void delete_frame(frame* f) {
    free(f->arr);
    free(f);
}

/* page */
typedef struct {
    int len;
    BYTE* arr;
} page;

page* init_page() {
    page* p = malloc(sizeof(page));
    p->len = PG_LEN;
    p->arr = malloc(sizeof(BYTE)*PG_LEN);

    return p;
}

void delete_page(page* p) {
    free(p->arr);
    free(p);
}

/* page table entry */
typedef struct {
    int ladr;
    int padr;
} pt_entry;

pt_entry* init_pt_entry(int l, int p) {
    pt_entry* pte = malloc(sizeof(pt_entry));
    pte->ladr = l;
    pte->padr = p;

    return pte;
}

void delete_pt_entry(pt_entry* pte) {
    free(pte);
}

/* page table */
typedef struct {
    int len;
    int* arr;
} page_table;

page_table* init_page_table() {
    page_table* pt = malloc(sizeof(page_table));
    pt->len = PG_NUM;
    pt->arr = malloc(sizeof(int)*PG_NUM);

    int i;
    for(i = 0; i < PG_NUM; ++i) pt->arr[i] = -1;

    return pt;
}

void insert_tbl_entry(page_table* pt, int ladr, int padr) {
    pt->arr[ladr] = padr;
}

void delete_page_table(page_table* pt) {
    free(pt->arr);
    free(pt);
}

/* tlb entry */
typedef struct {
    int ladr;
    int padr;
} tlb_entry;

tlb_entry* init_tlb_entry(int l, int p) {
    tlb_entry* te = malloc(sizeof(tlb_entry));
    te->ladr = l;
    te->padr = p;

    return te;
}

void delete_tlb_entry(tlb_entry* te) {
    free(te);
}

/* tlb */
typedef struct {
    int len;
    tlb_entry** arr;
} tlb;

tlb* init_tlb() {
    tlb* t = malloc(sizeof(tlb));
    t->len = TLB_ENT;
    t->arr = malloc(sizeof(tlb_entry)*TLB_ENT);

    int i;
    for(i = 0; i < t->len; ++i) t->arr[i] = NULL;

    return t;
}

void insert_tlb_entry(tlb* t, int l, int p) {
    printf("inserted\n");
    free(t->arr[t->len-1]);

    int i;
    for(i = t->len-1; i > 0; --i)
        t->arr[i] = t->arr[i-1];

    t->arr[0] = init_tlb_entry(l, p);
}

int is_in_tlb(int ladr, tlb* t) {
    int i;
    for(i = 0; i < t->len; ++i)
        if((t->arr[i] != NULL)&&(t->arr[i]->ladr == ladr))
            return t->arr[i]->padr;

    return -1;
}

void delete_tlb(tlb* t) {
    free(t->arr);
    free(t);
}

/* physical memory */
typedef struct {
    int len;
    int bottom;
    frame** arr;
} physical_memory;

physical_memory* init_physical_mem() {
    physical_memory* pm = malloc(sizeof(physical_memory));
    pm->len = FRM_LEN;
    pm->bottom = -1;
    pm->arr = malloc(sizeof(frame*)*FRM_LEN);

    return pm;
}

int add_physical_mem(physical_memory* pm, frame* f) {
    pm->arr[++pm->bottom] = f;
    return pm->bottom;
}

void delete_physical_mem(physical_memory* pm) {
    // Free pages
    while(pm->bottom>-1) free(pm->arr[pm->bottom--]);
    free(pm->arr);
    free(pm);
}

#endif
