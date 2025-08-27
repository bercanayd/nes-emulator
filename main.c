// main.c
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include "ppu.h"
#include "rom.h"

int main() {
    const char *filename = "games/dosya1.nes";

    CPU cpu = {0};
    PPU ppu = {0};
    Cartridge cart = {0};

    cpu.ppu = &ppu;
    ppu_init(&ppu);

  
    // ... önceki kod

    if (!load_rom(filename, &cart)) {
        printf("ROM dosyası yüklenemedi: %s\n", filename);
        return 1;
    } else {
        printf("ROM başarıyla yüklendi: %s\n", filename);
    }

    // PRG ROM kopyala
if (cart.prg_rom_size == 16384) {
    // 16 KB ise 0x8000'e ve 0xC000'e aynısını kopyala
    memcpy(&cpu.memory[0x8000], cart.prg_rom, 16384);
    memcpy(&cpu.memory[0xC000], cart.prg_rom, 16384);
} else if (cart.prg_rom_size == 32768) {
    // 32 KB ise doğrudan 0x8000'e kopyala
    memcpy(&cpu.memory[0x8000], cart.prg_rom, 32768);
}


// Vectorları CPU belleğine kopyala (reset ve IRQ/BRK)
cpu.memory[0xFFFC] = cart.prg_rom[cart.prg_rom_size - 4];
cpu.memory[0xFFFD] = cart.prg_rom[cart.prg_rom_size - 3];
cpu.memory[0xFFFE] = cart.prg_rom[cart.prg_rom_size - 2];
cpu.memory[0xFFFF] = cart.prg_rom[cart.prg_rom_size - 1];


    // Reset vector ROM'dan alınır
    uint16_t reset_vector = cpu.memory[0xFFFC] | (cpu.memory[0xFFFD] << 8);
cpu.PC = reset_vector;

    printf("Reset vector: %04X\n", reset_vector);

    cpu_reset(&cpu);   // Sadece registerları sıfırla
    cpu.PC = reset_vector; // PC'yi reset vector olarak ayarla
printf("ROM[0xC5F4] = %02X\n", cpu.memory[0xC5F4]);

    cpu_print_state(&cpu);

    while (1) {
    for (int i = 0; i < 3; i++) {
        ppu_step(&ppu);
    }

    cpu_step(&cpu);

    cpu_print_state(&cpu);

    if (cpu.memory[cpu.PC] == 0x00) {
        printf("BRK at PC=0x%04X, durduruluyor...\n", cpu.PC);
        break;
    }
}



    return 0;

}
