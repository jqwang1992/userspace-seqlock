#ifndef _SEQ_H
#define _SEQ_H
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include "barrier.h"

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define cpu_relax() rep_nop()
static __always_inline  void rep_nop(void)
{
	asm volatile("rep;nop":::"memory");
}

#define ACCESS_ONCE(x) (*(volatile typeof(x)*)&(x))

typedef struct {
	unsigned int sequence;
	pthread_spinlock_t lock;
} seqlock_t;

static void lock_init(seqlock_t *seqlock)
{
	int err;
	if ((err=pthread_spin_init(&seqlock->lock, PTHREAD_PROCESS_SHARED)) != 0)
		exit(1);
	seqlock->sequence = 0;
}

static void write_seqlock(seqlock_t *seqlock)
{
	pthread_spin_lock(&seqlock->lock);
	++seqlock->sequence;
	smp_wmb();
}

static void write_sequnlock(seqlock_t *seqlock)
{
	smp_wmb();
	++seqlock->sequence;
	pthread_spin_unlock(&seqlock->lock);
}

static __always_inline unsigned read_seqbegin(seqlock_t *seqlock)
{
	unsigned ret;

repeat:
	ret = ACCESS_ONCE(seqlock->sequence);
	if (unlikely(ret & 1)) {
		cpu_relax();
		goto repeat;
	}
	smp_rmb();

	return ret;
}

static __always_inline unsigned int read_seqretry(seqlock_t *seqlock, unsigned int start)
{
	smp_rmb();

	return unlikely(seqlock->sequence != start);
}
#endif /* _SEQLOCK_H */
