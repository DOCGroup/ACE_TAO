// $Id$

#include "orbsvcs/Event/EC_TPC_Dispatching_Task.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_TPC_Dispatching_Task::TAO_EC_TPC_Dispatching_Task
(ACE_Thread_Manager* thr_mgr,
 TAO_EC_Queue_Full_Service_Object* so)
  : TAO_EC_Dispatching_Task (thr_mgr, so)
{
}

int
TAO_EC_TPC_Dispatching_Task::close (u_long /* flags */)
{
  delete this;
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
