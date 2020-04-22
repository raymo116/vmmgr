#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define SIZE_OF_BIN 65536

int main() {
    char* input_fp = "BACKING_STORE.bin";
    char* output_fp = "translated_BACKING_STORE.txt";

    FILE* input_fptr = fopen(input_fp, "r");
    FILE* output_fptr = fopen(output_fp, "w+");

    unsigned char* bytes = malloc(sizeof(unsigned char)*SIZE_OF_BIN);
    fread(bytes, 1, SIZE_OF_BIN, input_fptr);

    int i;
    for(i = 0; i < SIZE_OF_BIN; ++i) {
        fprintf(output_fptr, "%u\n", bytes[i]);
    }

    fclose(input_fptr);
    fclose(output_fptr);

    free(bytes);

    return 0;
}
