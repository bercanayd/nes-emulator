#ifndef ROM_H
#define ROM_H
#include "cpu.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t prg_rom[32768];  // En fazla 2 bank (2 * 16KB)
    uint8_t chr_rom[8192];   // Genellikle 1 bank
    int prg_rom_size;
    int chr_rom_size;
} Cartridge;


typedef struct {
    uint8_t memory[65536];
    Cartridge cart;
    // CPU yapısını içeren struct burada olabilir
    // veya CPU burada typedef olarak ayrı tutulabilir
    CPU cpu;
} NES;

bool load_rom(const char *filename, Cartridge *cart);

#endif // ROM_H
