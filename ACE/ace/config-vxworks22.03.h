/* -*- C++ -*- */
// The following configuration file is designed to work for VxWorks
// 22.03 platforms using one of these compilers:
// 1) The llvm compiler that is shipped with VxWorks 22.03

#ifndef ACE_CONFIG_VXWORKS_22_03_H
#define ACE_CONFIG_VXWORKS_22_03_H
#include /**/ "ace/pre.h"

#include "ace/config-vxworks.h"

#if !defined (__RTP__)
# undef ACE_MKDIR_LACKS_MODE
# define ACE_LACKS_GETPID
#endif

#define ACE_HAS_SOCKLEN_T

#undef ACE_LACKS_GETSERVBYNAME
#undef ACE_LACKS_GETPROTOBYNAME
#undef ACE_LACKS_GETPROTOBYNUMBER
#undef ACE_LACKS_GETIPNODEBYADDR
#undef ACE_LACKS_LSTAT

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_VXWORKS_22_03_H */
