/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_sem.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_SEM_H
# define ACE_ACE_SYS_SEM_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_SYS_SEM_H)
#   include /**/ <sys/sem.h>
# endif /* !ACE_LACKS_SYS_SEM_H */

# if defined (ACE_LACKS_SEMBUF_T)
struct sembuf
{
  unsigned short sem_num; // semaphore #
  short sem_op; // semaphore operation
  short sem_flg; // operation flags
};
# endif /* ACE_LACKS_SEMBUF_T */



# if !defined (GETVAL)
#   define GETVAL 0
# endif /* GETVAL */

# if !defined (F_GETFL)
#   define F_GETFL 0
# endif /* F_GETFL */

# if !defined (SETVAL)
#   define SETVAL 0
# endif /* SETVAL */

# if !defined (GETALL)
#   define GETALL 0
# endif /* GETALL */

# if !defined (SETALL)
#   define SETALL 0
# endif /* SETALL */

# if !defined (SEM_UNDO)
#   define SEM_UNDO 0
# endif /* SEM_UNDO */


# if !defined (ACE_HAS_SEMUN) || (defined (__GLIBC__) && defined (_SEM_SEMUN_UNDEFINED))
union semun
{
  int val; // value for SETVAL
  struct semid_ds *buf; // buffer for IPC_STAT & IPC_SET
  u_short *array; // array for GETALL & SETALL
};
# endif /* !ACE_HAS_SEMUN || (defined (__GLIBC__) && defined (_SEM_SEMUN_UNDEFINED)) */

#endif /* ACE_ACE_SYS_SEM_H */
