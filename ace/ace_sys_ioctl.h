/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_ioctl.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_IOCTL_H
# define ACE_ACE_SYS_IOCTL_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_SYS_IOCTL_H)
#   include /**/ <sys/ioctl.h>
# endif /* !ACE_LACKS_SYS_IOCTL_H */

#endif /* ACE_ACE_SYS_IOCTL_H */
