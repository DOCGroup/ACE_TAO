/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    Scheduler_Internal.h
//
// = CREATION DATE
//    23 January 1997
//
// = AUTHOR
//    David Levine
//
// ============================================================================

#if ! defined (SCHEDULER_INTERNAL_H)
#define SCHEDULER_INTERNAL_H

#include "ace/Containers.h"
#include "ace/ACE.h"

#include "Scheduler.h"

struct Mode_Entry;  // for internal use only

class Scheduler_Internal : public ACE_Scheduler
  // = TITLE
  //    Implementation of an off-line scheduler.
  //
  // = DESCRIPTION
  //    Schedules tasks using rate-monotonic scheduling.
{
public:
  Scheduler_Internal ();
  virtual ~Scheduler_Internal ();

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

  // = Computes the schedule.
  virtual status_t schedule (void);

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

  typedef ACE_Map_Entry <ACE_CString, RT_Info *> Thread_Map_Entry;
  typedef ACE_Map_Manager <ACE_CString, RT_Info *, ACE_Null_Mutex>
      Thread_Map;
  typedef ACE_Map_Iterator <ACE_CString, RT_Info *, ACE_Null_Mutex>
      Thread_Map_Iterator;

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

  RT_Info ***ordered_info_;
  // An ordered array of pointers to the RT_Info pointers stored
  // in task_entries_.  It is index by [handle][mode].  This permits
  // efficient retrieval by get_priority ().

  ACE_Bounded_Set <const RT_Info *> *visited_;
  // When updating dependencies, use this set to keep track of
  // visited RT_Info instances in order to detect cycles in the
  // dependency graph.  It is dynamically allocated/deallocated so that
  // it doesn't have to be "cleaned out" manually for each mode.

  ACE_Bounded_Set <RT_Info *> *leaves_;
  // For 1) storing aggregate thread execution parameters during
  //        traversal of dependency chains, and
  //     2) assigning priorities to all tasks.

  ACE_Bounded_Set <const RT_Info *> *dependencies_;
  // Collection of RT_Info's that are dependent tasks, for efficient
  // identification of dependency-chain roots.

  ACE_Bounded_Set <RT_Info *> *roots_;
  // Collection of RT_Info's that are roots of dependency chains.

  status_t update_dependencies (RT_Info &, Sub_Priority subpriority);
  // For each of this task's dependencies (transitively), increment its
  // subpriority.  The higher subpriorities indicate that the dependent
  // tasks should run before this task.

  u_long *frame_size_; /* millisec */
  // For creating timelines (array indexed by mode).

  Thread_Map *thread_info_;
  // Collection of known threads, dynamically allocated because
  // it is reused for each mode.  It consists of a amp of RT_Info
  // instances, keyed by entry_point, that aggregate the info for the
  // entire thread.  Each instance has one dependency:  the root
  // RT_Info instance of the thread.
  // *thread_info_ is an array, indexed by mode.

  Thread_Map *leaf_info_;
  // Mapping from roots of dependency chains to their leaves.

public:  // for template specialization
  struct Timeline_Entry
  {
    u_long handle_;
    const char *entry_point_;
    u_long start_;  // microseconds
    u_long stop_;   // microseconds
    u_long next_start_;

    Timeline_Entry (const u_long handle = 0,
                    const char *entry_point = 0,
                    const u_long start = 0,
                    const u_long stop = 0,
                    const u_long next_start = 0)
      : handle_ (handle),
        entry_point_ (entry_point),
        start_ (start),
        stop_ (stop),
        next_start_ (next_start) {}
  };
private:
#if defined (ACE_HAS_THREADS)
  typedef ACE_Recursive_Thread_Mutex LOCK;
#else
  typedef ACE_Null_Mutex LOCK;
#endif /* ACE_HAS_THREADS */

  LOCK lock_;
  // This protects access to the scheduler during configuration runs.

  ACE_Unbounded_Queue <Timeline_Entry> *timeline_;
  // For storing timelines.


  ///////////////////////////////////////
  // member functions for internal use //
  ///////////////////////////////////////

  void reset ();
  // Prepare for another schedule computation, but do not
  // disturb the "output" (priorities that have already been assigned).

  status_t schedule_rms (const int minimum_priority,
                         const int maximum_priority);
  // Computes the schedule using Rate Monotonic Scheduling.

  RT_Info &calculate_thread_properties (RT_Info &thread_info,
					RT_Info &rt_info,
					const u_int number_of_calls);
  // Computes the execution time, etc., of the thread, identified by
  // rt_info, and store it in thread_info, by recursively traversing
  // the RT_Info call chain.  Returns the root node.

  void print_schedule ();
  // Display the schedule, task-by-task.

  status_t store_schedule (const char *filename);
  // Store the schedule in the named file.

  status_t store_rt_info (const char *filename);
  // Export all RT_Info to the named file.

  status_t identify_threads ();
  // Assembles RT_Tasks into threads.

  status_t aggregate_thread_parameters ();
  // Stores the aggregated thread parameters at each leaf (thread delineator)
  // in the tread_info_ array.

  u_int find_number_of_threads (mode_t mode);
  // Counts the number of threads in a mode based on the task dependencies.

  status_t find_thread_delineators (mode_t mode);
  // Find and store the thread delineators:  they have no dependencies
  // and have a specified rate, or they have internal threads of their own.

  status_t find_dependency_chain_roots (mode_t mode);
  // Find and store the roots of dependency chains.

  status_t find_dependency_chain_leaves ();
  // Find and store the leaves of dependency chains.

  status_t generate_timeline (
    Mode_Entry const sorted_rt_info [],
    const u_int number_of_tasks,
    u_long frame_size,
    ACE_Unbounded_Queue <Timeline_Entry> &jobs);

  status_t create_timelines (const char *filename);
  // Create a timeline for each mode.

  // the following functions are not implememented
  Scheduler_Internal (const Scheduler_Internal &);
  Scheduler_Internal &operator= (const Scheduler_Internal &);
};


#if defined (__ACE_INLINE__)
#include "Scheduler_Internal.i"
#endif /* __ACE_INLINE__ */

#endif /* SCHEDULER_INTERNAL_H */


// EOF
