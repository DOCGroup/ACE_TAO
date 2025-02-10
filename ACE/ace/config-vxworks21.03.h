/* -*- C++ -*- */
// The following configuration file is designed to work for VxWorks
// 21.03 platforms using one of these compilers:
// 1) The llvm compiler that is shipped with VxWorks 21.03

#ifndef ACE_CONFIG_VXWORKS_21_03_H
#define ACE_CONFIG_VXWORKS_21_03_H
#include /**/ "ace/pre.h"

#include "ace/config-vxworks7.0.h"

#if defined (__RTP__)
# include "semLibCommon.h"
# define ACE_LACKS_IF_NAMEINDEX
# define ACE_LACKS_FGETWC
#else
# undef ACE_MKDIR_LACKS_MODE
# define ACE_LACKS_GETPID
#endif

#undef  ACE_SIZEOF_WCHAR
#define ACE_SIZEOF_WCHAR 4

#define ACE_HAS_SOCKLEN_T

#undef ACE_LACKS_GETSERVBYNAME
#undef ACE_LACKS_GETPROTOBYNAME
#undef ACE_LACKS_GETPROTOBYNUMBER
#undef ACE_LACKS_GETIPNODEBYADDR
#undef ACE_LACKS_LSTAT
#undef ACE_LACKS_MKFIFO
#undef ACE_LACKS_MKSTEMP
#undef ACE_LACKS_READLINK
#undef ACE_LACKS_REALPATH
#undef ACE_LACKS_PIPE
#undef ACE_LACKS_UMASK
#undef ACE_LACKS_TEMPNAM
#undef ACE_HAS_SIGTIMEDWAIT
#undef ACE_HAS_SIGSUSPEND
#undef ACE_HAS_GETIFADDRS

#undef ACE_LACKS_SETEGID
#undef ACE_LACKS_SETUID
#undef ACE_LACKS_SETEUID
#undef ACE_LACKS_GETEUID
#undef ACE_LACKS_GETUID
#undef ACE_LACKS_GETEGID
#undef ACE_LACKS_GETGID
#undef ACE_LACKS_SETGID

#undef ACE_LACKS_SYS_UIO_H
#undef ACE_LACKS_TERMIOS_H
#undef ACE_LACKS_FCNTL
#undef ACE_HAS_CHARPTR_SOCKOPT

#undef ACE_LACKS_INTPTR_T
#undef ACE_LACKS_INTTYPES_H
#undef ACE_LACKS_STDINT_H
#undef ACE_LACKS_SYS_TIME_H
#undef ACE_LACKS_SYS_SELECT_H
#undef ACE_HAS_SIZET_PTR_ASCTIME_R_AND_CTIME_R
#undef ACE_LACKS_SEARCH_H

#undef ACE_LACKS_TZSET
#undef ACE_LACKS_ISWCTYPE
#undef ACE_LACKS_ISBLANK

#undef ACE_LACKS_RAND_R

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_VXWORKS_21_03_H */
