/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Asynch_Pseudo_Task.h
 *
 *  $Id$
 *
 *  @author Alexander Libman <alibman@ihug.com.au>
 */
//=============================================================================

#ifndef ACE_ASYNCH_PSEUDO_TASK_H
#define ACE_ASYNCH_PSEUDO_TASK_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Task.h"
#include "ace/Manual_Event.h"


/**
 * @class ACE_Asynch_Pseudo_Task
 *
 */
class ACE_Export ACE_Asynch_Pseudo_Task : public ACE_Task<ACE_SYNCH>
{
  friend class ACE_POSIX_Asynch_Accept;
  friend class ACE_POSIX_Asynch_Connect;
  friend class ACE_WIN32_Asynch_Connect;

public:

  ACE_Asynch_Pseudo_Task();
  virtual ~ACE_Asynch_Pseudo_Task();

  int start (void);
  int stop (void);

  virtual int svc (void);

  int is_active (void);

  int register_io_handler (ACE_HANDLE handle,
                           ACE_Event_Handler *handler,
                           ACE_Reactor_Mask mask,
                           int flg_suspend);

  int remove_io_handler (ACE_HANDLE handle);
  int remove_io_handler (ACE_Handle_Set &set);
  int resume_io_handler (ACE_HANDLE handle);
  int suspend_io_handler (ACE_HANDLE handle);

protected:

  int lock_finish (void);
  int unlock_finish (void);

  int flg_active_;

  ACE_Select_Reactor select_reactor_;  
  // should be initialized before reactor_

  ACE_Reactor reactor_;

  ACE_Lock &token_;

  int              finish_count_;
  ACE_Manual_Event finish_event_;
};

#include /**/ "ace/post.h"
#endif /* ACE_ASYNCH_PSEUDO_TASK_H */
