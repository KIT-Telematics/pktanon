AC_PREREQ(2.68)
AC_INIT([pktanon], [2.0-beta], [bless@kit.edu])

AM_INIT_AUTOMAKE([-Wall, -Werror foreign])
AC_CONFIG_SRCDIR([src/Main.cpp])
AC_CONFIG_MACRO_DIRS([m4])

## by default non-verbose output, override with make V=1
AM_SILENT_RULES

#################################################################################
# Source Code Config
#################################################################################

AC_PROG_CXX
AM_PROG_AR
AC_PROG_RANLIB
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG 

AC_CHECK_HEADERS([stddef.h],[],[AC_MSG_ERROR(["missing required headers"])])

AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

#xerces-c is required for configuration
PKG_CHECK_MODULES([xerces], [xerces-c])

# libnettle is required for hash functions
PKG_CHECK_MODULES([libnettle], [nettle], 
  [havenettle=true], 
  [AC_MSG_WARN([[
    *****
    libnettle was not found... AnonHashSha* will not be available
    *****
    ]])]
)

AM_CONDITIONAL([USE_NETTLE], [test x$havenettle = xtrue])

# include bytewise hash primitives (disabled by default)
AC_ARG_ENABLE(bytewisehashanons,
  [[
  --enable-bytewisehashanons  enables AnonBytewiseHashSha1 and AnonBytewiseHashHmacSha1
  ** WARNING ** these primitives are likely not secure!!
  ]],
  [case "${enableval}" in
    yes)
      # check if crypto library is present
      if [test x$havenettle = xtrue];
      then
	bytewisehashanons=true
      else 
      AC_MSG_ERROR([[
	*****
	cannot enable AnonBytewiseHash* primitives... crypto library not found...
	please disable bytewisehashanons
	*****
	]])
      fi
      ;;
    no)  
      bytewisehashanons=false ;;
    *) 
      AC_MSG_ERROR(bad value ${enableval} for --enable-bytewisehashanons) ;;
  esac],
  [bytewisehashanons=false])

  AM_CONDITIONAL(BYTEWISEHASHANONS, test x$bytewisehashanons = xtrue)

# check requirements for raw sockets  
  
# check requirements for (inet) sockets
AC_CHECK_FUNCS([socket],[],[AC_MSG_ERROR(["require sockets"])])
AC_CHECK_HEADERS([sys/socket.h arpa/inet.h],[],[AC_MSG_ERROR(["missing headers required for sockets"])])
# TODO: include conditionally #
# AM_CONDITIONAL([USE_SOCKETOUTPUT], [test "ac_cv_func_socket" = yes -a "ac_cv_header_sys_socket_h" = yes ])
  
# check requirements for raw sockets
AC_CHECK_HEADERS([sys/ioctl.h netinet/in.h linux/if_ether.h netpacket/packet.h net/if.h sys/ioctl.h fcntl.h],[],[AC_MSG_ERROR(["missing headers required for raw sockets"])])
# TODO: include conditionally #

# check libpcap 
AC_CHECK_HEADERS([pcap/pcap.h])
AC_CHECK_LIB([pcap],[pcap_create],[echo $ac_cv_lib_pcap_pcap_create])
AM_CONDITIONAL([USE_LIBPCAP], [test "$ac_cv_lib_pcap_pcap_create" = yes])


# libhogweed is a part of libnettle that depends on glib for bigint values,  it might be needed for 
# public key cryptography
# PKG_CHECK_MODULES([libhogweed], [hogweed], 
#   [HAVE_HOGWEED=1], 
#   [AC_MSG_WARN([[
#     *****
#     libnettle/libhogweed was not found... AnonHashHmac* will not be available
#     *****
#     ]])]
# )
# AM_CONDITIONAL([INC_NETTLE_HMACS], [test "$cryptolib" -eq "hogweed"])


#################################################################################
# Testing
#####################################################################

#AC_ARG_ENABLE(debug)

#################################################################################
# Last
#####################################################################

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 libpktanon/Makefile
 tests/Makefile
])
AC_OUTPUT
