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
#define ACE_ACE_SYS_SEM_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ <sys/sem.h>

# if defined (ACE_LACKS_SEMBUF_T)
struct sembuf
{
  unsigned short sem_num; // semaphore #
  short sem_op; // semaphore operation
  short sem_flg; // operation flags
};
# endif /* ACE_LACKS_SEMBUF_T */

#endif /* ACE_ACE_SYS_SEM_H */
