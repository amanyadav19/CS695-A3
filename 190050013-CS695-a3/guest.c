#include <stddef.h>
#include <stdint.h>

static void outb(uint16_t port, uint8_t value) {
	asm("outb %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

static inline void outb_32(uint16_t port, uint32_t value) {
  	asm("out %0,%1" : /* empty */ : "a" (value), "Nd" (port) : "memory");
}

static inline uint32_t inb_32(uint16_t port) {
  uint32_t ret;
  asm("in %1, %0" : "=a"(ret) : "Nd"(port) : "memory" );
  return ret;
}

void HC_print8bit(uint8_t val)
{
	outb(0xE9, val);
}

void HC_print32bit(uint32_t val)
{
	outb_32(0xEE, val);
}

uint32_t HC_numExits()
{
	return inb_32(0xEE);
}

void HC_printStr(char *str)
{
	outb_32(0xEB, (uint32_t)((intptr_t) str));
}

uint32_t HC_gvaToHva(uint32_t gva)
{
	// Fill in here
	outb_32(0xE8, gva);
	return inb_32(0xE8);
}

uint32_t HC_getTimestamp()
{
	return inb_32(0xED);
}

void
__attribute__((noreturn))
__attribute__((section(".start")))
_start(void) {
	// const char *p;

	// for (p = "Hello, world!\n"; *p; ++p)
	// 	HC_print8bit(*p);


	/*----------Don't modify this section. We will use grading script---------*/
	/*---Your submission will fail the testcases if you modify this section---*/
	HC_print32bit(2048);
	HC_print32bit(4294967295);

	uint32_t num_exits_a, num_exits_b;
	num_exits_a = HC_numExits();

	char *str = "CS 695 Assignment 3\n";
	HC_printStr(str);

	num_exits_b = HC_numExits();

	HC_print32bit(num_exits_a);
	HC_print32bit(num_exits_b);

	uint32_t hva;
	hva = HC_gvaToHva(1024);
	HC_print32bit(hva);
	hva = HC_gvaToHva(4294967295);
	HC_print32bit(hva);


	uint32_t tsc;
	tsc = HC_getTimestamp();
	HC_print32bit(tsc);
	/*------------------------------------------------------------------------*/

	*(long *) 0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a" (42) : "memory");
}
