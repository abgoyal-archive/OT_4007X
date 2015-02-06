/*
 * linux/include/asm-sh/timex.h
 *
 * sh architecture timex specifications
 */
#ifndef __ASM_SH_TIMEX_H
#define __ASM_SH_TIMEX_H

/*
 * Only parts using the legacy CPG code for their clock framework
 * implementation need to define their own Pclk value. If provided, this
 * can be used for accurately setting CLOCK_TICK_RATE, otherwise we
 * simply fall back on the i8253 PIT value.
 */

#if 0
#ifdef CONFIG_SH_PCLK_FREQ
#define CLOCK_TICK_RATE		(CONFIG_SH_PCLK_FREQ / 4) /* Underlying HZ */
#else
#define CLOCK_TICK_RATE		1193180
#endif

#include <asm-generic/timex.h>
#endif //0

#include <linux/io.h>
#include <cpu/timer.h>

#define CLOCK_TICK_RATE               (HZ * 100000UL)

typedef unsigned long long cycles_t;

static __inline__ cycles_t get_cycles (void)
{
	return 0xffffffff - ctrl_inl(TMU1_TCNT);
}

#endif /* __ASM_SH_TIMEX_H */
