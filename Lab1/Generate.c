#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char* instructions[] = {
    "MOV AX, BX",
    "ADD AX, 2",
    "SUB BX, AX",
    "MUL DX",
    "DIV CX",
    "PUSH AX",
    "POP BX",
    "CMP AX, BX",
    "JMP 0x004",
    "CALL 0x123"
};

const char* process_names[] = {
    "ProcessA",
    "ProcessB",
    "ProcessC",
    "ProcessD",
    "ProcessE"
};

int main() {
    FILE *f = fopen("data.txt", "w");
    if (f == NULL) {
        printf("Failed to open file\n");
        return -1;
    }
    srand(time(NULL)); 
    for(int i = 0; i < 100; i++) {
        int mem_address = rand() % 65536; 
        int proc_index = rand() % 5;
        int inst_index = rand() % 10;
        fprintf(f, "0x%04X %s %s\n", mem_address, process_names[proc_index], instructions[inst_index]);
    }
    fclose(f);
    return 0;
}