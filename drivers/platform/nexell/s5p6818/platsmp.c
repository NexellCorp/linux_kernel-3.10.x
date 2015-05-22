/*
 * Spin Table SMP initialisation
 *
 * Copyright (C) 2013 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/delay.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/smp.h>

#include <asm/cacheflush.h>
#include <asm/cpu_ops.h>
#include <asm/cputype.h>
#include <asm/smp_plat.h>
#include <asm/io.h>

#include <nexell/platform.h>

/*
#define	pr_debug	printk
*/

extern void secondary_start_head(void);
volatile unsigned long secondary_pen_release = INVALID_HWID;


/*
 * Write secondary_pen_release in a way that is guaranteed to be
 * visible to all observers, irrespective of whether they're taking part
 * in coherency or not.  This is necessary for the hotplug code to work
 * reliably.
 */
static void write_pen_release(u64 val)
{
	void *start = (void *)&secondary_pen_release;
	unsigned long size = sizeof(secondary_pen_release);

	secondary_pen_release = val;
	__flush_dcache_area(start, size);
}

static void inline smp_soc_cpu_signature(unsigned int cpu)
{
	__raw_writel(__pa(secondary_start_head), __io_address(SCR_ARM_SECOND_BOOT));
	__raw_writel(cpu, __io_address(SCR_SMP_WAKE_CPU_ID));
}

static int smp_soc_cpu_init(struct device_node *dn, unsigned int cpu)
{
	return 0;
}

#ifdef  RESET_SNOOP_CTRL
#define	CCI400_BASE		0xe0090000
static void smp_soc_cpu_set_snoop(void)
{
	void __iomem *cci = ioremap(CCI400_BASE, SZ_1M);

	__raw_writel(0x8, (cci + 0x0000));				// CCI
	__raw_writel(0x0, (cci + 0x1000));				// S0 : coresight
	__raw_writel(0x0, (cci + 0x2000));				// S1 : bottom bus
	__raw_writel(0x0, (cci + 0x3000));				// S2 : top bus
	__raw_writel((0x3<<30) | 0x3, (cci + 0x4000));	// S3: cpu cluster 1
	__raw_writel((0x3<<30) | 0x3, (cci + 0x5000));	// S4: cpu cluster 0

	iounmap(cci);
}
#endif
static int smp_soc_cpu_prepare(unsigned int cpu)
{
	return 0;
}

static int smp_soc_cpu_boot(unsigned int cpu)
{
    unsigned long timeout;

	smp_soc_cpu_signature(cpu);

    /*
     * This is really belt and braces; we hold unintended secondary
     * CPUs in the holding pen until we're ready for them.  However,
     * since we haven't sent them a soft interrupt, they shouldn't
     * be there.
     */
    write_pen_release(cpu_logical_map(cpu));

    /*
     * Send the secondary CPU a soft interrupt, thereby causing
     * the boot monitor to read the system wide flags register,
     * and branch to the address found there.
     */
	#if 0
    sev();
	#else
    arch_send_call_function_ipi_mask(cpumask_of(cpu));

    timeout = jiffies + (1000 * HZ);
    while (time_before(jiffies, timeout)) {
        smp_rmb();
        if (secondary_pen_release == INVALID_HWID)
            break;

        udelay(10);
    }
	#endif

    /*
     * now the secondary core is starting up let it run its
     * calibrations, then wait for it to finish
     */

    return secondary_pen_release != INVALID_HWID ? -ENOSYS : 0;
}

static void smp_soc_cpu_postboot(void)
{
	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	write_pen_release(INVALID_HWID);
}

#ifdef CONFIG_HOTPLUG_CPU

static int smp_soc_cpu_disable(unsigned int cpu)
{
	printk("[%s]\n", __func__);
	return 0;
}

static void smp_soc_cpu_die(unsigned int cpu)
{
	printk("[%s]\n", __func__);
}
#endif

#ifdef CONFIG_ARM64_CPU_SUSPEND
static int smp_soc_cpu_suspend(unsigned long index)
{
	printk("[%s]\n", __func__);
	return 0;
}
#endif

const struct cpu_operations cpu_nxp_ops = {
	.name		= "nxp-smp",
	.cpu_init	= smp_soc_cpu_init,
	.cpu_prepare	= smp_soc_cpu_prepare,
	.cpu_boot	= smp_soc_cpu_boot,
	.cpu_postboot	= smp_soc_cpu_postboot,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_disable	= smp_soc_cpu_disable,
	.cpu_die	= smp_soc_cpu_die,
#endif
#ifdef CONFIG_ARM64_CPU_SUSPEND
	.cpu_suspend	= smp_soc_cpu_suspend,
#endif
};
