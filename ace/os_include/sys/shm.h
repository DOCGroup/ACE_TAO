/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    shm.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_SHM_H
# define ACE_OS_INCLUDE_SYS_SHM_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_SYS_SHM_H)
#   include /**/ <sys/shm.h>
# endif /* !ACE_LACKS_SYS_SHM_H */

#endif /* ACE_OS_INCLUDE_SYS_SHM_H */
