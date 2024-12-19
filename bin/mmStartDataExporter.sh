#!/bin/bash

IPADDR=`ipconfig getifaddr en0` # This machine's ip address

GETDIR=$HOME/Documents/GET-software # The GET software directories
EXPORTER=$HOME/Documents/DataExporter/bin/DataExporter

export LD_LIBRARY_PATH=$GETDIR/lib:$LD_LIBRARY_PATH # Put the GET libs on the PATH

# Apparently this is a required hack due to ICE on MacPorts?
export DYLD_FALLBACK_LIBRARY_PATH=/opt/local/lib

# If there are any DataExporters running, kill them
killall DataExporter
# If there are any dataRouters running, kill them
killall dataRouter


$EXPORTER $IPADDR $IPADDR
