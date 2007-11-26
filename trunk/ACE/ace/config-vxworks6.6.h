//* -*- C++ -*- */
// $Id$

// The following configuration file is designed to work for VxWorks
// 6.6 platforms using one of these compilers:
// 1) The GNU g++ compiler that is shipped with VxWorks 6.6
// 2) The Diab compiler that is shipped with VxWorks 6.6

#ifndef ACE_CONFIG_VXWORKS_6_6_H
#define ACE_CONFIG_VXWORKS_6_6_H
#include /**/ "ace/pre.h"

#if !defined (ACE_VXWORKS)
# define ACE_VXWORKS 0x660
#endif /* ! ACE_VXWORKS */

#include "ace/config-vxworks6.5.h"

#if defined (__RTP__)
# define ACE_HAS_GETIFADDRS
#endif

#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_VXWORKS_6_6_H */

