/* -*- C++ -*- */
//
// $Id$
//

#if !defined(TASK_MANAGER_H)
#define TASK_MANAGER_H

#include "ace/ACE.h"
#include "ace/Singleton.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "RT_Task.h"

class ACE_ES_Reactor_Task;

class TAO_ORBSVCS_Export ACE_Task_Manager 
// = TITLE
//   Singleton class for the pool of ACE_ReactorTask.
//
// = DESCRIPTION
//   The EventChannel uses a pool of ACE_ReactorTask to handle the
//   dispatching of Events. In real-time multi-threaded enviroments
//   this maps to a different thread per priority.
//   This class offers a centralized access point to those tasks and
//   some related services.
//
{
public:
  typedef ACE_ES_Reactor_Task ReactorTask;

  static ACE_Task_Manager* instance();
  // Returns the singleton.

  ReactorTask* GetReactorTask(RtecScheduler::OS_Priority priority);
  // Obtain the ReactorTask for the given priority.
  // The Task must have been created already.

  ACE_RT_Thread_Manager* ThrMgr();
  // Returns a global ThreadManager for the Task pool.

private:
  friend class ACE_Singleton<ACE_Task_Manager,ACE_SYNCH_MUTEX>;
  ACE_Task_Manager();

  void initialize();

private:
  ReactorTask *reactorTasks[ACE_Scheduler_MAX_PRIORITIES];
  ACE_RT_Thread_Manager thr_mgr;
};

#if defined (__ACE_INLINE__)
#include "Task_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* TASK_MANAGER_H */
