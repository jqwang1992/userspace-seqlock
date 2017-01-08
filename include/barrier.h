/*
 * Author : Jianqiang Wang
 * Mail   : jqwang2015@lzu.edu.cn
 */
#ifndef _SEQLOCK_H
#define _SEQLOCK_H

/*
 * lfence : performs a serializing operation on all load-from-memory
 * instructions that were issued prior the LFENCE instruction.
 */
static void inline smp_rmb()
{
	asm volatile("lfence":::"memory");
}

/*
 * sfence : the means same as lfence.
 */
static void inline smp_wmb()
{
	asm volatile("sfence":::"memory");
}

/*
 * mfence : performs a serializing operation on all load-from-memory and
 * store-to-memory instructions that were issued prior the MFENCE instructions.
 */
static void inline smp_mb()
{
	asm volatile("mfence":::"memory");
}
#endif
