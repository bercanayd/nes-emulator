// opcode_stack.c
#include "cpu.h"
#include <stdio.h>

#define FLAG_B 0x10
#define FLAG_U 0x20
#define FLAG_Z 0x02
#define FLAG_N 0x80
#define STACK_BASE 0x0100

// Pusha: Push Accumulator
void pha(CPU *cpu) {
    cpu_write(cpu, STACK_BASE + cpu->SP, cpu->A);
    cpu->SP--;
    printf("PHA → Pushed A=%02X\n", cpu->A);
}

// Pulla: Pull Accumulator
void pla(CPU *cpu) {
    cpu->SP++;
    cpu->A = cpu_read(cpu, STACK_BASE + cpu->SP);

    // Update Zero and Negative flags
    cpu->P = (cpu->P & ~(FLAG_Z | FLAG_N))
           | (cpu->A == 0 ? FLAG_Z : 0)
           | (cpu->A & 0x80 ? FLAG_N : 0);

    printf("PLA → Pulled A=%02X\n", cpu->A);
}

// Push Processor Status
void php(CPU *cpu) {
    uint8_t flags = cpu->P | FLAG_B | FLAG_U;
    cpu_write(cpu, STACK_BASE + cpu->SP, flags);
    cpu->SP--;
    printf("PHP → Pushed P=%02X\n", flags);
}

// Pull Processor Status
void plp(CPU *cpu) {
    cpu->SP++;
    cpu->P = cpu_read(cpu, STACK_BASE + cpu->SP);

    // Clear bits 4 and 5 (B and U flags) as per NES behavior
    cpu->P &= ~(FLAG_B | FLAG_U);

    printf("PLP → Pulled P=%02X\n", cpu->P);
}

// Return from Interrupt
void rti(CPU *cpu) {
    plp(cpu);  // Pull Processor Status (increments SP)
    
    uint8_t low = cpu_read(cpu, STACK_BASE + ++cpu->SP);
    uint8_t high = cpu_read(cpu, STACK_BASE + ++cpu->SP);
    cpu->PC = ((uint16_t)high << 8) | low;

    printf("RTI → PC = %04X\n", cpu->PC);
}

// Jump to Subroutine

