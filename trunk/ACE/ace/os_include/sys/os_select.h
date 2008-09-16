// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_select.h
 *
 *  select types
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_OS_SELECT_H
#define ACE_OS_INCLUDE_SYS_OS_SELECT_H

#include /**/ "ace/pre.h"

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/sys/os_time.h"
#include "ace/os_include/os_signal.h"
#include "ace/os_include/os_unistd.h"

#if !defined (ACE_LACKS_SYS_SELECT_H)
#  include /**/ <sys/select.h>
#endif /* !ACE_LACKS_SYS_SELECT_H */

#if defined (ACE_VXWORKS) && defined (ACE_LACKS_SYS_SELECT_H)
#  include /**/ <selectLib.h>
#endif /* ACE_VXWORKS */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_LACKS_FD_MASK)
   typedef long fd_mask;
#endif /* __QNX__ */

#if defined (ACE_WIN32)
   // This will help until we figure out everything:
#  define NFDBITS 32 /* only used in unused functions... */
#elif defined (ACE_LACKS_NFDBITS)
#  define NFDBITS (sizeof(fd_mask) * NBBY)        /* bits per mask */
#endif /* ACE_WIN32 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include /**/ "ace/post.h"
#endif /* ACE_OS_INCLUDE_SYS_OS_SELECT_H */
