// -*- C++ -*-

//=============================================================================
/**
 *  @file    shm.h
 *
 *  XSI shared memory facility
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_SHM_H
#define ACE_OS_INCLUDE_SYS_SHM_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/ipc.h"

#if !defined (ACE_LACKS_SYS_SHM_H)
# include /**/ <sys/shm.h>
#endif /* !ACE_LACKS_SYS_SHM_H */

#if defined (ACE_WIN32)
struct shmaddr { };
#endif /* ACE_WIN32 */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_SHM_H */
