//opcode_misc.c
#include "cpu.h"
#include <stdio.h>

#define FLAG_C 0x01

void nop(CPU *cpu) {
    printf("NOP\n");
}


void clc(CPU *cpu) {
    cpu->P &= ~FLAG_C;
    printf("CLC\n");
}

void sei(CPU *cpu) {
    cpu->P |= 0x04;  // I flag (bit 2)
    printf("SEI\n");
}

void sec(CPU *cpu) {
    cpu->P |= FLAG_C;
    printf("SEC\n");
}

void cld(CPU *cpu) {
    cpu->P &= ~0x08;  // D flag = Bit 3
    printf("CLD\n");
}
