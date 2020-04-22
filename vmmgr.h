#ifndef VMMGR
#define VMMGR

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "cust_data_types.h"

#define BITS_16 0b1111111111111111
#define BITS_08 0b11111111

typedef struct {
    BYTE pg_num;
    BYTE pg_offset;
} ladr;

typedef struct {
    ladr* arr;
    int len;
} addrArray;

void printAddresses(addrArray* na) {
    int i;
    for(i = 0; i < na->len; ++i) {
        printf("%u - %u\n", na->arr[i].pg_num, na->arr[i].pg_offset);
    }
}

int checkLineNumber(FILE* fptr) {
    long num;
    int lineNumber = 0;

    int ret;
    while(1) {
        ret = fscanf(fptr, "%li", &num);
        if(ret == 1)
            ++lineNumber;
        else if(errno != 0) {
            perror("scanf:");
            break;
        } else if(ret == EOF) {
            break;
        } else
            printf("no match\n");
    }
    rewind(fptr);
    return lineNumber;
}

void readFile(FILE* fptr, addrArray* na) {
    na->arr = malloc(sizeof(ladr)*checkLineNumber(fptr));
    na->len = 0;
    long tempNum;

    int ret;
    while(1) {
        ret = fscanf(fptr, "%li", &tempNum);
        tempNum = tempNum & BITS_16;
        /* Page number */
        na->arr[na->len].pg_num = (tempNum >> 8) & BITS_08;
        /* Page offset */
        na->arr[na->len].pg_offset = tempNum & BITS_08;
        if(ret == 1) {
            ++na->len;
        }
        else if(errno != 0) {
            perror("scanf:");
            break;
        } else if(ret == EOF) {
            break;
        } else
            printf("no match\n");
    }
}

addrArray* loadAdresses(FILE* fptr) {
    addrArray* na = malloc(sizeof(addrArray));

    readFile(fptr, na);
    return na;
}

int find_padr(FILE* fptr, page_table* pt, physical_memory* pm, tlb* t, int curr, int* pf, int* th) {
    int r = is_in_tlb(curr, t);

    if(r != -1) {
        ++(*th);
        return r;
    }

    if(pt->arr[curr] == -1) {
        ++(*pf);
        frame* f = init_frame(fptr, curr);
        int b = add_physical_mem(pm, f);
        insert_tbl_entry(pt, curr, b);
    }

    insert_tlb_entry(t, curr, pt->arr[curr]);

    return pt->arr[curr];
}

void run_sim(char* mem, char* output, page_table* pt, physical_memory* pm, tlb* t, addrArray* na) {
    FILE* output_fptr = fopen(output, "w+");
    FILE* input_fptr = fopen(mem, "r+");

    if((output_fptr == NULL) || (input_fptr == NULL)) {
        printf("You don't have permission to open this file\n");
        exit(-1);
    }

    int page_faults = 0;
    int tlb_hits = 0;

    fprintf(output_fptr, "Logical Addr,Physical Addr,Offset,Value\n");
    int i, padr, offset, value;
    for(i = 0; i < na->len; ++i) {
        padr = find_padr(input_fptr, pt, pm, t, na->arr[i].pg_num, &page_faults, &tlb_hits);
        offset = na->arr[i].pg_offset;

        value = pm->arr[padr]->arr[offset];
        fprintf(output_fptr, "%d,%d,%d,%d\n", na->arr[i].pg_num, padr, offset, value);
    }

    fprintf(output_fptr, "\n,,,\nPage Faults,TLB Hits\n%d%%,%d%%\n", (page_faults*100)/na->len, (tlb_hits*100)/na->len);

    fclose(output_fptr);
    fclose(input_fptr);
}

#endif
