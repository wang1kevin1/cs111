# Assgn-1

This program is a simple shell that runs on FreeBSD 12.0.

## Getting Started

### Prerequisites

[FreeBSD 12](https://download.freebsd.org/ftp/releases/amd64/amd64/ISO-IMAGES/12.0/FreeBSD-12.0-RELEASE-amd64-disc1.iso.xz) - The server OS using tcsh

### Installing

To install the shell, download the directory /assgn-1/ .

Once downloaded, upload the directory to your FreeBSD server:
```
cd /path/to/host/directory/
scp -r -P 3022 assgn-1/ username@hostname:/path/to/server/directory/
```

Once you are in the newly uploaded directory /assgn-1/ on your server, run the Makefile to finish installation:
```
make
```

To check the code using infer, run the command:
```
make infer
```

To clean object files, run the command:
```
make clean
```
### Uninstalling

To uninstall the shell, exit to the parent directory of /assgn-1/ (cd ..) and run the command:
```
rm -r /assgn-1/
```

## Running the Shell

To run the shell, enter the directory ~/path/to/assgn-1/ and run the command:
```
./myshell
```

### Features

Directory Navigation, i.e:
```
cd /path/to/directory/
cd ..
cd
```

I/O Redirection, i.e:
```
sort -nr < scores
ls -l >> file
```

Error Redirection, i.e:
```
ls -l >& foo
```

Single Pipe Handling, i.e:
```
cat names | uniq
ls | grep logfile
```

**currently can not handle multiple commands using ';' or chained pipes*

## Authors

* **Kevin Wang**