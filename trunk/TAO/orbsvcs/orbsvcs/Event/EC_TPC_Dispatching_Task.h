/* -*- C++ -*- */
/**
 *  @file   EC_TPC_Dispatching_Task.h
 *
 *  $Id$
 *
 *  @author Chris Cleeland <cleeland at ociweb.com>
 */

#ifndef TAO_EC_TPC_DISPATCHING_TASK_H
#define TAO_EC_TPC_DISPATCHING_TASK_H
#include "ace/pre.h"

#include "EC_Dispatching_Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTEvent_Serv_Export TAO_EC_TPC_Dispatching_Task : public TAO_EC_Dispatching_Task
{
public:
  /// Constructor
  TAO_EC_TPC_Dispatching_Task (ACE_Thread_Manager* thr_mgr, TAO_EC_Queue_Full_Service_Object* so);
  virtual int close (u_long flags = 0);
};

#include "ace/post.h"
#endif /* TAO_EC_TPC_DISPATCHING_TASK_H */
