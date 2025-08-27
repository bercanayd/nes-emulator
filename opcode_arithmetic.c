/// opcode_arithmetic.c



#include "cpu.h"
#include <stdio.h>

#define FLAG_C 0x01
#define FLAG_Z 0x02
#define FLAG_V 0x40
#define FLAG_N 0x80

void adc_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    uint16_t sum = cpu->A + value + (cpu->P & FLAG_C ? 1 : 0);

    // Carry flag
    if (sum > 0xFF) cpu->P |= FLAG_C;
    else cpu->P &= ~FLAG_C;

    // Overflow flag: signed overflow kontrolü
    if (~(cpu->A ^ value) & (cpu->A ^ sum) & 0x80) cpu->P |= FLAG_V;
    else cpu->P &= ~FLAG_V;

    cpu->A = sum & 0xFF;

    // Zero flag
    if (cpu->A == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    // Negative flag
    if (cpu->A & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("ADC #%02X → A = %02X\n", value, cpu->A);
}

void sbc_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    uint16_t diff = cpu->A - value - (!(cpu->P & FLAG_C) ? 1 : 0);

    // Carry flag (borrow inverted)
    if (diff <= 0xFF) cpu->P |= FLAG_C;
    else cpu->P &= ~FLAG_C;

    // Overflow flag
    if (((cpu->A ^ diff) & (cpu->A ^ value) & 0x80) != 0)
        cpu->P |= FLAG_V;
    else
        cpu->P &= ~FLAG_V;

    cpu->A = diff & 0xFF;

    // Zero flag
    if (cpu->A == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    // Negative flag
    if (cpu->A & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("SBC #%02X → A = %02X\n", value, cpu->A);
}


void inc_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t value = cpu_read(cpu, addr) + 1;
    cpu_write(cpu, addr, value);

    // Flags
    if (value == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    if (value & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("INC $%02X → %02X\n", addr, value);
}
void inx(CPU *cpu) {
    cpu->X += 1;

    if (cpu->X == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    if (cpu->X & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("INX → X = %02X\n", cpu->X);
}



void dey(CPU *cpu) {
    cpu->Y -= 1;

    if (cpu->Y == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    if (cpu->Y & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("DEY → Y = %02X\n", cpu->Y);
}

void dex(CPU *cpu) {
    cpu->X = (uint8_t)(cpu->X - 1);  // unsigned wrap-around işlemi

    if (cpu->X == 0) {
        cpu->P |= FLAG_Z;
    } else {
        cpu->P &= ~FLAG_Z;
    }

    // Negative flag (N) ayarlanmalı
    if (cpu->X & 0x80)
        cpu->P |= FLAG_N;
    else
        cpu->P &= ~FLAG_N;

    printf("DEX executed: X = 0x%02X, P = 0x%02X (Z flag %s)\n",
           cpu->X, cpu->P,
           (cpu->P & FLAG_Z) ? "set" : "clear");
}



void iny(CPU *cpu) {
    cpu->Y += 1;

    if (cpu->Y == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    if (cpu->Y & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("INY → Y = %02X\n", cpu->Y);
}

void dec_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t value = cpu_read(cpu, addr) - 1;
    cpu_write(cpu, addr, value);

    // Flags
    if (value == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    if (value & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("DEC $%02X → %02X\n", addr, value);
}

void dec_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC++) | (cpu_read(cpu, cpu->PC++) << 8);
    uint8_t value = cpu_read(cpu, addr) - 1;
    cpu_write(cpu, addr, value);

    // Flags
    if (value == 0) cpu->P |= FLAG_Z;
    else cpu->P &= ~FLAG_Z;

    if (value & 0x80) cpu->P |= FLAG_N;
    else cpu->P &= ~FLAG_N;

    printf("DEC $%04X → %02X\n", addr, value);
}


void asl_accumulator(CPU *cpu) {
    uint8_t old = cpu->A;
    cpu->P = (old & 0x80) ? (cpu->P | FLAG_C) : (cpu->P & ~FLAG_C);
    cpu->A <<= 1;

    if (cpu->A == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (cpu->A & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("ASL A: %02X → %02X\n", old, cpu->A);
}

void asl_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t val = cpu_read(cpu, addr);
    cpu->P = (val & 0x80) ? (cpu->P | FLAG_C) : (cpu->P & ~FLAG_C);
    val <<= 1;
    cpu_write(cpu, addr, val);

    if (val == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (val & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("ASL $%02X → %02X\n", addr, val);
}


void lsr_accumulator(CPU *cpu) {
    uint8_t old = cpu->A;
    cpu->P = (old & 0x01) ? (cpu->P | FLAG_C) : (cpu->P & ~FLAG_C);
    cpu->A >>= 1;

    if (cpu->A == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    cpu->P &= ~FLAG_N;  // En yüksek bit sıfır olur

    printf("LSR A: %02X → %02X\n", old, cpu->A);
}

void lsr_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t val = cpu_read(cpu, addr);
    cpu->P = (val & 0x01) ? (cpu->P | FLAG_C) : (cpu->P & ~FLAG_C);
    val >>= 1;
    cpu_write(cpu, addr, val);

    if (val == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    cpu->P &= ~FLAG_N;

    printf("LSR $%02X → %02X\n", addr, val);
}


void ror_accumulator(CPU *cpu) {
    uint8_t old = cpu->A;
    uint8_t carry_in = (cpu->P & FLAG_C) ? 0x80 : 0;
    uint8_t carry_out = (old & 0x01) ? 1 : 0;

    cpu->A = (old >> 1) | carry_in;

    if (carry_out) cpu->P |= FLAG_C; else cpu->P &= ~FLAG_C;
    if (cpu->A == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    cpu->P &= ~FLAG_N;
    if (cpu->A & 0x80) cpu->P |= FLAG_N;

    printf("ROR A: %02X → %02X\n", old, cpu->A);
}



void cmp_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    uint8_t result = cpu->A - value;

    if (cpu->A >= value) cpu->P |= FLAG_C; else cpu->P &= ~FLAG_C;
    if (result == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (result & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("CMP #%02X → A = %02X\n", value, cpu->A);
}

void cpx_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    uint8_t result = cpu->X - value;

    if (cpu->X >= value) cpu->P |= FLAG_C; else cpu->P &= ~FLAG_C;
    if (result == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (result & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("CPX #%02X → X = %02X\n", value, cpu->X);
}

void cpy_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    uint8_t result = cpu->Y - value;

    if (cpu->Y >= value) cpu->P |= FLAG_C; else cpu->P &= ~FLAG_C;
    if (result == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (result & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("CPY #%02X → Y = %02X\n", value, cpu->Y);
}
void rol_accumulator(CPU *cpu) {
    uint8_t old = cpu->A;
    uint8_t carry_in = (cpu->P & FLAG_C) ? 1 : 0;
    uint8_t carry_out = (old & 0x80) ? 1 : 0;

    cpu->A = (old << 1) | carry_in;

    if (carry_out) cpu->P |= FLAG_C; else cpu->P &= ~FLAG_C;
    if (cpu->A == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (cpu->A & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("ROL A: %02X → %02X\n", old, cpu->A);
}

void rol_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t val = cpu_read(cpu, addr);
    uint8_t carry_in = (cpu->P & FLAG_C) ? 1 : 0;
    uint8_t carry_out = (val & 0x80) ? 1 : 0;

    val = (val << 1) | carry_in;
    cpu_write(cpu, addr, val);

    if (carry_out) cpu->P |= FLAG_C; else cpu->P &= ~FLAG_C;
    if (val == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (val & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("ROL $%02X → %02X\n", addr, val);
}

void ror_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t val = cpu_read(cpu, addr);
    uint8_t carry_in = (cpu->P & FLAG_C) ? 0x80 : 0;
    uint8_t carry_out = (val & 0x01) ? 1 : 0;

    val = (val >> 1) | carry_in;
    cpu_write(cpu, addr, val);

    if (carry_out) cpu->P |= FLAG_C; else cpu->P &= ~FLAG_C;
    if (val == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (val & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("ROR $%02X → %02X\n", addr, val);
}

void lda_absolute_x(CPU *cpu) {
    uint16_t base = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base + cpu->X;
    cpu->A = cpu_read(cpu, addr);

    // Zero and Negative flags
    if (cpu->A == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (cpu->A & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("LDA $%04X,X → A = %02X\n", addr, cpu->A);
}

void lda(CPU *cpu, uint16_t addr) {
    cpu->A = cpu_read(cpu, addr);

    // Zero and Negative flags
    if (cpu->A == 0) cpu->P |= FLAG_Z; else cpu->P &= ~FLAG_Z;
    if (cpu->A & 0x80) cpu->P |= FLAG_N; else cpu->P &= ~FLAG_N;

    printf("LDA $%04X → A = %02X\n", addr, cpu->A);
}