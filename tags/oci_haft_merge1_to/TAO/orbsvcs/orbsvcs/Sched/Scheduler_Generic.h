/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    Scheduler_Generic.h
//
// = CREATION DATE
//    19 November 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#ifndef SCHEDULER_INTERNAL_H
#define SCHEDULER_INTERNAL_H
#include "ace/pre.h"

#include "Scheduler.h"
#include "ace/Unbounded_Set.h"

class TAO_RTSched_Export Scheduler_Generic : public ACE_Scheduler
  // = TITLE
  //    Implementation of an off-line scheduler.
  //
  // = DESCRIPTION
  //    Schedules tasks, assigning the same priority to all of them.
{
public:
  Scheduler_Generic ();
  virtual ~Scheduler_Generic ();

  // = Initialize the scheduler.
  virtual void init (const int minimum_priority,
                     const int maximum_priority,
                     const char *runtime_filename = 0,
                     const char *rt_info_filename = 0,
                     const char *timeline_filename = 0);

  // = Registers a task.
  virtual status_t register_task (RT_Info *[],
                                  const u_int number_of_modes,
                                  handle_t &handle);

  virtual status_t lookup_rt_info (handle_t handle,
                                   RT_Info* &rtinfo);
  // Obtains an RT_Info based on its "handle".

  virtual status_t lookup_config_info (Preemption_Priority priority,
                                       Config_Info* &config_info);
  // Obtains a Config_Info based on its priority.

  // = Computes the schedule.
  virtual status_t
    schedule (ACE_Unbounded_Set<Scheduling_Anomaly *> &anomaly_set);

  // = Access a thread priority.
  virtual int priority (const handle_t handle,
                        OS_Thread_Priority &priority,
                        Sub_Priority &subpriority,
                        Preemption_Priority &preemption_prio,
                        const mode_t = CURRENT_MODE) const;
  // Defines "priority" as the priority that was assigned to the Task that
  // was assigned "handle", for the specified mode.  Defines "subpriority"
  // as the relative ordering (due to dependencies) within the priority.
  // Returns 0 on success, or 1 if an invalid mode or handle are supplied.

private:
  u_int handles_;
  // The number of task handles dispensed so far.

  int minimum_priority_;
  // The minimum priority value that the application specified (in
  // its call to init ()).

  int maximum_priority_;
  // The maximum priority value that the application specified (in
  // its call to init ()).

  const char *runtime_filename_;
  // Destination file of Scheduler output from the configuration run.

  const char *rt_info_filename_;
  // Destination file of all rt_info data from the configuration run.

  const char *timeline_filename_;
  // The destination of the timeline.

  int increasing_priority_;
  // Set to 1 if priority values increase with increasing priority,
  // such as on Solaris and Win32, or 0 if they decrease, such as on
  // VxWorks.

  ACE_Unbounded_Set <RT_Info **> task_entries_;
  // Collection of known tasks.

  Config_Info config_info_;
  // dispatchin configuration info

#if defined (ACE_HAS_THREADS)
  typedef TAO_SYNCH_RECURSIVE_MUTEX LOCK;
#else
  typedef ACE_Null_Mutex LOCK;
#endif /* ACE_HAS_THREADS */

  LOCK lock_;
  // This protects access to the scheduler during configuration runs.


  ///////////////////////////////////////
  // member functions for internal use //
  ///////////////////////////////////////

  void reset ();
  // Prepare for another schedule computation, but do not
  // disturb the "output" (priorities that have already been assigned).

  void print_schedule ();
  // Display the schedule, task-by-task.

  ACE_UNIMPLEMENTED_FUNC (Scheduler_Generic (const Scheduler_Generic &))
  ACE_UNIMPLEMENTED_FUNC (Scheduler_Generic &operator= (
    const Scheduler_Generic &))
};


#if defined (__ACE_INLINE__)
#include "Scheduler_Generic.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* SCHEDULER_INTERNAL_H */


// EOF
