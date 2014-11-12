include $(top_srcdir)/include.mk

lib_LIBRARIES = libpktanon.a 
				
###############################################################################
###  pktanon library						
###############################################################################

pktanon_sources =  $(sources) $(transformation_sources) $(anonprim_sources) $(c_sources)
libpktanon_a_SOURCES =  $(pktanon_sources)

# included anon primitives:
anonprim_sources = 
anonprim_sources += AnonFactory.cpp
anonprim_sources += anonprimitives/AnonPrimitive.cpp 

anonprim_sources +=  anonprimitives/AnonConstOverwrite.cpp 
anonprim_sources +=  anonprimitives/AnonConstOverwriteRange.cpp 
# anonprim_sources +=  anonprimitives/AnonContinuousChar.cpp 
anonprim_sources +=  anonprimitives/AnonIdentity.cpp 
anonprim_sources +=  anonprimitives/AnonRandomize.cpp 
anonprim_sources +=  anonprimitives/AnonShorten.cpp 
anonprim_sources +=  anonprimitives/AnonShuffle.cpp 
anonprim_sources +=  anonprimitives/AnonWhitenoise.cpp

anonprim_sources +=  anonprimitives/AnonBroadcastHandler.cpp 
anonprim_sources +=  anonprimitives/AnonKnownEthertypeHandler.cpp 
anonprim_sources +=  anonprimitives/AnonKnownProtocolHandler.cpp 

anonprim_sources +=  anonprimitives/cryptopan/rijndael.cpp 
anonprim_sources +=  anonprimitives/cryptopan/panonymizer.cpp 
anonprim_sources +=  anonprimitives/AnonCryptoPan.cpp 

if USE_NETTLE
anonprim_sources +=  anonprimitives/AnonHashSha1_Nettle.cpp 
anonprim_sources +=  anonprimitives/AnonHashSha256_Nettle.cpp 
anonprim_sources +=  anonprimitives/AnonHashHmacSha1_Nettle.cpp 
endif

if BYTEWISEHASHANONS
anonprim_sources +=  anonprimitives/AnonBytewise.cpp 
anonprim_sources +=  anonprimitives/AnonBytewiseHashHmacSha1.cpp 
anonprim_sources +=  anonprimitives/AnonBytewiseHashSha1.cpp 
# anonprim_sources +=  anonprimitives/hmacsha1/sha1_hmac.cpp 
endif

# included transformations:
transformation_sources = 
transformation_sources += transformations/Transformation.cpp
transformation_sources += transformations/EthernetPacketTransformation.cpp
transformation_sources += transformations/VlanTagTransformation.cpp
transformation_sources += transformations/LinuxCookedCaptureTransformation.cpp
transformation_sources += transformations/ArpPacketTransformation.cpp
transformation_sources += transformations/IPv4PacketTransformation.cpp
transformation_sources += transformations/IPv6PacketTransformation.cpp
transformation_sources += transformations/UdpPacketTransformation.cpp
transformation_sources += transformations/TcpPacketTransformation.cpp
transformation_sources += transformations/IcmpPacketTransformation.cpp
transformation_sources += transformations/ICMPv6PacketTransformation.cpp
transformation_sources += transformations/PayloadTransformation.cpp
transformation_sources += transformations/TransformationsConfigurator.cpp

transformation_sources += transformations/DefaultTransformationsConfigurator.cpp
transformation_sources += transformations/ErrorCodes.cpp

sources = 
	
# utilities:	
sources += Checksum.cpp
sources += RandomNumberGenerator.cpp
	
# configuration:
sources += ConfigSAXParser.cpp
sources += PktAnonConfig.cpp
sources += PktAnon.cpp

# config for profiling:
sources += HardCodedConfig.cpp

# c sources:
c_sources =
c_sources += PktAnonC.cpp