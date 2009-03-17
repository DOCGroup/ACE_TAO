// -*- C++ -*-

//=============================================================================
/**
 *  @file    Scheduler_Generic.h
 *
 *  $Id$
 *
 *  @author David Levine
 */
//=============================================================================


#ifndef SCHEDULER_INTERNAL_H
#define SCHEDULER_INTERNAL_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Sched/Scheduler.h"
#include "ace/Unbounded_Set.h"
#include "ace/Synch_Traits.h"
#include "ace/Recursive_Thread_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class Scheduler_Generic
 *
 * @brief Implementation of an off-line scheduler.
 *
 * Schedules tasks, assigning the same priority to all of them.
 */
class TAO_RTSched_Export Scheduler_Generic : public ACE_Scheduler
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

  /// Obtains an RT_Info based on its "handle".
  virtual status_t lookup_rt_info (handle_t handle,
                                   RT_Info* &rtinfo);

  /// Obtains a Config_Info based on its priority.
  virtual status_t lookup_config_info (Preemption_Priority priority,
                                       Config_Info* &config_info);

  // = Computes the schedule.
  virtual status_t
    schedule (ACE_Unbounded_Set<Scheduling_Anomaly *> &anomaly_set);

  // = Access a thread priority.
  /**
   * Defines "priority" as the priority that was assigned to the Task that
   * was assigned "handle", for the specified mode.  Defines "subpriority"
   * as the relative ordering (due to dependencies) within the priority.
   * Returns 0 on success, or 1 if an invalid mode or handle are supplied.
   */
  virtual int priority (const handle_t handle,
                        OS_Thread_Priority &priority,
                        Sub_Priority &subpriority,
                        Preemption_Priority &preemption_prio,
                        const mode_t = CURRENT_MODE) const;

private:
  /// The number of task handles dispensed so far.
  u_int handles_;

  /// The minimum priority value that the application specified (in
  /// its call to init ()).
  int minimum_priority_;

  /// The maximum priority value that the application specified (in
  /// its call to init ()).
  int maximum_priority_;

  /// Destination file of Scheduler output from the configuration run.
  const char *runtime_filename_;

  /// Destination file of all rt_info data from the configuration run.
  const char *rt_info_filename_;

  /// The destination of the timeline.
  const char *timeline_filename_;

  /**
   * Set to 1 if priority values increase with increasing priority,
   * such as on Solaris and Win32, or 0 if they decrease, such as on
   * VxWorks.
   */
  int increasing_priority_;

  /// Collection of known tasks.
  ACE_Unbounded_Set <RT_Info **> task_entries_;

  /// dispatchin configuration info
  Config_Info config_info_;

#if defined (ACE_HAS_THREADS)
  typedef TAO_SYNCH_RECURSIVE_MUTEX LOCK;
#else
  typedef ACE_Null_Mutex LOCK;
#endif /* ACE_HAS_THREADS */

  /// This protects access to the scheduler during configuration runs.
  LOCK lock_;


  ///////////////////////////////////////
  // member functions for internal use //
  ///////////////////////////////////////

  /// Prepare for another schedule computation, but do not
  /// disturb the "output" (priorities that have already been assigned).
  void reset ();

  /// Display the schedule, task-by-task.
  void print_schedule ();

  Scheduler_Generic (const Scheduler_Generic &);
  Scheduler_Generic &operator= (const Scheduler_Generic &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* SCHEDULER_INTERNAL_H */

// EOF
