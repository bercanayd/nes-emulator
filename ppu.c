#include "ppu.h"
#include <string.h>
#include <stdio.h>

// PPU'yu başlatır
void ppu_init(PPU *ppu) {
    memset(ppu, 0, sizeof(PPU));
    ppu->cycle = 0;
    ppu->scanline = 0;
    ppu->write_toggle = false;
    printf("PPU initialized\n");
}

// CPU'dan PPU register'larına yazma
void ppu_write(PPU *ppu, uint16_t addr, uint8_t value) {
    addr &= 0x0007; // 0x2000-0x2007 mirror

    switch (addr) {
        case 0x0: // PPUCTRL
            ppu->registers[0] = value;
            printf("PPUCTRL write: 0x%02X\n", value);
            break;

        case 0x1: // PPUMASK
            ppu->registers[1] = value;
            printf("PPUMASK write: 0x%02X\n", value);
            break;

        case 0x3: // OAMADDR
            ppu->registers[3] = value;
            printf("OAMADDR write: 0x%02X\n", value);
            break;

        case 0x4: // OAMDATA
            ppu->oam[ppu->registers[3]] = value;
            ppu->registers[3] = (ppu->registers[3] + 1) & 0xFF;
            printf("OAMDATA write: 0x%02X at OAM[0x%02X]\n", value, ppu->registers[3] - 1);
            break;

        case 0x5: // PPUSCROLL
            if (!ppu->write_toggle) {
                ppu->scroll_x = value;
                ppu->write_toggle = true;
                printf("PPUSCROLL X write: 0x%02X\n", value);
            } else {
                ppu->scroll_y = value;
                ppu->write_toggle = false;
                printf("PPUSCROLL Y write: 0x%02X\n", value);
            }
            break;

        case 0x6: // PPUADDR
            if (!ppu->write_toggle) {
                ppu->vram_addr = (ppu->vram_addr & 0x00FF) | ((uint16_t)(value & 0x3F) << 8);
                ppu->write_toggle = true;
                printf("PPUADDR high byte: 0x%02X, vram_addr=0x%04X\n", value, ppu->vram_addr);
            } else {
                ppu->vram_addr = (ppu->vram_addr & 0xFF00) | value;
                ppu->write_toggle = false;
                printf("PPUADDR low byte: 0x%02X, vram_addr=0x%04X\n", value, ppu->vram_addr);
            }
            break;

        case 0x7: // PPUDATA
            ppu->vram[ppu->vram_addr & 0x3FFF] = value;
            ppu->vram_addr += (ppu->registers[0] & 0x04) ? 32 : 1;
            printf("PPUDATA write: 0x%02X at vram[0x%04X]\n", value, ppu->vram_addr - ((ppu->registers[0] & 0x04) ? 32 : 1));
            break;

        default:
            printf("Warning: Write to unsupported PPU register 0x%04X, value=0x%02X\n", addr + 0x2000, value);
            break;
    }
}

// CPU'dan PPU register'larından okuma
uint8_t ppu_read(PPU *ppu, uint16_t addr) {
    addr &= 0x0007;

    switch (addr) {
        case 0x2: { // PPUSTATUS
            uint8_t status = ppu->registers[2];
            printf("PPU Read Status: 0x%02X (scanline=%d, cycle=%d)\n", status, ppu->scanline, ppu->cycle);
            printf("  VBlank (bit 7): %s\n", (status & 0x80) ? "SET" : "CLEAR");
            printf("  Sprite Zero Hit (bit 6): %s\n", (status & 0x40) ? "SET" : "CLEAR");
            printf("  Sprite Overflow (bit 5): %s\n", (status & 0x20) ? "SET" : "CLEAR");
            ppu->registers[2] &= ~0x80; // VBlank bitini temizle
            ppu->write_toggle = false;  // Toggle sıfırlama
            return status;
        }

        case 0x4: // OAMDATA
            printf("OAMDATA read: 0x%02X from OAM[0x%02X]\n", ppu->oam[ppu->registers[3]], ppu->registers[3]);
            return ppu->oam[ppu->registers[3]];

        case 0x7: { // PPUDATA
            uint16_t vram_addr = ppu->vram_addr & 0x3FFF;
            uint8_t value = ppu->data_buffer;
            ppu->data_buffer = ppu->vram[vram_addr];
            if (vram_addr >= 0x3F00) value = ppu->vram[vram_addr]; // Palette için doğrudan okuma
            ppu->vram_addr += (ppu->registers[0] & 0x04) ? 32 : 1;
            printf("PPUDATA read: 0x%02X from vram[0x%04X]\n", value, vram_addr);
            return value;
        }

        default:
            printf("Warning: Read from unsupported PPU register 0x%04X\n", addr + 0x2000);
            return 0;
    }
}

// PPU döngüsünü ilerletir
void ppu_step(PPU *ppu) {
    ppu->cycle++;

    if (ppu->cycle >= 341) {
        ppu->cycle = 0;
        ppu->scanline++;

        if (ppu->scanline == 241) {
            // VBlank başlar
            ppu->status |= 0x80;
        } else if (ppu->scanline == 261) {
            // VBlank biter, pre-render scanline
            ppu->status &= ~0x80;
            ppu->scanline = 0;
        }
    }
}



uint8_t ppu_read_status(PPU *ppu) {
    uint8_t result = ppu->status;

    // VBlank bit (bit 7) okunduktan sonra sıfırlanmalı
    ppu->status &= ~0x80;

    // PPUADDR ve PPUSCROLL için toggle sıfırlanır
    ppu->write_toggle = false;

    return result;
}


void ppu_render_frame(PPU *ppu, uint32_t *framebuffer) {
    // Burada VRAM'den tile'lar okunur ve framebuffer'a yazılır
}
