#include <stdio.h>

void poke(unsigned long addr, int value)
{
	printf("poking 0x%08x in address 0x%08x\n", value, addr);
	*(int *)addr = value;
}

int peek(unsigned long addr)
{
	printf("peeking at address 0x%08x\n", addr);
	return *(int *)addr;
}
