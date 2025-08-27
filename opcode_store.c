#include "cpu.h"
#include <stdio.h>

// STA Opcodes
void sta_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu_write(cpu, addr, cpu->A);
    printf("STA $%02X ← A=%02X\n", addr, cpu->A);
}

void sta_zero_page_x(CPU *cpu) {
    uint8_t base_addr = cpu_read(cpu, cpu->PC++);
    uint8_t addr = (base_addr + cpu->X) & 0xFF; // wrap zero page
    cpu_write(cpu, addr, cpu->A);
    printf("STA $%02X,X ← A=%02X\n", addr, cpu->A);
}


void sta_absolute_x(CPU *cpu){
    uint16_t base_addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base_addr + cpu->X;
    cpu_write(cpu, addr, cpu->A);
    printf("STA $%04X,X ← A=%02X\n", addr, cpu->A);
}

void sta_absolute_y(CPU *cpu) {
    uint16_t base_addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base_addr + cpu->Y;
    cpu_write(cpu, addr, cpu->A);
    printf("STA $%04X,Y ← A=%02X\n", addr, cpu->A);
}



// STX Opcodes
void stx_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu_write(cpu, addr, cpu->X);
    printf("STX $%02X ← X=%02X\n", addr, cpu->X);
}

void stx_zero_page_y(CPU *cpu) { // STX Zero Page,Y (Opcode 0x96)
    uint8_t base_addr = cpu_read(cpu, cpu->PC++);
    uint8_t addr = (base_addr + cpu->Y) & 0xFF;
    cpu_write(cpu, addr, cpu->X);
    printf("STX $%02X,Y ← X=%02X\n", addr, cpu->X);
}



// STY Opcodes
void sty_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu_write(cpu, addr, cpu->Y);
    printf("STY $%02X ← Y=%02X\n", addr, cpu->Y);
}

void sty_zero_page_x(CPU *cpu) { // STY Zero Page,X (Opcode 0x94)
    uint8_t base_addr = cpu_read(cpu, cpu->PC++);
    uint8_t addr = (base_addr + cpu->X) & 0xFF;
    cpu_write(cpu, addr, cpu->Y);
    printf("STY $%02X,X ← Y=%02X\n", addr, cpu->Y);
}
