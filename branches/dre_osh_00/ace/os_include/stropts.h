// -*- C++ -*-

//=============================================================================
/**
 *  @file    stropts.h
 *
 *  STREAMS interface (STREAMS)
 *
 *  $Id$
 *
 *  @author Don Hinton <dhinton@ieee.org>
 *  @author This code was originally in various places including ace/OS.h.
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_STROPTS_H
#define ACE_OS_INCLUDE_STROPTS_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/unistd.h"

#if !defined (ACE_LACKS_SYS_IOCTL_H)
# include /**/ <sys/ioctl.h>
#endif /* !ACE_LACKS_IOCTL_H */

#if !defined (ACE_LACKS_STROPTS_H)
# include /**/ <stropts.h>
#endif /* !ACE_LACKS_STROPTS_H */

# if defined (ACE_LACKS_STRRECVFD)
struct strrecvfd {};
# endif /* ACE_LACKS_STRRECVFD */

#include "ace/post.h"
#endif /* ACE_OS_INCLUDE_STROPTS_H */
