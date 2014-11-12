include $(top_srcdir)/include.mk
AM_CPPFLAGS += -I$(top_srcdir)/libpktanon

lib_LIBRARIES = libpktanonruntime.a

###############################################################################
###  pktanon-runtime library						
###############################################################################

pktanon_runtime_sources = $(runtime_sources) 
libpktanonruntime_a_SOURCES =  $(pktanon_runtime_sources)

# runtime: 

runtime_sources = 
runtime_sources += PktAnonRuntime.cpp
runtime_sources += RuntimeConfig.cpp
runtime_sources += RuntimeFactory.cpp	

runtime_sources += IstreamRecordsHandler.cpp
runtime_sources += RawSocketRecordsHandler.cpp

runtime_sources += OstreamOutput.cpp
runtime_sources += SocketOutput.cpp

if USE_LIBPCAP
runtime_sources += LibpcapRecordsHandler.cpp
runtime_sources += LibpcapDumpOutput.cpp
runtime_sources += LibpcapInjectOutput.cpp
endif