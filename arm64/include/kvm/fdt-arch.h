#ifndef ARM__FDT_H
#define ARM__FDT_H

enum phandles {
	PHANDLE_RESERVED = 0,
	PHANDLE_GIC,
	PHANDLE_MSI,
	PHANDLE_DMA,
	PHANDLES_MAX
};

void generate_dma_region_prop(void *fdt);

#endif /* ARM__FDT_H */
