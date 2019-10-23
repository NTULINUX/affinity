## Simple CPU affinity test

### To compile:

$ gcc affinity.c -O2 -lpthread -o affinity.o

### To execute:

$ ./affinity.o


This works best with nohz_full and rcu_nocbs as CPU 0 will always be used for timekeeping.
Using isolcpus is also strongly encouraged (assuming kernel support is available)

Loosely based off code from:

https://github.com/LinuxCNC/linuxcnc/blob/master/src/rtapi/uspace_rtapi_app.cc

https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/application_base

For more information, please visit the following pages:

https://www.kernel.org/doc/Documentation/timers/NO_HZ.txt

https://lwn.net/Articles/522262/

https://www.gnu.org/software/libc/manual/html_node/CPU-Affinity.html

https://linux.die.net/man/2/sched_setaffinity

https://linux.die.net/man/3/sched_getcpu
