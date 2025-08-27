#include "cpu.h"
#include <stdio.h>

// Okuma ve yazma fonksiyonları prototipini kullanıyoruz
extern uint8_t cpu_read(CPU *cpu, uint16_t addr);
extern void cpu_write(CPU *cpu, uint16_t addr, uint8_t value);

// --- LDA (Load Accumulator) ---
void lda_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    cpu->A = value;

    // Önce Z ve N flaglerini temizle
    cpu->P &= ~(0x02 | 0x80); // Zero ve Negative bayraklarını temizle

    if (cpu->A == 0) cpu->P |= 0x02;     // Zero
    if (cpu->A & 0x80) cpu->P |= 0x80;   // Negative

    printf("LDA #$%02X → A = %02X\n", value, cpu->A);
}


void lda_zeropage(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu->A = cpu_read(cpu, addr);
    cpu->P = (cpu->A == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->A & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("LDA $%02X → A = %02X\n", addr, cpu->A);
}

void lda_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;

    cpu->A = cpu_read(cpu, addr);

    // Zero flag
    if (cpu->A == 0) cpu->P |= 0x02; // FLAG_Z = 0x02
    else cpu->P &= ~0x02;

    // Negative flag
    if (cpu->A & 0x80) cpu->P |= 0x80; // FLAG_N = 0x80
    else cpu->P &= ~0x80;

    printf("LDA $%04X → A = %02X\n", addr, cpu->A);
}


// --- LDX (Load X Register) ---
void ldx_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    cpu->X = value;
    cpu->P = (cpu->X == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->X & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("LDX #$%02X → X = %02X\n", value, cpu->X);
}

void ldx_zeropage(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu->X = cpu_read(cpu, addr);
    cpu->P = (cpu->X == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->X & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("LDX $%02X → X = %02X\n", addr, cpu->X);
}

void ldx_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC++) | (cpu_read(cpu, cpu->PC++) << 8);
    cpu->X = cpu_read(cpu, addr);
    cpu->P = (cpu->X == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->X & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("LDX $%04X → X = %02X\n", addr, cpu->X);
}

// --- LDY (Load Y Register) ---
void ldy_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    cpu->Y = value;
    cpu->P = (cpu->Y == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->Y & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("LDY #$%02X → Y = %02X\n", value, cpu->Y);
}

void ldy_zeropage(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu->Y = cpu_read(cpu, addr);
    cpu->P = (cpu->Y == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->Y & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("LDY $%02X → Y = %02X\n", addr, cpu->Y);
}

void ldy_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC++) | (cpu_read(cpu, cpu->PC++) << 8);
    cpu->Y = cpu_read(cpu, addr);
    cpu->P = (cpu->Y == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->Y & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("LDY $%04X → Y = %02X\n", addr, cpu->Y);
}

// --- STA (Store Accumulator) ---
void sta_zeropage(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu_write(cpu, addr, cpu->A);
    printf("STA $%02X ← A = %02X\n", addr, cpu->A);
}

void sta_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC++) | (cpu_read(cpu, cpu->PC++) << 8);
    cpu_write(cpu, addr, cpu->A);
    printf("STA $%04X ← A = %02X\n", addr, cpu->A);
}

// --- STX (Store X Register) ---
void stx_zeropage(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu_write(cpu, addr, cpu->X);
    printf("STX $%02X ← X = %02X\n", addr, cpu->X);
}

void stx_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC++) | (cpu_read(cpu, cpu->PC++) << 8);
    cpu_write(cpu, addr, cpu->X);
    printf("STX $%04X ← X = %02X\n", addr, cpu->X);
}

// --- STY (Store Y Register) ---
void sty_zeropage(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    cpu_write(cpu, addr, cpu->Y);
    printf("STY $%02X ← Y = %02X\n", addr, cpu->Y);
}

void sty_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC++) | (cpu_read(cpu, cpu->PC++) << 8);
    cpu_write(cpu, addr, cpu->Y);
    printf("STY $%04X ← Y = %02X\n", addr, cpu->Y);
}

// --- Transfer Register Commands ---

void tax(CPU *cpu) {
    cpu->X = cpu->A;
    cpu->P = (cpu->X == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->X & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("TAX → X = %02X\n", cpu->X);
}

void tay(CPU *cpu) {
    cpu->Y = cpu->A;
    cpu->P = (cpu->Y == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->Y & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("TAY → Y = %02X\n", cpu->Y);
}

void txa(CPU *cpu) {
    cpu->A = cpu->X;
    cpu->P = (cpu->A == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->A & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("TXA → A = %02X\n", cpu->A);
}

void tya(CPU *cpu) {
    cpu->A = cpu->Y;
    cpu->P = (cpu->A == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->A & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("TYA → A = %02X\n", cpu->A);
}

void tsx(CPU *cpu) {
    cpu->X = cpu->SP;
    cpu->P = (cpu->X == 0 ? (cpu->P | 0x02) : (cpu->P & ~0x02));
    cpu->P = (cpu->X & 0x80 ? (cpu->P | 0x80) : (cpu->P & ~0x80));
    printf("TSX → X = %02X\n", cpu->X);
}

void txs(CPU *cpu) {
    cpu->SP = cpu->X;
    printf("TXS → SP = %02X\n", cpu->SP);
}
