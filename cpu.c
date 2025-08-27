// cpu.c
#include "cpu.h"
#include <stdbool.h>
#include "ppu.h"
#include <stdio.h>
#include <stdlib.h>


//tüm opcodeları şuradan referans aldık hepsi burada yok ama tammamlandığında hepsi eklencek.
//https://www.nesdev.org/wiki/Visual6502wiki/6502_all_256_Opcodes

void cpu_handle_nmi(CPU *cpu);
void cpu_handle_irq(CPU *cpu);

void cpu_print_state(CPU *cpu) {
    printf("PC: %04X  A: %02X  X: %02X  Y: %02X  P: %02X  SP: %02X\n",
           cpu->PC, cpu->A, cpu->X, cpu->Y, cpu->P, cpu->SP);
}
uint8_t cpu_read(CPU *cpu, uint16_t addr) {
    if (addr < 0x2000) {
        // RAM (2KB mirroring)
        return cpu->memory[addr % 0x0800];
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // PPU register erişimi (mirrored her 8 byte)
        uint16_t reg = addr % 8;
        if (reg == 2) {
            // PPUSTATUS — gerçek PPU durumunu oku
         return ppu_read_status(cpu->ppu);
          
        } else {
            return ppu_read(cpu->ppu, reg); // Diğer PPU registerları
        }
    } else if (addr >= 0x8000) {
        // PRG-ROM alanı
        return cpu->memory[addr];
    } else {
        // Diğer alanlar (IO, expansion, APU, vs.)
        return 0;
    }
}


void cpu_write(CPU *cpu, uint16_t addr, uint8_t value) {
    if (addr < 0x2000) {
        // RAM
        cpu->memory[addr % 0x0800] = value;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        // PPU register yazımı
        ppu_write(cpu->ppu, addr % 8, value);
    } else if (addr >= 0x8000) {
        // ROM genellikle yazılamaz ama bazı mapper’lar destekler
        // Şimdilik ignore
    } else {
        // Diğer alanlara yazım (şimdilik yapmaylaım)
    }
}


// cpu.c içindeki cpu_reset fonksiyonunu şöyle değiştir:

void cpu_reset(CPU *cpu) {
    cpu->SP = 0xFD;
    cpu->P = 0x24;
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    // PC ayarlama kaldırıldı, main() bunu yapacak
}



