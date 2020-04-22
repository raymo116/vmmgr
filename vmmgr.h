/*
* @name: vmmgr.h
* @author: Matt Raymond
* @email: raymo116@mail.chapman.edu
* @id: 2270559
* @date: 04/21/2020
* @version: 1.0
*/

#ifndef VMMGR
#define VMMGR

/* imports */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "cust_data_types.h"

/* defining masks */
#define BITS_16 0b1111111111111111
#define BITS_08 0b11111111

/* defining a logical address datatype */
typedef struct {
    BYTE pg_num;
    BYTE pg_offset;
} ladr;

/* defining an array of addresses */
typedef struct {
    ladr* arr;
    int len;
} addrArray;

/* check to see how many lines there are */
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

    /* go back to the begining of the file */
    rewind(fptr);
    return lineNumber;
}

/* read in addresses from the file */
addrArray* loadAdresses(FILE* fptr) {
    addrArray* na = malloc(sizeof(addrArray))

    /* create an array of addresses */
    na->arr = malloc(sizeof(ladr)*checkLineNumber(fptr));
    na->len = 0;
    long tempNum;

    /* read all addresses from memory */
    int ret;
    while(1) {
        /* scan info */
        ret = fscanf(fptr, "%li", &tempNum);

        /* get rid of extra bits of information */
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
        }
    }

    return na;
}

/* find the physical address of a given logical address */
int find_padr(FILE* fptr, page_table* pt, physical_memory* pm, tlb* t, int curr, int* pf, int* th) {
    /* tries to grab address from the tlb */
    int r = is_in_tlb(curr, t);

    /* if there, return the value */
    if(r != -1) {
        /* incriment tlb hit counter */
        ++(*th);
        return r;
    }

    /* if the logical address is not yet taken */
    if(pt->arr[curr] == -1) {
        /* count as a page fault */
        ++(*pf);

        /* create a new frame */
        frame* f = init_frame(fptr, curr);
        /* add the frame to physical memory */
        int b = add_physical_mem(pm, f);
        /* add as a tbl entry */
        insert_tbl_entry(pt, curr, b);
    }

    /* if it is in the table, add as a tbl entry */
    insert_tlb_entry(t, curr, pt->arr[curr]);

    return pt->arr[curr];
}

/* run a memory-referencing simulation */
void run_sim(char* mem, char* output, page_table* pt, physical_memory* pm, tlb* t, addrArray* na) {
    /* open files */
    FILE* output_fptr = fopen(output, "w");
    FILE* input_fptr = fopen(mem, "r");

    /* checks for correct permissions */
    if((output_fptr == NULL) || (input_fptr == NULL)) {
        printf("You don't have permission to open this file\n");
        exit(-1);
    }

    /* keeps track of page faults and tlb hits */
    int page_faults = 0;
    int tlb_hits = 0;

    /* creates file header */
    fprintf(output_fptr, "Logical Addr,Physical Addr,Offset,Value\n");

    /* tries to grab every address in the address file */
    int i, padr, offset, value;
    for(i = 0; i < na->len; ++i) {
        /* gets the physical address */
        padr = find_padr(input_fptr, pt, pm, t, na->arr[i].pg_num, &page_faults, &tlb_hits);

        /* gets the offset */
        offset = na->arr[i].pg_offset;

        /* finds the value at that address */
        value = pm->arr[padr]->arr[offset];

        /* saves the result to a file */
        fprintf(output_fptr, "%d,%d,%d,%d\n", na->arr[i].pg_num, padr, offset, value);
    }

    /* adds collected stats to the end of the csv file */
    fprintf(output_fptr, "\n,,,\nPage Faults,TLB Hits\n%d%%,%d%%\n", (page_faults*100)/na->len, (tlb_hits*100)/na->len);

    /* close files */
    fclose(output_fptr);
    fclose(input_fptr);
}

#endif
