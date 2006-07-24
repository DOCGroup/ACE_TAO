// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_if.h
 *
 *  sockets local interfaces
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_NET_OS_IF_H
#define ACE_OS_INCLUDE_NET_OS_IF_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_NEEDS_IPC_1C_H)
   // LynxOS 3.1.0 and later need this
#  include /**/ <ipc_1c.h>
#endif /* ACE_NEEDS_IPC_1C_H */

#if !defined (ACE_LACKS_NET_IF_H)
   // This part if to avoid STL name conflict with the map structure
   // in net/if.h.
#  if defined (ACE_HAS_STL_MAP_CONFLICT)
#    define map _Resource_Allocation_Map_
#  endif /* ACE_HAS_STL_MAP_CONFLICT */
   extern "C" {
#  include /**/ <net/if.h>
   }
#  if defined (ACE_HAS_STL_MAP_CONFLICT)
#    undef map
#  endif /* ACE_HAS_STL_MAP_CONFLICT */
#  if defined (HPUX) && defined (IOR)
   /* HP-UX 11.11 defines IOR in /usr/include/pa/inline.h
      and we don't want that definition.  See IOP_IORC.h.
      Thanks to Torsten Kopper <tkue_0931@fastmail.fm> for this patch.*/
#    undef IOR
#  endif /* HPUX && IOR */
#endif /* !ACE_LACKS_NET_IF_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_HAS_BROKEN_IF_HEADER)
   struct ifafilt;
#endif /* ACE_HAS_BROKEN_IF_HEADER */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_NET_OS_IF_H */
