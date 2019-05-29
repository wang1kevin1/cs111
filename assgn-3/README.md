# Assignment 2

These files modify the FreeBSD 12.0 scheduler and implements a new paging algorithm called Bon Chance.

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

Forgot to make a snapshot? To restore the original FreeBSD kernel file(s):
```
make uninstall

// recompile kernel, install, and reboot
```

## Benchmarking

To run the benchmark, install the stress test package and run it:
```
pkg install stress
vmstat 1 20 > benchmark_results.log & sleep 5 && stress -m 1 --vm-bytes 16G -t 60s
```
When the benchmark has completed, read the kernel system messages into a file:
```
dmesg | grep assgn3 >> benchmark_results.log
```

## Authors

* **Kevin Wang** *(kwang43)*
* **Malcolm Neill** *(mneill)*
* **Nicolette Miller** *(nmiller2)*
* **Edmund Yu** *(eyu9)*