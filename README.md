# DataExporter

DataExporter is a modified GetBench DataRouter designed to pipe data from the CoBo module to both a file as well as a generic TCP server. DataExporter is intended to be used in concert with the [attpc_conduit](https://github.com/gwm17/attpc_conduit) online analysis pipeline.

## Installation

Clone the repository using `git clone --recursive https://github.com/gwm17/DataExporter.git`.

The installation is highly customized to the configuration of the AT-TPC electronics and networks. The build system is CMake, but several paths are set to the known install location for AT-TPC software. Assuming you are on one of the AT-TPC machines, DataExporter is built and installed using the following commands from within the repository:

```sh
mkdir build
cd build
cmake ..
make -j 4
```

Once building is complete the excutable is installed to the `bin` directory of the repository.

## Usage

Again, this is highly dependent on what system you have configured, but for the AT-TPC, navigate to the directory `~/Data/mm#` where `#` should be replaced by the ID of the machine you are on and run the following command

`/path/to/Data/Exporter/repo/bin/DataExporter`

and you should see print statements indicating that the exporter is up and running.

## Notes

DataExporter should be started *before* the CoBos have been fully configured.

## Requirements

All of the requirements for compiling the GETDAQ system as well as C++11 and CMake > 3.12.