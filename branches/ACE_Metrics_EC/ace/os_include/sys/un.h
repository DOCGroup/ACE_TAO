// -*- C++ -*-

//=============================================================================
/**
 *  @file    un.h
 *
 *  definitions for UNIX domain sockets
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_UN_H
#define ACE_OS_INCLUDE_SYS_UN_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/socket.h"

#if !defined (ACE_LACKS_SYS_UN_H)
# include /**/ <sys/un.h>
#endif /* !ACE_LACKS_SYS_UN_H */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_UN_H */
