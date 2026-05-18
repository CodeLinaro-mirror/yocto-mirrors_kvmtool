#include "kvm/fdt.h"
#include "kvm/gic.h"
#include "kvm/kvm.h"
#include "kvm/kvm-cpu.h"
#include "kvm/timer.h"
#include "kvm/util.h"

void timer__generate_fdt_nodes(void *fdt, struct kvm *kvm)
{
	const char compatible[] = "arm,armv8-timer\0arm,armv7-timer";
	u32 cpu_mask = gic__get_fdt_irq_cpumask(kvm);
	int irqs[5] = {13, 14, 11, 10, 12};
	int nr = ARRAY_SIZE(irqs);
	u32 irq_prop[nr * 3];

	if (!kvm->cfg.arch.nested_virt)
		nr--;

	for (int i = 0; i < nr; i++) {
		irq_prop[i * 3 + 0] = cpu_to_fdt32(GIC_FDT_IRQ_TYPE_PPI);
		irq_prop[i * 3 + 1] = cpu_to_fdt32(irqs[i]);
		irq_prop[i * 3 + 2] = cpu_to_fdt32(cpu_mask | IRQ_TYPE_LEVEL_LOW);
	}

	_FDT(fdt_begin_node(fdt, "timer"));
	_FDT(fdt_property(fdt, "compatible", compatible, sizeof(compatible)));
	_FDT(fdt_property(fdt, "interrupts", irq_prop, nr * 3 * sizeof(irq_prop[0])));
	_FDT(fdt_property(fdt, "always-on", NULL, 0));
	if (kvm->cfg.arch.force_cntfrq > 0)
		_FDT(fdt_property_cell(fdt, "clock-frequency", kvm->cfg.arch.force_cntfrq));
	_FDT(fdt_end_node(fdt));
}

