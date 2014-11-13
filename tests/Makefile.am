include $(top_srcdir)/include.mk
AM_CPPFLAGS += -I$(top_srcdir)/libpktanon

############################################################################
# AnonTester
############################################################################

VPATH=$(top_srcdir)/src/
AM_CPPFLAGS += -I$(VPATH)

UNITTESTS_PROGS = anontester 
anontester_SOURCES = anonprimitives/AnonTester.cpp
anontester_DEPENDENCIES = $(lib_LIBRARIES)
anontester_LDFLAGS = -Wl,--whole-archive,$(top_srcdir)/libpktanon/libpktanon.a,--no-whole-archive

noinst_PROGRAMS = $(UNITTESTS_PROGS)
check_PROGRAMS = $(UNITTESTS_PROGS) 

############################################################################
# DeJaGNU Tests 
############################################################################

AUTOMAKE_OPTIONS += dejagnu
EXTRA_DEJAGNU_SITE_CONFIG = site_extra.exp

DEJATOOL = 
DEJATOOL += anonprimitives 
DEJATOOL += transformations
DEJATOOL += pktanon-runtime 
DEJATOOL += errors

AM_RUNTESTFLAGS = TOP_SRC_DIR=$(top_srcdir)

CLEANFILES = *.log *.sum site.bak out.pcap

############################################################################
# some stuff for development
############################################################################

# .PHONY check_local
# check_local: 
# 	-sudo killall pktanon
# 	-killall pktanon
# 	sudo ../src/pktanon -v test-config-n.xml 
#	sudo ../src/pktanon -v test-config-n.xml 
# 	../src/pktanon -v test-config-n.xml 
# 	-sudo killall tcpdump
# 	sudo tcpdump -i enp2s1 -s 512 -w - | ../src/pktanon -v ./settings.xml

