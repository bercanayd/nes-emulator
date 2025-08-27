//romun hafızaya alınıp işlenmeye başlaması.



#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

#define NES_HEADER_SIZE 16
#define PRG_ROM_PAGE_SIZE 16384  // 16 KB

int load_rom(CPU *cpu, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("ROM açılırken hata");
        return 0;
    }

    uint8_t header[NES_HEADER_SIZE];
    fread(header, 1, NES_HEADER_SIZE, file);

    // "NES\x1A" kontrolü
    if (memcmp(header, "NES\x1A", 4) != 0) {
        fprintf(stderr, "Geçersiz NES dosyası.\n");
        fclose(file);
        return 0;
    }

    int prg_rom_size = header[4] * PRG_ROM_PAGE_SIZE;

    // PRG-ROM'u 0x8000 adresinden itibaren belleğe yükle
    fread(&cpu->memory[0x8000], 1, prg_rom_size, file);

    // Reset vektörünü PRG-ROM'a göre ayarla
    cpu->memory[0xFFFC] = 0x00;
    cpu->memory[0xFFFD] = 0x80;

    fclose(file);
    return 1;
}

