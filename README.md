# The discRete event protocol emulatIon vesseL (gRaIL)

The gRaIL architecture allows loading binary protocol implementations into a discrete event simulator.
This repository contains an example implementation as a module for the ns-3 simulator and the Linux/amd64 platform.
Our gRaIL implementation is in a prototype development stage, but already supports loading some common network protocols/networking software.
If you find any bugs or, even better, make an improvement, feel free to share it as an issue or a merge request!

# gRaIL in a Nutshell

In a nutshell, gRaIL executes a protocol binary by intercepting all system calls that the protocol process issues.
The gRaIL architecture is able to maintain perfect repeatability, that is, executing a simulation with identical parameters normally yields identical results.
Also, gRaIL fully runs in *discrete* time.
Thus gRaIL simulations can, run faster than system time if only few events are spawned by the simulated protocols, or slower than system time for large scale simulations without negatively affecting results (different from many real time approaches).

You may find in-depth information on the gRaIL architecture in our associated research article (TODO cite once published).

# Try gRaIL with Docker

The repository contains a docker file that helps you get gRaIL working as fast as possible.
Simply check out the repository and build/run the docker file.
Note that the `--privileged` option is strictly required to allow intercepting system calls via `ptrace`:

```sh
git clone https://gitlab.informatik.hu-berlin.de/grail/grail-module grail
cd grail
docker build -t grail .
docker run --privileged -it grail
```

The docker container will move you to an ns-3 installation with gRaIL locked and loaded!

# Examples

## OLSR example

The OLSR example simulates a wireless network with a regular grid topology or a random disc topology.
Inside the wireless network, OLSR is used to establish routes.
The example can run either ns-3's OLSR or the real-world OLSRd implementation.

One server and one client node are attached to the wireless network via wired CSMA (Ethernet) links.
The client continuously sends packets to the server and the simulation reports the packet delivery ratio.
The packet delivery ratio is indicative for the quality of the routes that OLSR(d) determine.

You can see the available options with by running (inside the Docker container):
```sh
./waf --run 'grail-olsr-example --help'
```

To, e.g., compare OLSRd with link quality (LQ) extensions enabled (the OLSRd default) to the version without LQ (i.e., simple hop count) for five seeds/topologies each, run:

```sh
for i in `seq 1 5`; do ./waf --run "grail-olsr-example --disc=true --olsrLq=true --n=12 --rngRun=$i"; done
# results on our docker system:  98%, 100%, 100%, 97%, 68%
for i in `seq 1 5`; do ./waf --run "grail-olsr-example --disc=true --olsrLq=false --n=12 --rngRun=$i"; done
# results on our docker system: 100%,  98%,  52%, 04%, 06%
```

You may repeat these example runs to observe that the results are fully repeatable between consequtive runs (not necessarily between different systems, though. Should results not be repeatable, please do report this as a bug).

## Iperf3 example

You may also run Iperf as an example with the following instruction:
```
./waf --run 'grail-iperf-example'
```

Again, the `--help` option shows available switches.

# Manual installation

To install this ns-3 gRaIL implementation, you must check out the repository into a compatible ns-3 tree.
We have tested gRaIL for compatilibity with *ns-3.28* and *ns-3.29*.
The location of the grail-module directory must be the *src* directory, the repository-directory should be named *grail*.
To download ns-3.29 *and* install gRaIL, you may follow these instructions:

```sh
  git clone --branch ns-3.29 https://github.com/nsnam/ns-3-dev-git ns-3.29
  cd ns-3.29/src
  git clone https://gitlab.informatik.hu-berlin.de/grail/grail-module grail
  cd ..
  ./waf configure
  # check in output that gRaIL is enabled, then build project:
  ./waf
```

# Supported protocol implementations

As of now, we have tested the following networking software explicitly for compatibility:

 * OLSRd
 * Iperf3

You may, however, try other software.
General socket APIs, netlink protocols, IO, and randomness related functionality is supported.

# Module attributes

The gRaIL module supports a number of attributes settable individually for each protocol instance, which you may find useful.
For a complete list, consult `module/grail.cc`.
The most current attributes are:

 - `PrintStdout` (`bool`, default `false`): Print the protocol process's stdout and stderr messages to stdout. Mainly useful for debugging.
 - `MayQuit` (`bool`, default: `false`): If `false`, consider it an error if the protocol's process terminates. Should be set `false` on server side protocol processes which are not supposed to terminate and `true` on clients that may terminate.
 - `PollLoopDetection` (`bool`, default: `true`): A feature that massively increases performance if the protocol contains poll loop. Not yet documented or evaluated in any paper, so you may want to disable it for publications.
 - `EnableRouting` (`bool`, default: `true`): The protocol process may modify Linux routing tables via, e.g., netlink kernel subsystems. If you want to disallow this, set the attribute to `false`.
 - `SyscallProcessingTime` (`ns3::Time`, default: `0s`): The system call processing time. If you want to be in sync with the paper, set it to `100ns`. Setting it to `0s` can avoid possible perfect-repeatability limitations arising from protocol-process-introduced differences in the file system between repeated simulation runs. If you disable the `PollLoopDetection`, consider setting `SyscallProcessingTime` to `100ns` if you encounter hangs due to poll loop behavior in emulated protocols.
 - `EnablePreloading` (`bool`, default: `true`): Enables the `no_vdso.so` feature, may be disabled if the vDSO is disabled on the simulation system. See the section on the vDSO below for details and background information.

# Suggested: vDSO system configuration

gRaIL uses the system call barrier for protocol emulation.
On many Linux/amd64 operating systems, a dynamic object called the vDSO is loaded into each process that allows reading some I/O values without the system call barrier.
This feature is supposed to improve performance if a process makes frequent calls of, e.g., gettimeofday, but it leaks information into a process and thus invalidates gRaIL results if not taken care of.
We advice to disable the kernel feature system wide, but our architecture has a countermeasure implemented that will very likely work on your system if you cannot do that (see futher below).

We suggest to *disable* the vDSO feature globally on your simulation system, as we could not observe any significant differences in simulation performance.
To disable the vDOS globally, simply pass `vdso=0` as a flag to the Linux kernel upon boot.
On Ubuntu/Debian operating systems, this is easiest accomplished by modifying `/etc/default/grub` so that it contains the following line:
```
GRUB_CMDLINE_LINUX="vdso=0"
```
After the modification, run `update-grub` once as root and reboot the simulation server.

Alternatively, e.g., if you do not have root access on the simulation server, you may make use of our `no_vdso.so` library feature.
This feature is enabled by default for gRaIL as it does not conflict with a system-wide disabled vDSO.

The `no_vdso.so` library is pre-loaded at each protocol process start via the `LD_PRELOADING` environment variable.
The library is built automatically along with the gRaIL module and replaces the four vDSO-affected system calls' glibc-wrappers to date: `gettimeofday`, `clock_gettime`, `time`, and `getcpu`.
We could not observe any significant performance difference between disabling the vDSO globally and using the wrapper library, but it is possible that a future Linux kernel version extends the usage of the vDSO (althrough this has not happened since the 2.6 release of Linux/amd64).
In this unlikely case, an update to the `no_vdso.so` library is required or simulation results may be invalid, thus our advice to disable the vDSO system wide.
If you opt to rely on the `no_vdso.so` library, you may consult the simulation system's `vdso(7)` man page to see if any additional calls were implemented (please report an issue in this case).
