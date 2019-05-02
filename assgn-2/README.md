# Assignment 2

These files modify the FreeBSD 12.0 scheduler and implements a new random scheduler called Splatter Scheduler.

## Getting Started

### Prerequisites

[FreeBSD 12.0](https://download.freebsd.org/ftp/releases/amd64/amd64/ISO-IMAGES/12.0/FreeBSD-12.0-RELEASE-amd64-disc1.iso.xz) - The server OS using tcsh

### Installing The Kernel

To install the new kernel first (as a root user), create a new kernel configuration
```
cd /usr/src/sys/amd64/conf
cp GENERIC MYKERNEL
```
Next, download the files into your workspace on your FreeBSD machine.

In the directory containing the new files, run the makefile command:
```
cd path/to/directory
sudo make install
```

Change directories and compile the new kernel:
```
cd /usr/src
sudo make buildkernel KERNCONF=MYKERNEL                 // initial compile

sudo make buildkernel -j4 KERNCONF=MYKERNEL -DKERNFAST  // fast compile
```

Install the compiled kernel:
```
make installkernel KERNCONF=MYKERNEL
```

Shutdown the system and reboot into the new kernel.

## Running the Benchmark


## Authors

* **Kevin Wang**
* **Malcolm Neill**
* **Nicolette Miller**
* **Edmund Yu**
