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

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
#endif /* !ACE_LACKS_NET_IF_H */

#if defined (ACE_PSOS)
#  if !defined (IFF_LOOPBACK)
#    define IFF_LOOPBACK IFF_EXTLOOPBACK
#  endif /* IFF_LOOPBACK */
#endif /* ACE_PSOS */

#if defined (ACE_HAS_BROKEN_IF_HEADER)
   struct ifafilt;
#endif /* ACE_HAS_BROKEN_IF_HEADER */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_NET_OS_IF_H */
