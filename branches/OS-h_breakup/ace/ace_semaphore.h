/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_semaphore.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SEMAPHORE_H
#define ACE_ACE_SEMAPHORE_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// These hooks enable ACE to have all dynamic memory management
// automatically handled on a per-object basis.

# if defined (ACE_LACKS_KEY_T)
#   if defined (ACE_WIN32)
   // Win32 doesn't use numeric values to name its semaphores, it uses
   // strings!
typedef char *key_t;
#   else
typedef int key_t;
#   endif /* ACE_WIN32 */
# endif /* ACE_LACKS_KEY_T */

# if defined (ACE_HAS_POSIX_SEM)
#   include /**/ <semaphore.h>
#   if !defined (SEM_FAILED) && !defined (ACE_LACKS_NAMED_POSIX_SEM)
#     define SEM_FAILED ((sem_t *) -1)
#   endif  /* !SEM_FAILED */

typedef struct
{
  sem_t *sema_;
  // Pointer to semaphore handle.  This is allocated by ACE if we are
  // working with an unnamed POSIX semaphore or by the OS if we are
  // working with a named POSIX semaphore.

  char *name_;
  // Name of the semaphore (if this is non-NULL then this is a named
  // POSIX semaphore, else its an unnamed POSIX semaphore).

#   if defined (ACE_LACKS_NAMED_POSIX_SEM)
  int new_sema_;
  // this->sema_ doesn't always get created dynamically if a platform
  // doesn't support named posix semaphores.  We use this flag to
  // remember if we need to delete <sema_> or not.
#   endif /* ACE_LACKS_NAMED_POSIX_SEM */
} ACE_sema_t;
# endif /* !ACE_WIN32 */

#endif /* ACE_ACE_SEMAPHORE_H */

