/* Simple non-adaptive, non backoff RTM lock. */
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "barrier.h"
#include "seq.h"
#include "comm.h"

static comm_data test_data;
seqlock_t lock;
/* get max priority of specified policy*/
int max_priority(int policy) {
        return sched_get_priority_max(policy);
}

/* bind cpu to eliminate the scheduler time and cache time*/
int cpu_bind(int cpu_id) {
	if (cpu_id < 0)
		return -1;
	int err;
	cpu_set_t mask;

	CPU_ZERO(&mask);
	CPU_SET(cpu_id, &mask);
	if ((err = sched_setaffinity(0, sizeof mask, &mask)) != 0)
		EXIT(0, "set affinity failed");

	return 0;
}

void *read(void *arg) {
	unsigned int start;
	int temp0=0, temp1=0, temp2=0;
        cpu_bind(0);
        pthread_barrier_wait(&barrier);
	do{
		start = read_seqbegin(&lock);
		temp0 = test_data.test_0;
		temp1 = test_data.test_1;
		temp2 = test_data.test_2;
	}while(read_seqretry(&lock, start));
	if (temp0 != temp1 || temp1 != temp2)
		printf("%d %d %d\n", temp0, temp1, temp2);
}

void *write(void *arg) {
        cpu_bind(1);
        pthread_barrier_wait(&barrier);
	write_seqlock(&lock);
	test_data.test_0++;
	test_data.test_1++;
	test_data.test_2++;
	write_sequnlock(&lock);
}

int main(void) {
	int err;
	int i;
	pthread_attr_t attr;
	struct sched_param param;
	char *msg;

	lock_init(&lock);
	test_data.test_0 = 1;
	test_data.test_1 = 1;
	test_data.test_2 = 1;
	param.sched_priority = max_priority(SCHED_RR);
        if ((err=pthread_attr_init(&attr)) != 0)
                EXIT(errno, "init attr failed");
	/* set thread policy */
        if ((err=pthread_attr_setschedpolicy(&attr, SCHED_RR)) != 0)
                EXIT(errno, "set policy failed");
        /* set thread priority */
        if ((err=pthread_attr_setschedparam(&attr, &param)) != 0)
                EXIT(errno, "set param failed");

	pthread_barrier_init(&barrier, NULL, NUM_READER + NUM_WRITER);

	for(i=0; i<NUM_READER; i++)
		if ((err=pthread_create(&reader[i], NULL, &read, NULL)) != 0) {
			sprintf(msg, "create read pthread %d error", i);
			EXIT(errno, msg);
		}
	for(i=0; i<NUM_WRITER; i++)
		if ((err=pthread_create(&writer[i], NULL, &write, NULL)) != 0) {
			sprintf(msg, "create writer thread %d error", i);
			EXIT(errno, msg);
		}

	for(i=0; i<NUM_READER; i++)
		pthread_join(reader[i], NULL);
	for(i=0; i<NUM_WRITER; i++)
		pthread_join(writer[i], NULL);

	return 0;
}
