// $Id$

#include "EC_TPC_Dispatching_Task.h"

//ACE_RCS_ID(RTEvent, EC_TPC_Dispatching_Task, "$Id$")

TAO_EC_TPC_Dispatching_Task::TAO_EC_TPC_Dispatching_Task
(ACE_Thread_Manager* thr_mgr,
 TAO_EC_Queue_Full_Service_Object* so)
  : TAO_EC_Dispatching_Task (thr_mgr, so)
{
}

int
TAO_EC_TPC_Dispatching_Task::close (u_long flags)
{
  ACE_UNUSED_ARG (flags);
  delete this;
  return 0;
}
