# Assignment 2

These files modify the FreeBSD 12.0 scheduler and implements a new random scheduler called Splatter Scheduler.

## Getting Started

### Prerequisites

[FreeBSD 12.0](https://download.freebsd.org/ftp/releases/amd64/amd64/ISO-IMAGES/12.0/FreeBSD-12.0-RELEASE-amd64-disc1.iso.xz) - The server OS using tcsh

### Installing The Kernel

To install the new kernel first (as a root user), create a new kernel configuration:
```
cd /usr/src/sys/amd64/conf
cp GENERIC MYKERNEL
```
Next, download the files into your workspace on your FreeBSD machine.

In the directory containing the new files, run the makefile command:
```
cd path/to/directory
make install
```

Change directories and compile the new kernel:
```
cd /usr/src
make buildkernel KERNCONF=MYKERNEL                 // initial compile

make buildkernel -j4 KERNCONF=MYKERNEL -DKERNFAST  // fast compile
```

Install the compiled kernel:
```
make installkernel KERNCONF=MYKERNEL
```

Shutdown the system and reboot into the new kernel.

### Uninstalling

Forgot to make a snapshot? To restore the original FreeBSD kernel file(s) and remove created benchmark items:
```
make uninstall

// recompile kernel, install, and reboot
```

## Switching Scheduling Cases

To switch between scheduling cases:
```
make case_1  // use ULE scheduling (FreeBSD)
make case_2  // use ULE scheduling with priority queues
make case_3  // use splatter scheduler
make case_4  // use splatter scheduling with priority queues
```

To check which scheduling case is being used:
```
make check_case
```

## Running the Benchmark

To run the benchmark tool and get runtime stats:
```
make test
```

The runtime logs can be viewed in the file ```benchmark_log```.

## Authors

* **Kevin Wang**
* **Malcolm Neill**
* **Nicolette Miller**
* **Edmund Yu**
