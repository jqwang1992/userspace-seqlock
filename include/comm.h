#ifndef _COMM_H
#define _COMM_H
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

#define NUM_READER  2000
#define NUM_WRITER  20

#define EXIT(errno, msg) \
 do { \
	printf("%s\n, error happen in file:%s" \
	", line:%d", msg, __FILE__, __LINE__); \
	exit(errno); \
 }while(0)

pthread_barrier_t   barrier;

#ifdef SPIN_LOCK
pthread_spinlock_t  spinlock;
#elif MUTEX_LOCK
pthread_mutex_t mutexlock;
#endif

static int hle_lock_var;

typedef struct {
	int test_0;
        int test_1;
        int test_2;
} comm_data;

pthread_t reader[NUM_READER];
pthread_t writer[NUM_WRITER];

#endif /* _RTM_SPIN_H */
