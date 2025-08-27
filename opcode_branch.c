#include "cpu.h"
#include <stdio.h>
#include <stdint.h>

// Flag tanımları
#define FLAG_C 0x01  // Carry
#define FLAG_Z 0x02  // Zero
#define FLAG_V 0x40  // Overflow
#define FLAG_N 0x80  // Negative

void branch_if(CPU *cpu, int condition) {
    int8_t offset = (int8_t)cpu_read(cpu, cpu->PC++);
    printf("BNE called at PC=0x%04X, P=0x%02X\n", cpu->PC - 1, cpu->P);
    printf("Raw offset byte at PC=0x%04X: 0x%02X\n", cpu->PC - 1, (uint8_t)offset);

    if (condition) {
        uint16_t old_pc = cpu->PC;
        cpu->PC += offset;
        printf("Offset sign-extended: %d, PC incremented to 0x%04X\n", offset, cpu->PC);
        printf("BNE branch taken from 0x%04X to 0x%04X\n", old_pc, cpu->PC);
    } else {
        printf("Branch not taken\n");
    }
}


// Branch komutları

// BEQ: Zero flag set ise branch
void beq(CPU *cpu) {
    branch_if(cpu, (cpu->P & FLAG_Z) != 0);
}

// BNE: Zero flag clear ise branch
void bne(CPU *cpu) {
    int8_t offset = (int8_t)cpu_read(cpu, cpu->PC++);
    printf("BNE at PC=0x%04X, P=0x%02X (Z flag=%d), offset=%d\n", cpu->PC-1, cpu->P, (cpu->P & 0x02) != 0, offset);
    if ((cpu->P & 0x02) == 0) {
        uint16_t old_pc = cpu->PC;
        cpu->PC += offset;
        printf("Branch taken from 0x%04X to 0x%04X\n", old_pc, cpu->PC);
    } else {
        printf("Branch not taken\n");
    }
}





// BCC: Carry flag clear ise branch
void bcc(CPU *cpu) {
    branch_if(cpu, (cpu->P & FLAG_C) == 0);
}

// BCS: Carry flag set ise branch
void bcs(CPU *cpu) {
    branch_if(cpu, (cpu->P & FLAG_C) != 0);
}

// BPL: Negative flag clear ise branch (pozitif ise)
void bpl(CPU *cpu) {
    branch_if(cpu, (cpu->P & FLAG_N) == 0);
}

// BMI: Negative flag set ise branch (negatif ise)
void bmi(CPU *cpu) {
    branch_if(cpu, (cpu->P & FLAG_N) != 0);
}

// BVC: Overflow flag clear ise branch
void bvc(CPU *cpu) {
    branch_if(cpu, (cpu->P & FLAG_V) == 0);
}

// BVS: Overflow flag set ise branch
void bvs(CPU *cpu) {
    branch_if(cpu, (cpu->P & FLAG_V) != 0);
}
void jmp(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC = addr;
}

void jsr(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t return_addr = cpu->PC - 1;
    cpu_write(cpu, 0x100 + cpu->SP--, (return_addr >> 8) & 0xFF);
    cpu_write(cpu, 0x100 + cpu->SP--, return_addr & 0xFF);
    cpu->PC = addr;
}

void rts(CPU *cpu) {
    uint8_t low = cpu_read(cpu, 0x100 + ++cpu->SP);
    uint8_t high = cpu_read(cpu, 0x100 + ++cpu->SP);
    cpu->PC = ((uint16_t)high << 8) | low;
    cpu->PC++;
}