void cpu_step(CPU *cpu) {
    uint8_t opcode = cpu_read(cpu, cpu->PC++);

    // --- INTERRUPT ÖNCELİĞİ ---
    if (cpu->nmi_pending) {
        cpu_handle_nmi(cpu);
        cpu->nmi_pending = false;
        return;
    } else if (cpu->irq_pending && !(cpu->P & 0x04)) {
        cpu_handle_irq(cpu);
        cpu->irq_pending = false;
        return;
    }

    printf("PC = %04X, opcode = %02X\n", cpu->PC - 1, opcode);

    switch (opcode) {
        // Tüm opcode'lar burada
        case 0xA9: lda_immediate(cpu); break;
        case 0xA2: ldx_immediate(cpu); break;
        case 0xAA: tax(cpu); break;
     
        case 0x69: adc_immediate(cpu); break;
        case 0x29: and_immediate(cpu); break;
        case 0x09: ora_immediate(cpu); break;
        case 0x49: eor_immediate(cpu); break;
        case 0x10: bpl(cpu); break;
        case 0x30: bmi(cpu); break;
        case 0x50: bvc(cpu); break;
        case 0x70: bvs(cpu); break;
        case 0x90: bcc(cpu); break;
        case 0xB0: bcs(cpu); break;
        case 0xF0: beq(cpu); break;


    
      
       
        case 0x4C: jmp(cpu); break;
        case 0x20: jsr(cpu); break;
        case 0x60: rts(cpu); break;

        case 0x48: pha(cpu); break;
        case 0x68: pla(cpu); break;
        case 0x08: php(cpu); break;
        case 0x28: plp(cpu); break;
        case 0x40: rti(cpu); break;
        case 0x8D: sta_absolute(cpu); break;
        case 0x8E: ldx_absolute(cpu); break;
        case 0xEA: nop(cpu); break;
        case 0x00: brk(cpu); break;
        case 0x78: sei(cpu); break;
        case 0xD8: cld(cpu); break;
        case 0xBD: sta_absolute_x(cpu); break;
        case 0xB9: sta_absolute_y(cpu); break;
        case 0x18: clc(cpu); break;
        case 0x38: sec(cpu); break;
        case 0x8A: txa(cpu); break;
        case 0xA8: tay(cpu); break;
        case 0x98: tya(cpu); break;
        case 0x85: sta_zero_page(cpu); break;
        case 0xBA: tsx(cpu); break;
        case 0xE0: ldy_immediate(cpu); break;
        case 0x9A: txs(cpu); break;
        case 0xA0: ldy_immediate(cpu); break;
        case 0xE6: inc_zero_page(cpu); break;
        case 0xE8: inx(cpu); break;
        case 0xC8: iny(cpu); break;
        case 0xCA: dex(cpu); break;
        case 0x88: dey(cpu); break;
        case 0x05: ora_zero_page(cpu); break;
        case 0xE9: sbc_immediate(cpu); break;
         case 0xC6: dec_zero_page(cpu); break;       // DEC zeropage
        case 0xCE: dec_absolute(cpu); break;        // DEC absolute

        // Yeni eklenen rotate fonksiyonları
        case 0x2A: rol_accumulator(cpu); break;     // ROL accumulator
        case 0x26: rol_zero_page(cpu); break;       // ROL zeropage
        case 0x6A: ror_accumulator(cpu); break;     // ROR accumulator
        case 0x66: ror_zero_page(cpu); break;       // ROR zeropage

     
        case 0x25: and_zero_page(cpu); break;
        case 0x35: and_zero_page_x(cpu); break;
        case 0x2D: and_absolute(cpu); break;
        case 0x3D: and_absolute_x(cpu); break;
        case 0x39: and_absolute_y(cpu); break;
        case 0x21: and_indirect_x(cpu); break;
        case 0x31: and_indirect_y(cpu); break;

        // ORA
        
        case 0x15: ora_zero_page_x(cpu); break;
        case 0x0D: ora_absolute(cpu); break;
        case 0x1D: ora_absolute_x(cpu); break;
        case 0x19: ora_absolute_y(cpu); break;
        case 0x01: ora_indirect_x(cpu); break;
        case 0x11: ora_indirect_y(cpu); break;

        // EOR
      
        case 0x45: eor_zero_page(cpu); break;
        case 0x55: eor_zero_page_x(cpu); break;
        case 0x4D: eor_absolute(cpu); break;
        case 0x5D: eor_absolute_x(cpu); break;
        case 0x59: eor_absolute_y(cpu); break;
        case 0x41: eor_indirect_x(cpu); break;
        case 0x51: eor_indirect_y(cpu); break;

        // BIT
        case 0x24: bit_zero_page(cpu); break;
        case 0x2C: bit_absolute(cpu); break;
        case 0xAD: lda_absolute(cpu); break;  // LDA absolute
    case 0xAE: ldx_absolute(cpu); break;  // LDX absolute (doğru opcode)
    // ...
    case 0xD0: bne(cpu); break;

        default:
            printf("Unknown opcode: 0x%02X at PC: 0x%04X\n", opcode, cpu->PC - 1);
            exit(1);
    }
}


void cpu_handle_nmi(CPU *cpu) {
    // PC’yi stack’e yaz
    cpu_write(cpu, 0x0100 + cpu->SP--, (cpu->PC >> 8) & 0xFF);
    cpu_write(cpu, 0x0100 + cpu->SP--, cpu->PC & 0xFF);

    // Status register (P) yaz, break bit olmadan
    cpu_write(cpu, 0x0100 + cpu->SP--, cpu->P & ~0x10);

    // I flag’i set et (IRQ disable)
    cpu->P |= 0x04;

    // Vektör adresinden yeni PC oku
    uint16_t lo = cpu_read(cpu, 0xFFFA);
    uint16_t hi = cpu_read(cpu, 0xFFFB);
    cpu->PC = (hi << 8) | lo;
}



void cpu_handle_irq(CPU *cpu) {
    cpu_write(cpu, 0x0100 + cpu->SP--, (cpu->PC >> 8) & 0xFF);
    cpu_write(cpu, 0x0100 + cpu->SP--, cpu->PC & 0xFF);
    cpu_write(cpu, 0x0100 + cpu->SP--, cpu->P & ~0x10);
    cpu->P |= 0x04;  // I flag set
    uint16_t lo = cpu_read(cpu, 0xFFFE);
    uint16_t hi = cpu_read(cpu, 0xFFFF);
    cpu->PC = (hi << 8) | lo;
}

void brk(CPU *cpu) {
    cpu->PC++; // BRK sonrası dummy byte atlanır
    cpu_write(cpu, 0x0100 + cpu->SP--, (cpu->PC >> 8) & 0xFF);
    cpu_write(cpu, 0x0100 + cpu->SP--, cpu->PC & 0xFF);
    cpu_write(cpu, 0x0100 + cpu->SP--, cpu->P | 0x10);  // Break bit set
    cpu->P |= 0x04;
    uint16_t lo = cpu_read(cpu, 0xFFFE);
    uint16_t hi = cpu_read(cpu, 0xFFFF);
    uint16_t brk_vector = (hi << 8) | lo;
    printf("BRK Vector: %04X\n", brk_vector);
    cpu->PC = brk_vector;
}

