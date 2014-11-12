AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -O2  -pipe -Werror=return-type -I$(top_srcdir)/include 
LDADD = $(libnettle_LIBS) $(libhogweed_LIBS) $(xerces_LIBS) 

# debug version:
AM_CPPFLAGS += -g 
AM_LDFLAGS = -g 
# AM_CPPFLAGS += -DNOXMLCONFIG -DTRACE_ENABLED
AM_CPPFLAGS += -DTRACE_ENABLED

if USE_LIBPCAP
AM_CPPFLAGS += -DHAVE_LIBPCAP
LDADD += -lpcap
endif 
