#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include <stdbool.h>  // `bool`, `true`, `false` için gerekli

typedef struct {
    uint8_t registers[8];       // $2000 - $2007 register cache
    uint8_t oam[256];           // Sprite RAM (Object Attribute Memory)
    uint8_t vram[0x4000];       // 16 KB Video RAM (Pattern tables, nametables, palettes)
    uint16_t vram_addr;         // VRAM adres göstergesi
    uint8_t data_buffer;        // PPUDATA okuma buffer’ı
    uint8_t status;
    bool write_toggle;          // PPUADDR ve PPUSCROLL için toggle
    
    

  
   

    uint8_t scroll_x, scroll_y;

    int cycle;
    int scanline;


} PPU;

// Fonksiyon bildirimleri
void ppu_init(PPU *ppu);
uint8_t ppu_read(PPU *ppu, uint16_t addr);
void ppu_write(PPU *ppu, uint16_t addr, uint8_t value);
void ppu_step(PPU *ppu);
uint8_t ppu_read_status(PPU *ppu);
void ppu_render_frame(PPU *ppu, uint32_t *framebuffer) ;
#endif
