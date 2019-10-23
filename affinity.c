/*
 * Written by Alec Ari
 *
 * Simple test of offloading program to last core on system.
 *
 * Compile: gcc affinity.c -O2 -lpthread -o affinity.o
 * Execute: ./affinity.o
 *
 * This works best with nohz_full and rcu_nocbs as CPU 0 will always be used for timekeeping.
 * Using isolcpus is also strongly encouraged (assuming kernel support is available)
 *
 * Loosely based off code from:
 * https://github.com/LinuxCNC/linuxcnc/blob/master/src/rtapi/uspace_rtapi_app.cc
 * https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/application_base
 *
 * For more information, please visit the following pages:
 * https://www.kernel.org/doc/Documentation/timers/NO_HZ.txt
 * https://lwn.net/Articles/522262/
 * https://www.gnu.org/software/libc/manual/html_node/CPU-Affinity.html
 * https://linux.die.net/man/2/sched_setaffinity
 * https://linux.die.net/man/3/sched_getcpu
 */

#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_func(void *data) {
	/* Count the number of online CPUs starting from 1 */
	int all_cpus = sysconf(_SC_NPROCESSORS_CONF);

	/* Now count from 0 like the kernel does */
	int cpus = all_cpus -1;

	/* If dual-core or more */
	if(cpus >= 1) {
		cpu_set_t cpuset; /* Bitset where each bit represents a CPU */
		CPU_ZERO(&cpuset); /* Zero initialize cpuset */
		CPU_SET(cpus, &cpuset); /* Pick the last core available */
		sched_setaffinity(0, sizeof(cpuset), &cpuset); /* Set process CPU affinity mask */
		printf("Selecting CPU %d.\n", cpus);
	/* If single-core */
	} else if (cpus == 0) {
		printf("Single-core detected, doing nothing.\n");
	/* If srsly borked */
	} else {
		printf("Error detecting number of cores. Exiting.\n");
		exit(1);
	}

	printf("Running on CPU %d.\n", sched_getcpu());
	return 0;
}

int main(void) {
	pthread_t thread;
	pthread_create(&thread, NULL, thread_func, NULL);
	pthread_join(thread, NULL);
	return 0;
}
