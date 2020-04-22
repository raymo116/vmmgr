/*
* @name: cust_data_types.c
* @author: Matt Raymond
* @email: raymo116@mail.chapman.edu
* @id: 2270559
* @date: 04/21/2020
* @version: 1.0
*/

#ifndef CUST_DATA_TYPES
#define CUST_DATA_TYPES

/* constants */
#define PG_LEN 256
#define PG_NUM 256
#define TLB_ENT 16
#define FRM_LEN 256

/* importing other library I wrote */
#include "vmmgr.h"

/* redefine unsigned char as a byte to make the logic more straightforward */
typedef unsigned char BYTE;

/*  -----  */
/* |FRAME| */
/*  -----  */

/*
 * Since a page and a frame are essentially the same (just in different
 * locations, I didn't bother redefining it)
 */

/* frame struct */
typedef struct {
    int len;
    BYTE* arr;
} frame;

/* initialize the frame */
frame* init_frame(FILE* fptr, int frame_no) {
    frame* f = malloc(sizeof(frame));
    f->len = FRM_LEN;
    f->arr = malloc(sizeof(BYTE)*FRM_LEN);

    fseek(fptr, frame_no*FRM_LEN, SEEK_SET);
    fread(f->arr, 1, FRM_LEN, fptr);

    return f;
}

/* free memory from frame */
void delete_frame(frame* f) {
    free(f->arr);
    free(f);
}



/*  ----------------  */
/* |PAGE TABLE ENTRY| */
/*  ----------------  */

/* page table entry */
typedef struct {
    int ladr;
    int padr;
} pt_entry;

/* initializes values in page table entry */
pt_entry* init_pt_entry(int l, int p) {
    pt_entry* pte = malloc(sizeof(pt_entry));

    pte->ladr = l;
    pte->padr = p;

    return pte;
}

/* frees memory from page table entry */
void delete_pt_entry(pt_entry* pte) {
    free(pte);
}



/*  ----------  */
/* |PAGE TABLE| */
/*  ----------  */

/* page table */
typedef struct {
    int len;
    int* arr;
} page_table;

/* initializes values in the page table */
page_table* init_page_table() {
    page_table* pt = malloc(sizeof(page_table));
    pt->arr = malloc(sizeof(int)*PG_NUM);
    pt->len = PG_NUM;

    /* initializes all values to -1  */
    int i;
    for(i = 0; i < PG_NUM; ++i) pt->arr[i] = -1;

    return pt;
}

/* inserts an entry into the table */
void insert_tbl_entry(page_table* pt, int ladr, int padr) {
    /* maps a physical address to a logical address */
    pt->arr[ladr] = padr;
}

/* frees memory */
void delete_page_table(page_table* pt) {
    free(pt->arr);
    free(pt);
}



/*  ---------  */
/* |TLB ENTRY| */
/*  ---------  */

/* tlb entry */
typedef struct {
    int ladr;
    int padr;
} tlb_entry;

/* Initializes a tlb entry*/
tlb_entry* init_tlb_entry(int l, int p) {
    tlb_entry* te = malloc(sizeof(tlb_entry));
    te->ladr = l;
    te->padr = p;

    return te;
}

/* frees memory */
void delete_tlb_entry(tlb_entry* te) {
    free(te);
}



/*  ---  */
/* |TLB| */
/*  ---  */

/* tlb */
typedef struct {
    int len;
    tlb_entry** arr;
} tlb;

/* initializes tlb */
tlb* init_tlb() {
    tlb* t = malloc(sizeof(tlb));
    t->arr = malloc(sizeof(tlb_entry)*TLB_ENT);
    t->len = TLB_ENT;

    /* initializs to -1 */
    int i;
    for(i = 0; i < t->len; ++i) t->arr[i] = NULL;

    return t;
}

/* inserts a value into the tlb as a fifo data structure */
void insert_tlb_entry(tlb* t, int l, int p) {
    /* deletes the last one */
    free(t->arr[t->len-1]);

    /* shifts all others down by one */
    int i;
    for(i = t->len-1; i > 0; --i)
        t->arr[i] = t->arr[i-1];

    /* adds one to the top of the list */
    t->arr[0] = init_tlb_entry(l, p);
}

/* checks to see if an address is in the tlb */
int is_in_tlb(int ladr, tlb* t) {
    int i;
    for(i = 0; i < t->len; ++i)
        if((t->arr[i] != NULL)&&(t->arr[i]->ladr == ladr))
            return t->arr[i]->padr;

    return -1;
}

/* free memory */
void delete_tlb(tlb* t) {
    free(t->arr);
    free(t);
}



/*  ---------------  */
/* |PHYSICAL MEMORY| */
/*  ---------------  */

/* physical memory */
typedef struct {
    int len;
    int bottom;
    frame** arr;
} physical_memory;

/* initializes physical memory */
physical_memory* init_physical_mem() {
    physical_memory* pm = malloc(sizeof(physical_memory));
    pm->len = FRM_LEN;
    pm->bottom = -1;
    pm->arr = malloc(sizeof(frame*)*FRM_LEN);

    return pm;
}

/* adds a new frame to physical memory */
int add_physical_mem(physical_memory* pm, frame* f) {
    pm->arr[++pm->bottom] = f;
    return pm->bottom;
}

/* free memory */
void delete_physical_mem(physical_memory* pm) {
    /* Free pages */
    while(pm->bottom>-1) free(pm->arr[pm->bottom--]);
    free(pm->arr);
    free(pm);
}

#endif
