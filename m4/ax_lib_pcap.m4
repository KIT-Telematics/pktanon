# SYNOPSIS
#
#   AX_LIB_PCAP([ACTION-IF-TRUE], [ACTION-IF-FALSE])
#
# DESCRIPTION
#
#   This macro will check for the existence of libpcap
#   (http://www.tcpdump.org/). It does this by checking for the header
#   file pcap/pcap.h and the pcap library object file. A --with-libpcap
#   option is supported as well. The following output variables are set
#   with AC_SUBST:
#
#     PCAP_CPPFLAGS
#     PCAP_LDFLAGS
#     PCAP_LIBS
#
#   You can use them like this in Makefile.am:
#
#     AM_CPPFLAGS = $(PCAP_CPPFLAGS)
#     AM_LDFLAGS = $(PCAP_LDFLAGS)
#     program_LDADD = $(PCAP_LIBS)
#
#   Additionally, the C preprocessor symbol HAVE_LIBPCAP will be defined
#   with AC_DEFINE if libpcap is available.
#
# LICENSE
#
#   Derived from AX_LIB_UPNP which is Copyright (c) 2009 Oskar Liljeblad
#   <oskar@osk.mine.nu> by Philipp Kern <pkern@debian.org> in 2011.
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 5

AU_ALIAS([AC_LIB_PCAP], [AX_LIB_PCAP])
AC_DEFUN([AX_LIB_PCAP], [
  AH_TEMPLATE([HAVE_LIBPCAP], [Define if libpcap is available])
  AC_ARG_WITH(libpcap, [  --with-libpcap=DIR      prefix for pcap library files and headers], [
    if test "$withval" = "no"; then
      ac_pcap_path=
      $2
    elif test "$withval" = "yes"; then
      ac_pcap_path=/usr
    else
      ac_pcap_path="$withval"
    fi
  ],[ac_pcap_path=/usr])
  if test "$ac_pcap_path" != ""; then
    saved_CPPFLAGS="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS -I$ac_pcap_path/include"
    AC_CHECK_HEADER([pcap/pcap.h], [
      saved_LDFLAGS="$LDFLAGS"
      LDFLAGS="$LDFLAGS -L$ac_pcap_path/lib"
      AC_CHECK_LIB(pcap, pcap_create, [
        AC_SUBST(PCAP_CPPFLAGS, [-I$ac_pcap_path/include])
        AC_SUBST(PCAP_LDFLAGS, [-L$ac_pcap_path/lib])
        AC_SUBST(PCAP_LIBS, [-lpcap])
	AC_DEFINE([HAVE_LIBPCAP])
        $1
      ], [
	:
        $2
      ])
      LDFLAGS="$saved_LDFLAGS"
    ], [
      AC_MSG_RESULT([not found])
      $2
    ])
    CPPFLAGS="$saved_CPPFLAGS"
  fi
])
