# SUBDIRS =  src . tests tests/transformations

SUBDIRS = libpktanon src . tests 
# dist_doc_DATA = README

include $(top_srcdir)/include.mk
AM_CPPFLAGS += -I$(top_srcdir)/libpktanon

###############################################################################
###  pktanon binary						
###############################################################################

bin_PROGRAMS = pktanon 
pktanon_SOURCES = src/Main.cpp src/console_utils.cpp
pktanon_DEPENDENCIES = libpktanon/libpktanon.a src/libpktanonruntime.a 
pktanon_LDADD =  src/libpktanonruntime.a  $(LDADD) 
pktanon_LDFLAGS = -Wl,--whole-archive,libpktanon/libpktanon.a,--no-whole-archive