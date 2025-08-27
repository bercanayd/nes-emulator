#include "rom.h"
#include <stdio.h>
#include <string.h>

bool load_rom(const char *filename, Cartridge *cart) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("ROM dosyası acilamadi: %s\n", filename);
        return false;
    }

    uint8_t header[16];
    fread(header, 1, 16, file);

    // Başlık kontrolü: "NES\x1A"
    if (memcmp(header, "NES\x1A", 4) != 0) {
        printf("Geçersiz NES dosyası.\n");
        fclose(file);
        return false;
    }
    int mapper = (header[7] & 0xF0) | (header[6] >> 4);
printf("Mapper: %d\n", mapper);
    int prg_banks = header[4];
    int chr_banks = header[5];

    cart->prg_rom_size = prg_banks * 16384;
    cart->chr_rom_size = chr_banks * 8192;

    fread(cart->prg_rom, 1, cart->prg_rom_size, file);

    if (chr_banks > 0) {
        fread(cart->chr_rom, 1, cart->chr_rom_size, file);
    }

    fclose(file);
    return true;
}
