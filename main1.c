// main1.c
#include <stdio.h>
#include "cpu.h"
#include "ppu.h"

// CPU ve PPU durumlarını yazdıran yardımcı fonksiyonlar
void print_cpu_state(CPU *cpu) {
    printf("CPU State:\n");
    printf("PC: %04X  SP: %02X  A: %02X  X: %02X  Y: %02X  P: %02X\n",
           cpu->PC, cpu->SP, cpu->A, cpu->X, cpu->Y, cpu->P);
}

void print_ppu_state(PPU *ppu) {
    printf("\nPPU State:\n");
    printf("PPU Register 0: %02X\n", ppu->registers[0]);
    printf("PPU Register 1: %02X\n", ppu->registers[1]);
    printf("PPU Status: %02X\n", ppu->registers[2]);
    printf("Scanline: %d, Cycle: %d\n", ppu->scanline, ppu->cycle);
}

int main() {
    CPU cpu = {0};
    PPU ppu = {0};

    cpu.ppu = &ppu;

    cpu_reset(&cpu);

    cpu.memory[0x8000] = 0xA9; // LDA immediate
    cpu.memory[0x8001] = 0x42;
    cpu.memory[0x8002] = 0xEA; // NOP
    cpu.memory[0x8003] = 0x00; // BRK

    cpu.PC = 0x8000;

    // Buraya VBlank bitini set et
    cpu.ppu->registers[2] |= 0x80;
    print_ppu_state(cpu.ppu);

    // main döngüsünde:
int steps = 0;
while (1) {
    cpu_step(&cpu);
    ppu_step(cpu.ppu);

    if (steps % 1000 == 0) {  // Her 1000 adımdan bir durumu yazdır
        print_cpu_state(&cpu);
        print_ppu_state(cpu.ppu);
    }

    if (cpu.memory[cpu.PC] == 0x00) break;
    steps++;
}



    printf("Program tamamlandı.\n");
    return 0;
}

