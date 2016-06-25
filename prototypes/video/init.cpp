#include <inttypes.h>

#include "multiboot.h"

#include "../base/include/io.hpp"

struct dummy;

// Symbols generated by linker, no useful content in there...
extern dummy kernelStartMarker;
extern dummy kernelEndMarker;

uint16_t * video = (uint16_t*)0xB8000;

// Prüft, ob man bereits schreiben kann
static uint8_t is_transmit_empty(uint16_t base) {
	return inb(base+5) & 0x20;
}
 
 // Byte senden
static void write_com(uint16_t base, uint8_t chr) {
	while (is_transmit_empty(base)==0);
	outb(base,chr);
}

#define VBE_FAR(name) uint32_t name;

struct ModeInfoBlock {
  uint16_t attributes;
  uint8_t winA,winB;
  uint16_t granularity;
  uint16_t winsize;
  uint16_t segmentA, segmentB;
  VBE_FAR(realFctPtr);
  uint16_t pitch; // bytes per scanline
 
  uint16_t Xres, Yres;
  uint8_t Wchar, Ychar, planes, bpp, banks;
  uint8_t memory_model, bank_size, image_pages;
  uint8_t reserved0;
 
  uint8_t red_mask, red_position;
  uint8_t green_mask, green_position;
  uint8_t blue_mask, blue_position;
  uint8_t rsv_mask, rsv_position;
  uint8_t directcolor_attributes;
 
  uint32_t physbase;  // your LFB (Linear Framebuffer) address ;)
  uint32_t reserved1;
  uint16_t reserved2;
} __attribute__((packed));

extern "C" void init(multiboot_info_t const & data)
{
	const char *msg = "You should not see this.";
	while(*msg)
	{
		*(video++) = *msg++ | 0x0700;
	}
	
	ModeInfoBlock *modeInfo = (ModeInfoBlock*)data.vbe_mode_info;
	
	uint8_t *fb = (uint8_t*)modeInfo->physbase;
	
	for(int y = 0; y < modeInfo->Yres; y++)
	{
		for(int x = 0; x < modeInfo->Xres; x++)
		{
			uint32_t r = (x * 384 / modeInfo->Xres);
			uint32_t g = (y * 384 / modeInfo->Yres);
			uint32_t b = 0x00;
			
			fb[modeInfo->pitch * y + 4 * x + 2] = r;
			fb[modeInfo->pitch * y + 4 * x + 1] = g;
			fb[modeInfo->pitch * y + 4 * x + 0] = b;
		}
	}
	
	write_com(0x3F8, 'H');
	write_com(0x3F8, 'i');
	write_com(0x3F8, '\n');
	
	while(true);
}