// -*- C++ -*-

//=============================================================================
/**
 *  @file    os_stropts.h
 *
 *  STREAMS interface (STREAMS)
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@dresystems.com>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_OS_STROPTS_H
#define ACE_OS_INCLUDE_OS_STROPTS_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_unistd.h"

#if defined (ACE_HAS_TIMOD_H)
#  if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#    define queue _Queue_
#  endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#  include /**/ <sys/timod.h>
#  if defined (ACE_HAS_STL_QUEUE_CONFLICT)
#    undef queue
#  endif /* ACE_HAS_STL_QUEUE_CONFLICT */
#elif defined (ACE_HAS_OSF_TIMOD_H)
#  include /**/ <tli/timod.h>
#endif /* ACE_HAS_TIMOD_H */

#if !defined (ACE_LACKS_SYS_IOCTL_H)
#  include /**/ <sys/ioctl.h>
#endif /* !ACE_LACKS_IOCTL_H */

#if defined (ACE_HAS_SYS_FILIO_H)
#  include /**/ <sys/filio.h>
#endif /* ACE_HAS_SYS_FILIO_H */

#if defined (ACE_HAS_SOCKIO_H)
#  include /**/ <sys/sockio.h>
#endif /* ACE_HAS_SOCKIO_ */

#if !defined (ACE_LACKS_STROPTS_H)
#  include /**/ <stropts.h>
#endif /* !ACE_LACKS_STROPTS_H */

#if defined (VXWORKS)
// for ioctl()
#  include /**/ <ioLib.h>
#endif /* VXWORKS */

// Place all additions (especially function declarations) within extern "C" {}
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if defined (ACE_LACKS_STRRECVFD)
   struct strrecvfd {};
#endif /* ACE_LACKS_STRRECVFD */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_OS_STROPTS_H */
