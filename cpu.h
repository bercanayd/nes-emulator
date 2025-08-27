//tüm opcodelarımız burada dursuyor. tüm işlemler buradan olacak.
#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include <stdint.h>

#include "ppu.h"  // En sonda include et, çünkü yukarıda sadece tanımını kullandın


//cpu yapımız
typedef struct {
    uint16_t PC;         // Program Counter
    uint8_t SP;          // Stack Pointer
    uint8_t A, X, Y;     // CPU registerları
    uint8_t P;           // Status Flags
    uint8_t memory[0x10000]; // 64KB CPU belleği
    uint8_t ram[0x0800];     // 2KB dahili RAM
    PPU *ppu;                // PPU bağlantısı
    bool nmi_pending;
    bool irq_pending;
} CPU;

// Temel CPU işlemleri
void cpu_print_state(CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_step(CPU *cpu);
uint8_t cpu_read(CPU *cpu, uint16_t addr);
void cpu_write(CPU *cpu, uint16_t addr, uint8_t value);
void bpl(CPU *cpu);
void bmi(CPU *cpu);
void bvc(CPU *cpu);
void bvs(CPU *cpu);
void bcc(CPU *cpu);
void bcs(CPU *cpu);
void beq(CPU *cpu);
void bne(CPU *cpu);

// Transfer Opcodes
void lda_immediate(CPU *cpu);
void ldx_immediate(CPU *cpu);
void ldy_immediate(CPU *cpu);
void tax(CPU *cpu);
void tay(CPU *cpu);
void txa(CPU *cpu);
void tya(CPU *cpu);
void tsx(CPU *cpu);
void txs(CPU *cpu);
void ora_zero_page(CPU *cpu);
void ora_absolute(CPU *cpu);

// Arithmetic Opcodes
void adc_immediate(CPU *cpu);
void sbc_immediate(CPU *cpu);
void sbc_zero_page(CPU *cpu);
void sbc_absolute(CPU *cpu);

// Increment / Decrement
void inc_zero_page(CPU *cpu);
void inx(CPU *cpu);
void iny(CPU *cpu);
void dex(CPU *cpu);
void dey(CPU *cpu);
void dec_zero_page(CPU *cpu);         // EKLENDİ
void dec_absolute(CPU *cpu);          // EKLENDİ

// Branch Opcodes
void beq(CPU *cpu);
void bne(CPU *cpu);
void bcc(CPU *cpu);
void bcs(CPU *cpu);
void bmi(CPU *cpu);
void bpl(CPU *cpu);
void bvs(CPU *cpu);
void bvc(CPU *cpu);
void jmp(CPU *cpu);
void jsr(CPU *cpu);
void rts(CPU *cpu);

// Stack Opcodes
void pha(CPU *cpu);
void pla(CPU *cpu);
void php(CPU *cpu);
void plp(CPU *cpu);
void rti(CPU *cpu);

// Misc Opcodes
void nop(CPU *cpu);
void brk(CPU *cpu);
void clc(CPU *cpu);
void sec(CPU *cpu);
void cli(CPU *cpu);
void sei(CPU *cpu);
void clv(CPU *cpu);
void sei(CPU *cpu);
void cld(CPU *cpu);
void lda_absolute(CPU *cpu);
void lda(CPU *cpu, uint16_t addr);
// Logic Opcodes
void and_immediate(CPU *cpu);
void ora_immediate(CPU *cpu);
void eor_immediate(CPU *cpu);
void bit_zero_page(CPU *cpu);

// Compare Opcodes
void cmp_immediate(CPU *cpu);
void cpx_immediate(CPU *cpu);
void cpy_immediate(CPU *cpu);

// Load/Store
void sta_zero_page(CPU *cpu);
void sta_absolute(CPU *cpu);
void sta_absolute_x(CPU *cpu);
void sta_absolute_y(CPU *cpu);
void stx_zero_page(CPU *cpu);
void stx_absolute(CPU *cpu);
void sty_zero_page(CPU *cpu);
void sty_absolute(CPU *cpu);
void ldx_zero_page(CPU *cpu);
void ldx_absolute(CPU *cpu);
void ldy_zero_page(CPU *cpu);
void ldy_absolute(CPU *cpu);

// Shift/Rotate Opcodes
void asl_accumulator(CPU *cpu);
void asl_zero_page(CPU *cpu);
void lsr_accumulator(CPU *cpu);
void lsr_zero_page(CPU *cpu);
void rol_accumulator(CPU *cpu);        // EKLENDİ
void rol_zero_page(CPU *cpu);           // EKLENDİ
void ror_accumulator(CPU *cpu);        // EKLENDİ
void ror_zero_page(CPU *cpu);           // EKLENDİ
void and_immediate(CPU *cpu);
void and_zero_page(CPU *cpu);
void and_zero_page_x(CPU *cpu);
void and_absolute(CPU *cpu);
void and_absolute_x(CPU *cpu);
void and_absolute_y(CPU *cpu);
void and_indirect_x(CPU *cpu);
void and_indirect_y(CPU *cpu);

// ORA
void ora_immediate(CPU *cpu);
void ora_zero_page(CPU *cpu);
void ora_zero_page_x(CPU *cpu);
void ora_absolute(CPU *cpu);
void ora_absolute_x(CPU *cpu);
void ora_absolute_y(CPU *cpu);
void ora_indirect_x(CPU *cpu);
void ora_indirect_y(CPU *cpu);

// EOR
void eor_immediate(CPU *cpu);
void eor_zero_page(CPU *cpu);
void eor_zero_page_x(CPU *cpu);
void eor_absolute(CPU *cpu);
void eor_absolute_x(CPU *cpu);
void eor_absolute_y(CPU *cpu);
void eor_indirect_x(CPU *cpu);
void eor_indirect_y(CPU *cpu);

// BIT
void bit_zero_page(CPU *cpu);
void bit_absolute(CPU *cpu);
#endif // CPU_H

