# $Id$
# LynxOS with g++

# NOTE:  with LynxOS 2.5.0-FCS version: 012797-G, you'll need to
#        apply this patch:
# ftp://ftp.lynx.com/pub/patches/v2.5-patches/v2.5-on-x86/000134-00.tar.Z
#
# And, you'll need to apply this patch to /usr/include/netinet/in.h:
# --- in.h.ORIGINAL	Sat Feb  8 09:58:28 1997
# +++ in.h	Wed Nov 26 15:41:04 1997
# @@ -278,8 +278,7 @@
#  	compiler, the macros are not defined:  Functions must be used.
#  */
#  static __inline__ unsigned short
# -htons(arg)
# -unsigned short arg;
# +htons(unsigned short arg)
#  {
#  	unsigned short retval;
# 
# @@ -292,8 +291,7 @@
#  }
# 
#  static __inline__ unsigned long
# -htonl(arg)
# -unsigned long arg;
# +htonl(unsigned long arg)
#  {
# 	unsigned long retval;

debug = 1
shared_libs =
static_libs = 1

CC              = gcc
CXX             = g++
CFLAGS          += -mthreads -pipe      #### -Wall -Wpointer-arith -Winline
CCFLAGS         += $(CFLAGS) -fno-implicit-templates -fno-strict-prototype
DCFLAGS         += -g
DLD             = $(CXX)
LD              = $(CXX)
LIBS            += -lnetinet -lnsl
OCFLAGS         += -O2
AR              = ar
ARFLAGS         = ruv
RANLIB          = @true
PRELIB          = @true
