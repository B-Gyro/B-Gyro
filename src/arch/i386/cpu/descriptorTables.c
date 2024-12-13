# include "arch/i386/cpu/descriptorTables.h"

void	initDescriptorTables() {
	initGDT();
	initIDT();
}