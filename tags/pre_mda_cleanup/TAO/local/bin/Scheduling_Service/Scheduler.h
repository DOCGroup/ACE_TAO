/* -*- C++ -*- */
//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    Scheduler.h
//
// = CREATION DATE
//    23 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#if ! defined (SCHEDULER_H)
#define SCHEDULER_H

#include "ace/ACE.h"
#include "ace/Map_Manager.h"
#include "ace/Message_Block.h"
#include "ace/Synch.h"
#include "ace/SString.h"

#include "RtecSchedulerC.h"
#include "Event_Service_Constants.h"

class ACE_Scheduler
  // = TITLE
  //    Thread scheduler interface.
  //
  // = DESCRIPTION
  //    This virtual base class is the interface to either an off-line
  //    scheduler, or to the necessary on-line component of the Scheduler.
{
public:
  typedef u_int mode_t;

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Thread_Priority;
  typedef RtecScheduler::Sub_Priority Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  // Map some types to simplify re-use.

  typedef const char *Object_Name;
  // Objects are named by unique strings.

  static const mode_t CURRENT_MODE;

  enum status_t {
    // The following are used both by the runtime Scheduler and during
    // scheduling.
    NOT_SCHEDULED = -1        // the schedule () method has not been called yet
    , FAILED = -1
    , SUCCEEDED
    , ST_UNKNOWN_TASK
    , ST_TASK_ALREADY_REGISTERED
    , ST_VIRTUAL_MEMORY_EXHAUSTED

    // The following are only used by the runtime Scheduler.
    , INVALID_MODE
    , MODE_COUNT_MISMATCH     // only used by schedule ()
    , TASK_COUNT_MISMATCH     // only used by schedule ()
    , INVALID_PRIORITY        // only used by schedule (): mismatch of
                              // (off-line, maybe) Scheduler output to
                              // the runtime Scheduler component.

    // The following are only used during scheduling (in the case of
    // off-line scheduling, they are only used prior to runtime).
    , ST_UTILIZATION_BOUND_EXCEEDED
    , ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS
    , ST_CYCLE_IN_DEPENDENCIES
    , UNABLE_TO_OPEN_SCHEDULE_FILE
    , UNABLE_TO_WRITE_SCHEDULE_FILE
  };

  virtual ~ACE_Scheduler ();

  // = Utility function for outputting the textual representation of a
  //   status_t value to a FILE.
  static void output (FILE *, const status_t);

  // = Initialize the scheduler.
  virtual void init (const int minimum_priority,
                     const int maximum_priority,
                     const char *runtime_filename = 0,
                     const char *rt_info_filename = 0,
                     const char *timeline_filename = 0) = 0;
  // The minimum and maximum priority are the OS-specific priorities that
  // are used when creating the schedule (assigning priorities).  The
  // minimum_priority is the priority value of the lowest priority.
  // It may be numerically higher than the maximum_priority, on OS's such
  // as VxWorks that use lower values to indicate higher priorities.
  //
  // When Scheduler::schedule is called, the schedule is output to the
  // file named by "runtime_filename" if it is non-zero.
  // This file is compilable; it is linked into the runtime executable
  // to provide priorities to the runtime scheduling component.
  // If the "rt_info_filename" is non-zero, the RT_Info for
  // every task is exported to it.  It is not used at runtime.
  // If the "timeline_filename" is non-zero, the timeline output
  // file is created.  It is not used at runtime.
  //
  // The runtime scheduling component ignores these filenames.  It just
  // uses the priorities that were linked in to the executable, after
  // converting them to platform-specific values.

  // = Registers a task.
  virtual status_t register_task (RT_Info *[],
                                  const u_int number_of_modes,
                                  handle_t &handle) = 0;
  // If the Task registration succeeds, this function returns SUCCEEDED
  // and sets "handle" to a unique identifier for the task.
  // Otherwise, it returns either VIRTUAL_MEMORY_EXHAUSTED or
  // TASK_ALREADY_REGISTERED sets the handle to 0.  (A task may
  // only be registered once.)
  // The RT_Info * array is indexed by mode; there must be one element for
  // each mode, as specified by number_of_modes.  If a task does not
  // run in a mode, then its entry in the array for that mode must
  // be 0.

  virtual status_t get_rt_info (Object_Name name,
                                RT_Info* &rtinfo);
  // Tries to find the RT_Info corresponding to <name> in the RT_Info
  // database.  Returns SUCCEEDED if <name> was found and <rtinfo> was
  // set.  Returns UNKNOWN_TASK if <name> was not found, but <rtinfo>
  // was set to a newly allocated RT_Info.  In this UNKNOWN_TASK case,
  // the task must call RT_Info::set to fill in execution properties.
  // In the SUCCEEDED and UNKNOWN_TASK cases, this->register_task
  // (rtinfo, 0, handle) is called.  Returns FAILED if an error
  // occurs.
  // 
  // One motivation for allocating RT_Info's from within the Scheduler
  // is to allow RT_Infos to persist after the tasks that use them.
  // For instance, we may want to call this->schedule right before the
  // application exits a configuration run.  If the tasks have been
  // deleted (deleting their RT_Infos with them), this->schedule will
  // fail.

  virtual status_t lookup_rt_info (handle_t handle,
				   RT_Info* &rtinfo) = 0;
  // Obtains an RT_Info based on its "handle".

  // = Computes the schedule.
  virtual status_t schedule (void) = 0;
  // This actually generates the files.

  // = Access a thread priority.
  virtual int priority (const handle_t handle,
                        OS_Thread_Priority &priority,
                        Sub_Priority &subpriority,
                        Preemption_Priority &preemption_prio,
                        const mode_t = CURRENT_MODE) const = 0;
  // Defines "priority" as the priority that was assigned to the Task that
  // was assigned "handle", for the specified mode.  Defines "subpriority"
  // as the relative ordering (due to dependencies) within the priority.
  // Returns 0 on success, or -1 if an invalid mode or handle are supplied.
  // Queue numbers are platform-independent priority values, ranging from
  // a highest priority value of 0 to the lowest priority value, which is
  // returned by "minimum_priority_queue ()".

  // = Access the platform-independent priority value of the lowest-priority
  //   thread.
  u_int minimum_priority_queue () const { return minimum_priority_queue_; }
  // This is intended for use by the Event Channel, so it can determine the
  // number of priority dispatch queues to create.

  // = Access the number of modes.
  u_int modes () const { return modes_; }

  // = Access the number of tasks.
  u_int tasks () const { return tasks_; }

  // = Access the number of threads.
  u_int threads () const { return threads_; }

  // = Access the current mode.
  mode_t mode () const { return mode_; }

  // = Set the current mode.
  void mode (const mode_t mode) { mode_ = mode; }

  // = Access the current scheduler status.
  status_t status () const { return status_; }

  // = Access the current output (debugging) level.
  u_int output_level () const { return output_level_; }
  // Default is 0; set to 1 to print out schedule, by task.  Set
  // to higher than one for debugging info.

  // = Set the scheduler output (debugging) level.
  void output_level (const u_int level) { output_level_ = level; }
  // the only supported levels are 0 (quiet), 1 (verbose) and 2
  // (debug)

  static int add_dependency(RT_Info* rt_info,
			    const Dependency_Info& d);

  static int number_of_dependencies(RT_Info* rt_info);
  static int number_of_dependencies(RT_Info& rt_info);

  static void export(RT_Info*, FILE* file);
  static void export(RT_Info&, FILE* file);

protected:
  ACE_Scheduler ();

  // = Set the minimum priority value.
  void minimum_priority_queue (const u_int minimum_priority_queue_number)
    { minimum_priority_queue_ = minimum_priority_queue_number; }

  // = Set the number of modes.
  void modes (const u_int modes) { modes_ = modes; }

  // = Set the number of tasks.
  void tasks (const u_int tasks) { tasks_ = tasks; }

  // = Set the number of threads.
  void threads (const u_int threads) { threads_ = threads; }

  // = Set the current scheduler status.
  void status (const status_t new_status) { status_ = new_status; }

private:
  typedef ACE_CString EXT;
  typedef RT_Info **INT;

#if defined (ACE_HAS_THREADS)
  typedef ACE_Thread_Mutex SYNCH;
#else
  typedef ACE_Null_Mutex SYNCH;
#endif /* ACE_HAS_THREADS */

  typedef ACE_Map_Manager<EXT, INT, SYNCH> Info_Collection;
  typedef ACE_Map_Iterator<EXT, INT, SYNCH> Info_Collection_Iterator;
  typedef ACE_Map_Entry<EXT, INT> Info_Collection_Entry;

  Info_Collection info_collection_;
  // A binding of name to rt_info.  This is the mapping for every
  // rt_info in the process.

  static ACE_Scheduler *instance_;

  u_int minimum_priority_queue_;
  // The platform-independent priority value of the Event Channel's
  // minimum priority dispatch queue.  The value of the maximum priority
  // dispatch queue is always 0.

  u_int modes_;
  u_int tasks_;
  u_int threads_;

  mode_t mode_;
  status_t status_;
  u_int output_level_;

  // the following functions are not implememented
  ACE_UNIMPLEMENTED_FUNC(ACE_Scheduler (const ACE_Scheduler &))
  ACE_UNIMPLEMENTED_FUNC(ACE_Scheduler &operator= (const ACE_Scheduler &))
};

typedef ACE_Scheduler Scheduler;

#if defined (__ACE_INLINE__)
#include "Scheduler.i"
#endif /* __ACE_INLINE__ */

#endif /* SCHEDULER_H */


// EOF
