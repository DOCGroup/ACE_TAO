/* -*- C++ -*- */
//
// $Id$
//

#ifndef TAO_EC_TIMER_MODULE_H
#define TAO_EC_TIMER_MODULE_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/RT_Task.h"

class ACE_Command_Base;

class TAO_ORBSVCS_Export TAO_EC_Timer_Module
{
  // = TITLE
  //   The timer managment module.
  //
  // = DESCRIPTION
  //   The Event Channel can use several strategies to dispatch
  //   timers, for instance, it can use the ORB reactor or a pool of
  //   reactors running at different priorities or a pool of
  //   Thread_Timer_Queue_Adapters running at different priorities
  //   also.
  //   This class is the abstract base class to abstract this
  //   strategies.
  //
public:
  virtual ~TAO_EC_Timer_Module (void);
  // The dtor

  virtual void activate (void) = 0;
  // Activate the threads, it waits until the threads are up and
  // running.

  virtual void shutdown (void) = 0;
  // Deactivate the threads, it waits until all the threads have
  // terminated.

  virtual RtecScheduler::handle_t
     rt_info (RtecScheduler::OS_Priority priority) = 0;
  // The RT_Info handle for the "task" at <priority>

  virtual int schedule_timer (RtecScheduler::Preemption_Priority_t priority,
                              ACE_Command_Base* act,
                              const ACE_Time_Value& delta,
                              const ACE_Time_Value& interval) = 0;
  // Add a timer at the given priority, returns the timer ID.

  virtual int cancel_timer (RtecScheduler::Preemption_Priority_t priority,
                            int id,
                            ACE_Command_Base*& act) = 0;
  // Add a timer at the given priority.

  virtual int register_handler (RtecScheduler::Preemption_Priority_t priority,
                                ACE_Event_Handler* eh,
                                ACE_HANDLE handle) = 0;
  // Register a handler?????

  virtual ACE_Reactor* reactor (RtecScheduler::Preemption_Priority_t priority) = 0;
  // Obtain the reactor for the given priority.
  // @@ This may prove tricky to implement with timer queues not based
  // on reactors.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Timeout_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   Event Service Timeout handler.
  //
  // = DESCRIPTION
  //   This is used by the Timer_Modules as an adaptor between the
  //   reactor (Event_Handler) and the Command objects.
  //
public:
  TAO_EC_Timeout_Handler (void);
  // Default construction.

private:
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *act);
  // Casts <act> to ACE_Command_Base and calls execute.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_ST_Timer_Module : public TAO_EC_Timer_Module
{
  // = TITLE
  //   A single threaded implementation for the timer module.
  //
  // = DESCRIPTION
  //   This timer module uses a single Reactor to implement the timer,
  //   usually the ORB reactor is used for this purposes.
  //
public:
  TAO_EC_ST_Timer_Module (ACE_Reactor* reactor);
  // The ctor.

  virtual ~TAO_EC_ST_Timer_Module (void);
  // The dtor

  // = The TAO_EC_Timer_Module methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual RtecScheduler::handle_t
     rt_info (RtecScheduler::Preemption_Priority_t priority);
  virtual int schedule_timer (RtecScheduler::Preemption_Priority_t priority,
                              ACE_Command_Base* act,
                              const ACE_Time_Value& delta,
                              const ACE_Time_Value& interval);
  virtual int cancel_timer (RtecScheduler::Preemption_Priority_t priority,
                            int id,
                            ACE_Command_Base*& act);
  virtual int register_handler (RtecScheduler::Preemption_Priority_t priority,
                                ACE_Event_Handler* eh,
                                ACE_HANDLE handle);
  virtual ACE_Reactor* reactor (RtecScheduler::Preemption_Priority_t priority);

private:
  ACE_Reactor* reactor_;
  // The reactor.

  TAO_EC_Timeout_Handler timeout_handler_;
  // To receive the timeouts.
};

// ****************************************************************

class ACE_ES_Reactor_Task;

class TAO_ORBSVCS_Export TAO_EC_RPT_Timer_Module : public TAO_EC_Timer_Module
{
  //
  // = TITLE
  //   A timer module using reactor-per-thread.
  //
  // = DESCRIPTION
  //   This Timer Module uses a pool of ACE_ReactorTask to handle the
  //   dispatching of timeouts. In real-time multi-threaded enviroments
  //   each Reactor runs at a different priority.
  //
public:
  TAO_EC_RPT_Timer_Module (RtecScheduler::Scheduler_ptr scheduler);
  // Create the Timer Module

  virtual ~TAO_EC_RPT_Timer_Module (void);
  // The dtor also shutdowns the Task_Manager.

  typedef ACE_ES_Reactor_Task ReactorTask;

  ReactorTask* GetReactorTask(RtecScheduler::Preemption_Priority_t priority);
  // Obtain the ReactorTask for the given priority.
  // The Task must have been created already.

  ACE_RT_Thread_Manager* ThrMgr();
  // Returns a global ThreadManager for the Task pool.

  // = The TAO_EC_Timer_Module methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual RtecScheduler::handle_t
     rt_info (RtecScheduler::Preemption_Priority_t priority);
  virtual int schedule_timer (RtecScheduler::Preemption_Priority_t priority,
                              ACE_Command_Base* act,
                              const ACE_Time_Value& delta,
                              const ACE_Time_Value& interval);
  virtual int cancel_timer (RtecScheduler::Preemption_Priority_t priority,
                            int id,
                            ACE_Command_Base*& act);
  virtual int register_handler (RtecScheduler::Preemption_Priority_t priority,
                                ACE_Event_Handler* eh,
                                ACE_HANDLE handle);
  virtual ACE_Reactor* reactor (RtecScheduler::Preemption_Priority_t priority);

private:
  int shutdown_;
  // The reactors are shutdown, do not attempt to restart them.

  ReactorTask *reactorTasks[ACE_Scheduler_MAX_PRIORITIES];
  // The set of ReactorTasks

  ACE_RT_Thread_Manager thr_mgr;
  // The thread manager.

  TAO_EC_Timeout_Handler timeout_handler_;
  // To receive the timeouts.

  RtecScheduler::Scheduler_var scheduler_;
  // The scheduler.
};

#if defined (__ACE_INLINE__)
#include "Timer_Module.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_TIMER_MODULE_H */
