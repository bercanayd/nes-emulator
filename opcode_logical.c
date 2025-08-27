//opcode_logical.c
#include "cpu.h"
#include <stdio.h>
#include <stdint.h>

#define FLAG_Z 0x02
#define FLAG_N 0x80

// Zero ve Negative flaglerini güncelleyen yardımcı fonksiyon
static void update_zero_and_negative_flags(CPU *cpu, uint8_t result) {
    if (result == 0)
        cpu->P |= FLAG_Z;
    else
        cpu->P &= ~FLAG_Z;

    if (result & 0x80)
        cpu->P |= FLAG_N;
    else
        cpu->P &= ~FLAG_N;
}

// AND

void and_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND #%02X → A = %02X\n", value, cpu->A);
}

void and_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t value = cpu_read(cpu, addr);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND $%02X → A = %02X\n", addr, cpu->A);
}

void and_zero_page_x(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint8_t addr = (base + cpu->X) & 0xFF;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND $%02X,X → A = %02X\n", addr, cpu->A);
}

void and_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND $%04X → A = %02X\n", addr, cpu->A);
}

void and_absolute_x(CPU *cpu) {
    uint16_t base = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base + cpu->X;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND $%04X,X → A = %02X\n", addr, cpu->A);
}

void and_absolute_y(CPU *cpu) {
    uint16_t base = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base + cpu->Y;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND $%04X,Y → A = %02X\n", addr, cpu->A);
}

void and_indirect_x(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint8_t ptr = (base + cpu->X) & 0xFF;
    uint16_t addr = cpu_read(cpu, ptr) | (cpu_read(cpu, (ptr + 1) & 0xFF) << 8);
    uint8_t value = cpu_read(cpu, addr);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND ($%02X,X) → A = %02X\n", base, cpu->A);
}

void and_indirect_y(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint16_t ptr_addr = cpu_read(cpu, base) | (cpu_read(cpu, (base + 1) & 0xFF) << 8);
    uint16_t addr = ptr_addr + cpu->Y;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A &= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("AND ($%02X),Y → A = %02X\n", base, cpu->A);
}

// ORA

void ora_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA #%02X → A = %02X\n", value, cpu->A);
}

void ora_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t value = cpu_read(cpu, addr);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA $%02X → A = %02X\n", addr, cpu->A);
}

void ora_zero_page_x(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint8_t addr = (base + cpu->X) & 0xFF;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA $%02X,X → A = %02X\n", addr, cpu->A);
}

void ora_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA $%04X → A = %02X\n", addr, cpu->A);
}

void ora_absolute_x(CPU *cpu) {
    uint16_t base = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base + cpu->X;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA $%04X,X → A = %02X\n", addr, cpu->A);
}

void ora_absolute_y(CPU *cpu) {
    uint16_t base = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base + cpu->Y;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA $%04X,Y → A = %02X\n", addr, cpu->A);
}

void ora_indirect_x(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint8_t ptr = (base + cpu->X) & 0xFF;
    uint16_t addr = cpu_read(cpu, ptr) | (cpu_read(cpu, (ptr + 1) & 0xFF) << 8);
    uint8_t value = cpu_read(cpu, addr);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA ($%02X,X) → A = %02X\n", base, cpu->A);
}

void ora_indirect_y(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint16_t ptr_addr = cpu_read(cpu, base) | (cpu_read(cpu, (base + 1) & 0xFF) << 8);
    uint16_t addr = ptr_addr + cpu->Y;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A |= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("ORA ($%02X),Y → A = %02X\n", base, cpu->A);
}

// EOR

void eor_immediate(CPU *cpu) {
    uint8_t value = cpu_read(cpu, cpu->PC++);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR #%02X → A = %02X\n", value, cpu->A);
}

void eor_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t value = cpu_read(cpu, addr);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR $%02X → A = %02X\n", addr, cpu->A);
}

void eor_zero_page_x(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint8_t addr = (base + cpu->X) & 0xFF;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR $%02X,X → A = %02X\n", addr, cpu->A);
}

void eor_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR $%04X → A = %02X\n", addr, cpu->A);
}

void eor_absolute_x(CPU *cpu) {
    uint16_t base = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base + cpu->X;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR $%04X,X → A = %02X\n", addr, cpu->A);
}

void eor_absolute_y(CPU *cpu) {
    uint16_t base = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint16_t addr = base + cpu->Y;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR $%04X,Y → A = %02X\n", addr, cpu->A);
}

void eor_indirect_x(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint8_t ptr = (base + cpu->X) & 0xFF;
    uint16_t addr = cpu_read(cpu, ptr) | (cpu_read(cpu, (ptr + 1) & 0xFF) << 8);
    uint8_t value = cpu_read(cpu, addr);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR ($%02X,X) → A = %02X\n", base, cpu->A);
}

void eor_indirect_y(CPU *cpu) {
    uint8_t base = cpu_read(cpu, cpu->PC++);
    uint16_t ptr_addr = cpu_read(cpu, base) | (cpu_read(cpu, (base + 1) & 0xFF) << 8);
    uint16_t addr = ptr_addr + cpu->Y;
    uint8_t value = cpu_read(cpu, addr);
    cpu->A ^= value;
    update_zero_and_negative_flags(cpu, cpu->A);
    printf("EOR ($%02X),Y → A = %02X\n", base, cpu->A);
}

// BIT komutu

void bit_zero_page(CPU *cpu) {
    uint8_t addr = cpu_read(cpu, cpu->PC++);
    uint8_t value = cpu_read(cpu, addr);
    uint8_t result = cpu->A & value;

    if (result == 0)
        cpu->P |= FLAG_Z;
    else
        cpu->P &= ~FLAG_Z;

    // Negative flag bit 7'den (bit 7)
    if (value & 0x80)
        cpu->P |= FLAG_N;
    else
        cpu->P &= ~FLAG_N;

    // Overflow flag bit 6'dan (bit 6)
    if (value & 0x40)
        cpu->P |= 0x40;
    else
        cpu->P &= ~0x40;

    printf("BIT $%02X → A = %02X\n", addr, cpu->A);
}

void bit_absolute(CPU *cpu) {
    uint16_t addr = cpu_read(cpu, cpu->PC) | (cpu_read(cpu, cpu->PC + 1) << 8);
    cpu->PC += 2;
    uint8_t value = cpu_read(cpu, addr);
    uint8_t result = cpu->A & value;

    if (result == 0)
        cpu->P |= FLAG_Z;
    else
        cpu->P &= ~FLAG_Z;

    if (value & 0x80)
        cpu->P |= FLAG_N;
    else
        cpu->P &= ~FLAG_N;

    if (value & 0x40)
        cpu->P |= 0x40;
    else
        cpu->P &= ~0x40;

    printf("BIT $%04X → A = %02X\n", addr, cpu->A);
}
