// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_tcp.h
 *
 *  definitions for the Internet Transmission Control Protocol (TCP)
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_NETINET_OS_TCP_H
#define ACE_OS_INCLUDE_NETINET_OS_TCP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (ACE_LACKS_NETINET_TCP_H)
# include /**/ <netinet/tcp.h>
#endif /* !ACE_LACKS_NETIINET_TCP_H */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_PSOS)
#  if ! defined (TCP_NODELAY)
#    define TCP_NODELAY  1
#  endif /* TCP_NODELAY */
#endif /* ACE_PSOS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_NETINET_OS_TCP_H */
