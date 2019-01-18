# The discRete event protocol emulatIon vesseL (gRaIL)

The gRaIL architecture allows loading binary protocol implementations into a discrete event simulator.
This repository contains an exemplary implementation as a module for the ns-3 simulator and the Linux/amd64 platform.
Our gRaIL implementation is in a prototype development stage, but already supports using some common network protocols/networking software.
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
