# The discRete event protocol emulatIon vesseL (gRaIL)

The gRaIL architecture allows loading binary protocol implementations into a discrete event simulator.
In a nutshell, gRaIL executes a protocol binary and intercepts all system calls that the protocol process issues.
The gRaIL architecture is able to maintain perfect repeatability, that is, executing a simulation with identical parameters yields identical results.
Also, gRaIL fully fully runs in *discrete* time.
Therefore, gRaIL simulations can, run faster/slower than system time -- if only few/many events are spawned by the simulated protocols -- without affecting results.

You may find in-depth information on the gRaIL architecture in our article (TODO cite once published). 

This repository contains an exemplary implementation for the ns-3 simulator and the Linux/amd64 platform.
Our gRaIL implementation is in a prototype development stage.
If you find any bugs or, even better, made an improvement, feel free to share it with us as a bug report or merge request!

# Try gRaIL with Docker

The repository contains a docker file that helps you get gRaIL working as fast as possible.
Simply check our the repository and build/run the docker file:

```sh
git clone https://gitlab.informatik.hu-berlin.de/grail/grail-module grail
cd grail
docker build -t grail .
docker run -it grail
```

The docker container will move you to an ns-3 installation with gRaIL installed and configured.

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

As of now, we have tested the following networking software for compatibility:

 * OLSRd
 * Iperf3

# Examples

Todo, add examples.
