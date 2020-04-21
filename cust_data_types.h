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

frame* init_frame() {
    frame* f = malloc(sizeof(frame));
    f->len = FRM_LEN;
    f->arr = malloc(sizeof(BYTE)*FRM_LEN);

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
    int len;
    frame* arr;
} pt_entry;

pt_entry* init_pt_entry() {
    pt_entry* pte = malloc(sizeof(pt_entry));
    pte->len = PG_LEN;
    pte->arr = malloc(sizeof(BYTE)*PG_LEN);

    return pte;
}

void delete_pt_entry(pt_entry* pte) {
    free(pte->arr);
    free(pte);
}

/* page table */
typedef struct {
    int len;
    pt_entry* arr;
} page_table;

page_table* init_page_table() {
    page_table* pt = malloc(sizeof(page_table));
    pt->len = PG_NUM;
    pt->arr = malloc(sizeof(pt_entry)*PG_NUM);

    int i;
    for(i = 0; i < PG_NUM; ++i) pt->arr = NULL;

    return pt;
}

void delete_page_table(page_table* pt) {
    free(pt->arr);
    free(pt);
}

/* tlb entry */
typedef struct {
    int p_num;
    int f_num;
} tlb_entry;

tlb_entry* init_tlb_entry(int p, int f) {
    tlb_entry* te = malloc(sizeof(tlb_entry));
    te->p_num = p;
    te->f_num = f;

    return te;
}

void delete_tlb_entry(tlb_entry* te) {
    free(te);
}

/* tlb */
typedef struct {
    int len;
    tlb_entry* arr;
} tlb;

tlb* init_tlb() {
    tlb* t = malloc(sizeof(tlb));
    t->len = TLB_ENT;
    t->arr = malloc(sizeof(tlb_entry)*TLB_ENT);
    return t;
}

void delete_tlb(tlb* t) {
    free(t->arr);
    free(t);
}

/* physical memory */
typedef struct {
    int len;
    int bottom;
    page* arr;
} pysical_memory;

pysical_memory* init_physical_mem() {
    pysical_memory* pm = malloc(sizeof(pysical_memory));
    pm->len = FRM_LEN;
    pm->bottom = 0;
    pm->arr = malloc(sizeof(frame)*FRM_LEN);

    return pm;
}

void delete_physical_mem(pysical_memory* pm) {
    free(pm->arr);
    free(pm);
}

#endif
