# pktanon
tool for network traces anonymization

## Overview
PktAnon is a generic framework for anonymization of network traces. The user provides an anonymization
profile – a small configuration file where he or she describes how individual fields in packets’ headers must
be processed. An anonymization profile consists of a list of packets, and for each field in the packet header
an anonymization primitive is specified. Anonymization primitive is a function that transforms an original
field into an anonymized field. For example, a user can configure ipv6 addresses to be anonymized using
SHA1 anonymization primitive. PktAnon parses the packets from the input source and applies configured
anonymization primitives to the listed fields.
The main design decision in PktAnon is that anonymized packets are created from scratch. Fields are copied
from the original packets into the new packets after anonymization. Thus no information from the original
trace appears in the anonymized trace, unless it is explicitly configured.
PktAnon comes with code to parse common packets and includes several anonymization primitives. PktAnon
can also be easily extended with new anonymization primitives and new packets.

## Building

1. download the latest version of PktAnon

        git clone https://github.com/KIT-Telematics/pktanon.git

2. change into the project directory and run the autotools chain

        cd pktanon
        ./bootstrap
        ./configure
        make

3. PktAnon executable will be `pktanon` file in the project directory

### Build requirements

In Debian-based distribution, you can install the build dependencies using this command:

        apt-get install build-essential autoconf libxerces-c-dev nettle-dev libpcap-dev libboost-dev

## Documentation

Please see doc/manual.pdf
