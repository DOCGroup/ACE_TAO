/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ipc.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_OS_INCLUDE_SYS_IPC_H
# define ACE_OS_INCLUDE_SYS_IPC_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Only include if platform/compiler provide the header.
# if !defined (ACE_LACKS_SYS_IPC_H)
#   include /**/ <sys/ipc.h>
# endif /* !ACE_LACKS_SYS_IPC_H */

# if !defined (IPC_CREAT)
#   define IPC_CREAT 0
# endif /* IPC_CREAT */

# if !defined (IPC_NOWAIT)
#   define IPC_NOWAIT 0
# endif /* IPC_NOWAIT */

# if !defined (IPC_RMID)
#   define IPC_RMID 0
# endif /* IPC_RMID */

# if !defined (IPC_EXCL)
#   define IPC_EXCL 0
# endif /* IPC_EXCL */

# if !defined (IPC_PRIVATE)
#   define IPC_PRIVATE ACE_INVALID_SEM_KEY
# endif /* IPC_PRIVATE */

# if !defined (IPC_STAT)
#   define IPC_STAT 0
# endif /* IPC_STAT */

#endif /* ACE_OS_INCLUDE_SYS_IPC_H */
