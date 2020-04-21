#ifndef VMMGR
#define VMMGR

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define BITS_16 0b1111111111111111
#define BITS_08 0b11111111
typedef unsigned char BYTE;

typedef struct {
    BYTE pg_num;
    BYTE pg_offset;
} ladr;

typedef struct {
    ladr* arr;
    int len;
} addrArray;

void readFile(FILE*, addrArray*);
int checkLineNumber(FILE*);
void scrubNums(addrArray*);
addrArray* loadAdresses(char*);
void printAddresses(addrArray*);

void printAddresses(addrArray* na) {
    int i;
    for(i = 0; i < na->len; ++i) {
        printf("%u - %u\n", na->arr[i].pg_num, na->arr[i].pg_offset);
    }
}

void readFile(FILE* fptr, addrArray* na) {
    na->arr = malloc(sizeof(ladr)*checkLineNumber(fptr));
    na->len = 0;
    long tempNum;

    int ret;
    while(1) {
        ret = fscanf(fptr, "%li", &tempNum);
        tempNum = tempNum & BITS_16;
        // Page number
        na->arr[na->len].pg_num = (tempNum >> 8) & BITS_08;
        // Page offset
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

addrArray* loadAdresses(char* fp) {
    addrArray* na = malloc(sizeof(addrArray));

    FILE* fptr;
    if((fptr = fopen(fp, "r+")) == NULL) {
        printf("You cannot open this file\n");
        exit(-1);
    }

    readFile(fptr, na);
    return na;
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

#endif
