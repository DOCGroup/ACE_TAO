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
#
# And this patch to /usr/include/name.h:
# --- name.h.ORIGINAL	Sat Feb  8 09:57:36 1997
# +++ name.h	Wed Nov 26 22:38:28 1997
# @@ -112,7 +112,7 @@
#  static __inline__ int _name_fchmod(int nsid, mode_t mode)
#  { return _name_server(_NS_FCHMOD, nsid, mode); }
#
# -static __inline__ int _name_fchown(int nsid, uid_t owner, gid_t group)
# +static __inline__ int _name_fchown(int /* nsid */, uid_t owner, gid_t group)
#  { return _name_server(_NS_FCHOWN, owner, group); }
#
#  static __inline__ int _name_close(int nsid)
#
# And this patch to /usr/include/scsi.h:
# --- scsi.h.ORIGINAL	Sat Feb  8 09:57:43 1997
# +++ scsi.h	Mon Dec  1 09:13:13 1997
# @@ -166,7 +166,7 @@
#  ------------------------------------------------------------------------------
#  */
#  #define GET_DATA_BUFFER_STATUS          0x34	/* O     14.2.1 */
# -#define GET WINDOW                      0x25	/* O     14.2.2 */
# +#define GET_WINDOW                      0x25	/* O     14.2.2 */
#  #define OBJECT_POSITION                 0x31	/* O     14.2.3 */
#  #define SCAN                            0x1B	/* O     14.2.5 */
#  #define SET_WINDOW                      0x24	/* M     14.2.6 */

debug = 1
shared_libs =
static_libs = 1

CC              = gcc
CXX             = g++
CFLAGS          += -mthreads -pipe -Wall -Wpointer-arith #### -Winline
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
