// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_inet.h
 *
 *  definitions for internet operations
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_ARPA_OS_INET_H
#define ACE_OS_INCLUDE_ARPA_OS_INET_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/netinet/in.h"

#if !defined (ACE_LACKS_ARPA_INET_H)
   extern "C" {
#  include /**/ <arpa/inet.h>
   }
#endif /* !ACE_LACKS_ARPA_INET_H */

// @todo move the ACE_HTONL, etc macros here

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_ARPA_OS_INET_H */
