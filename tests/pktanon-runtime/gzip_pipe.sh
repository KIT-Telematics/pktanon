#!/bin/bash
zcat trace.pcap.gz | $1 - $2
