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
{
  //
  // = TITLE
  //  Manager for the pool of ACE_ReactorTask.
  //
  // = DESCRIPTION
  //   The EventChannel uses a pool of ACE_ReactorTask to handle the
  //   dispatching of timeouts. In real-time multi-threaded enviroments
  //   this maps to a different thread per priority.
  //   This class offers a centralized access point to those tasks and
  //   some related services.
  //
public:
  ACE_Task_Manager (void);
  // Create the Task_Manager.

  ~ACE_Task_Manager (void);
  // The dtor also shutdowns the Task_Manager.

  void activate (void);
  // Activate the threads, it waits until the threads are up and
  // running.

  void shutdown (void);
  // Deactivate the threads, it waits until all the threads have
  // terminated.

  typedef ACE_ES_Reactor_Task ReactorTask;

  ReactorTask* GetReactorTask(RtecScheduler::OS_Priority priority);
  // Obtain the ReactorTask for the given priority.
  // The Task must have been created already.

  ACE_RT_Thread_Manager* ThrMgr();
  // Returns a global ThreadManager for the Task pool.

private:
  ReactorTask *reactorTasks[ACE_Scheduler_MAX_PRIORITIES];
  // The set of ReactorTasks

  ACE_RT_Thread_Manager thr_mgr;
  // The thread manager.
};

#if defined (__ACE_INLINE__)
#include "Task_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* TASK_MANAGER_H */
