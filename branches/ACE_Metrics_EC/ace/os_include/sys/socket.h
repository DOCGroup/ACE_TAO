// -*- C++ -*-

//=============================================================================
/**
 *  @file    socket.h
 *
 *  main sockets header
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_SOCKET_H
#define ACE_OS_INCLUDE_SYS_SOCKET_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/uio.h"

#if !defined (ACE_LACKS_SYS_SOCKET_H)
# include /**/ <sys/socket.h>
#endif /* !ACE_LACKS_SYS_SOCKET_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_SOCKET_H */
