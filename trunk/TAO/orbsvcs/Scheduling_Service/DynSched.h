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
//    DynSched.h
//
// = CREATION DATE
//    23 January 1997
//
// = AUTHOR
//    Chris Gill
//
// ============================================================================

#if ! defined (SCHEDULER_H)
#define SCHEDULER_H

#include "ace/ACE.h"
#include "ace/Map_Manager.h"
#include "ace/Message_Block.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "SchedEntry.h"

class ACE_Scheduler
  // = TITLE
  //    Thread scheduler interface.
  //
  // = DESCRIPTION
  //    This abstract base class provides the majority of the 
  //    implementation of either an off-line scheduler, or the
  //    necessary on-line component of the Scheduler.
{
public:

  //////////////////////////////
  // public type declarations //
  //////////////////////////////

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Sub_Priority Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period Period;
  typedef RtecScheduler::Info_Type Info_Type;
  typedef RtecScheduler::Dependency_Type Dependency_Type;

  typedef ACE_Map_Entry <ACE_CString, RT_Info *> Thread_Map_Entry;
  typedef ACE_Map_Manager <ACE_CString, RT_Info *, ACE_Null_Mutex>
      Thread_Map;
  typedef ACE_Map_Iterator <ACE_CString, RT_Info *, ACE_Null_Mutex>
      Thread_Map_Iterator;

  typedef const char *Object_Name;
  // Objects are named by unique strings.

  enum status_t {
    // The following are used both by the runtime Scheduler and during
    // scheduling.
      NOT_SCHEDULED = -1    // the schedule () method has not been called yet
    , FAILED = -1
    , SUCCEEDED
    , ST_UNKNOWN_TASK
    , ST_TASK_ALREADY_REGISTERED
    , ST_BAD_DEPENDENCIES_ON_TASK
    , ST_BAD_INTERNAL_POINTER
    , ST_VIRTUAL_MEMORY_EXHAUSTED

    // The following are only used by the runtime Scheduler.
    , TASK_COUNT_MISMATCH     // only used by schedule ()
    , THREAD_COUNT_MISMATCH   // only used by schedule ()
    , INVALID_PRIORITY        // only used by schedule (): mismatch of
                              // (off-line, maybe) Scheduler output to
                              // the runtime Scheduler component.

    // The following are only used during scheduling (in the case of
    // off-line scheduling, they are only used prior to runtime).
    , ST_UTILIZATION_BOUND_EXCEEDED
    , ST_INSUFFICIENT_THREAD_PRIORITY_LEVELS
    , ST_CYCLE_IN_DEPENDENCIES
    , ST_INVALID_PRIORITY_ORDERING
    , UNABLE_TO_OPEN_SCHEDULE_FILE
    , UNABLE_TO_WRITE_SCHEDULE_FILE
  };



  /////////////////////////////
  // public member functions //
  /////////////////////////////

  virtual ~ACE_Scheduler ();
    // public dtor

  // = Utility function for outputting the textual
  //   representation of a status_t value.
  static const char * status_message (status_t status);

  // = Initialize the scheduler.
  void init (const OS_Priority minimum_priority,
             const OS_Priority maximum_priority,
             const char *runtime_filename = 0,
             const char *rt_info_filename = 0,
             const char *timeline_filename = 0);
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

  void reset ();
  // Prepare for another schedule computation: once a reasonable schedule
  // has been generated, a new schedule will not be computed unless an
  // RT_Info is added, or this method is invoked to clear the previous 
  // schedule (allows fault correcting alteration of RT_Infos outside the 
  // scheduler implementation, followed by generation of a new schedule).

  // = Registers a task.
  status_t register_task (RT_Info *, handle_t &handle);
  // If the Task registration succeeds, this function returns SUCCEEDED
  // and sets "handle" to a unique identifier for the task.
  // Otherwise, it returns either VIRTUAL_MEMORY_EXHAUSTED or
  // TASK_ALREADY_REGISTERED sets the handle to 0.  (A task may
  // only be registered once.)

  status_t get_rt_info (Object_Name name, RT_Info* &rtinfo);
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

  status_t lookup_rt_info (handle_t handle, RT_Info* &rtinfo);
  // Obtains an RT_Info based on its "handle".

  status_t schedule (void);
  // This sets up the data structures, invokes the internal scheduling method.

  status_t output_timeline (const char *filename);
  // this prints the entire set of timeline outputs to the specified file

  // = Access a thread priority.
//  TBD - put this back in, but with dynamic subpriority as well as static
//  int priority (const handle_t handle,
//                OS_Priority &OS_priority,
//                 Preemption_Priority &preemption_priority,
//                 Sub_Priority &dynamic_subpriority,
//                 Sub_Priority &static_subpriority);
  // Defines "priority" as the priority that was assigned to the Task that
  // was assigned "handle".  Defines "dynamic subpriority" as the strategy
  // specific assignment of dynamic subpriority within a priority level, and
  // "static subpriority" as the minimal importance and topological ordering.
  // Returns 0 on success, or -1 if an invalid handle was supplied.
  // Queue numbers are platform-independent priority values, ranging from
  // a highest priority value of 0 to the lowest priority value, which is
  // returned by "minimum_priority_queue ()".

  // = Access the platform-independent priority value of the lowest-priority
  //   thread.

   Preemption_Priority minimum_priority_queue () const;
  // This is intended for use by the Event Channel, so it can determine the
  // number of priority dispatch queues to create.

  // = Access the number of tasks.
  u_int tasks () const;

  // = Access the number of threads.
  u_int threads () const;

  // = Access the current scheduler status.
  status_t status () const;

  // = Access the current output (debugging) level.
  u_int output_level () const;
  // Default is 0; set to 1 to print out schedule, by task.  Set
  // to higher than one for debugging info.

  // = Set the scheduler output (debugging) level.
  void output_level (const u_int level);
  // the only supported levels are 0 (quiet), 1 (verbose) and 2 (debug)

  int add_dependency(RT_Info* rt_info,
       			     Dependency_Info& d);

  static int number_of_dependencies(RT_Info* rt_info);
  static int number_of_dependencies(RT_Info& rt_info);

  // TBD - modify these to take a Task_Entry and show all its dispatches
  static void export(RT_Info*, FILE* file);
  static void export(RT_Info&, FILE* file);

protected:

  ////////////////////////////////
  // protected member functions //
  ////////////////////////////////

  ACE_Scheduler ();


  status_t schedule_threads (void);
  // thread scheduling method: sets up array of pointers to task 
  // entries that are threads, calls internal thread scheduling method

  status_t schedule_dispatches (void);
  // dispatch scheduling method: sets up an array of dispatch entries,
  // calls internal dispatch scheduling method.

  // = Set the minimum priority value.
  void minimum_priority_queue (const Preemption_Priority minimum_priority_queue_number);

  // = Set the number of tasks.
  void tasks (const u_int tasks);

  // = Set the number of threads.
  void threads (const u_int threads);

  // = Set the current scheduler status.
  void status (const status_t new_status);

  /////////////////////////////////////////////
  // protected pure virtual member functions //
  /////////////////////////////////////////////

  virtual Preemption_Priority minimum_critical_priority () = 0;
  // = determine the minimum critical priority number

  virtual status_t sort_dispatches (Dispatch_Entry **, u_int) = 0;
  // internal sorting method: this orders the dispatches by
  // static priority and dynamic and static subpriority.

  virtual status_t assign_priorities (Dispatch_Entry **dispatches,
                                      u_int count) = 0;
    // = assign priorities to the sorted dispatches

  virtual status_t assign_subpriorities (Dispatch_Entry **dispatches, 
                                         u_int count) = 0;
    // = assign dynamic and static sub-priorities to the sorted dispatches


  ////////////////////////////
  // protected data members //
  ////////////////////////////

  OS_Priority minimum_priority_;
  // The minimum OS thread priority value that the application specified (in
  // its call to init ()).

  OS_Priority maximum_priority_;
  // The maximum OS thread priority value that the application specified (in
  // its call to init ()).

  Task_Entry *task_entries_;
  // Collection of known tasks.

  Task_Entry **ordered_task_entries_;
  // An array of pointers to task entries which wrap RT_Infos. It is 
  // sorted by the DFS finishing time and then the resulting topological
  // over the call graph is used both to check for call chain cycles and
  // to correctly propagate scheduling information away from the threads.

  ACE_Unbounded_Set <Dispatch_Entry *> *thread_delineators_;
    // identifies dispatch entries whose underlying
    // Task Entries delineate threads

  Dispatch_Entry **ordered_thread_dispatch_entries_;
  // An array of pointers to task entries which initiate call chains.
  // It is sorted by the schedule_threads method defined in the derived class.

  ACE_Unbounded_Set <Dispatch_Entry *> *dispatch_entries_;
    // the set of dispatch entries

  Dispatch_Entry **ordered_dispatch_entries_;
  // An array of pointers to  dispatch entries. It is  
  // sorted by the schedule_dispatches method.

  u_int dispatch_entry_count_;
  // the number of dispatch entries in the schedule

  u_int threads_;
  // the number of dispatch entries in the schedule

private:

  ///////////////////////////////
  // private type declarations //
  ///////////////////////////////

  typedef ACE_CString EXT;
  typedef RT_Info *INT;

#if defined (ACE_HAS_THREADS)
  typedef ACE_Thread_Mutex SYNCH;
  typedef ACE_Recursive_Thread_Mutex LOCK;
#else
  typedef ACE_Null_Mutex SYNCH;
  typedef ACE_Null_Mutex LOCK;
#endif /* ACE_HAS_THREADS */

  typedef ACE_Map_Manager<EXT, INT, ACE_SYNCH_MUTEX> Info_Collection;
  typedef ACE_Map_Iterator<EXT, INT, ACE_SYNCH_MUTEX> Info_Collection_Iterator;
  typedef ACE_Map_Entry<EXT, INT> Info_Collection_Entry;

  //////////////////////////////
  // private member functions //
  //////////////////////////////

  status_t create_timeline ();
  // Create a timeline.

  status_t output_dispatch_timeline (const char *filename);
  // this prints the entire set of timeline outputs to the specified file

  status_t output_preemption_timeline (const char *filename);
  // this prints the entire set of timeline outputs to the specified file

  // = Set up the task entry data structures
  status_t setup_task_entries (void);

  // = Relate the task entries according to the 
  //   dependencies of the underlying RT_Infos
  status_t relate_task_entries (void);

  // recursively traverse dependency graph, relating
  // task entries and performing DFS start/end marking
  status_t relate_task_entries_recurse (long &time, Task_Entry &entry);

  // identify thread delimiters
  status_t identify_threads (void);

  // checks for cycles in the dependency graph
  status_t check_dependency_cycles (void);

  // recursion used to check for cycles in the dependency graph
  status_t check_dependency_cycles_recurse (Task_Entry &entry);

  // = Aggregate the scheduling parameters of the threads
  status_t aggregate_thread_parameters (void);

  // = recursion over oneway dependencies used to aggregate thread parameters
  status_t aggregate_oneways_recurse (Task_Entry &entry);

  // = recursion over twoway dependencies used to aggregate thread parameters
  status_t aggregate_twoways_recurse (Task_Entry &entry);

  // update the scheduling parameters for the previous priority level
  void update_priority_level_params ();
  
  status_t propagate_dispatches ();
  // propagate the dispatch information from the
  // threads throughout the call graph

  status_t calculate_utilization_params (void);
  // calculate utilization, frame size, etc.

  // the following functions are not implememented
  ACE_UNIMPLEMENTED_FUNC(ACE_Scheduler (const ACE_Scheduler &))
  ACE_UNIMPLEMENTED_FUNC(ACE_Scheduler &operator= (const ACE_Scheduler &))

  //////////////////////////
  // private data members //
  //////////////////////////

  ACE_Unbounded_Set <RT_Info *> rt_info_entries_;
  // Collection of known tasks.

  LOCK lock_;
  // This protects access to the scheduler during configuration runs.

  u_int handles_;
  // The number of task handles dispensed so far.

  const char *runtime_filename_;
  // Destination file of Scheduler output from the configuration run.

  const char *rt_info_filename_;
  // Destination file of all rt_info data from the configuration run.

  const char *timeline_filename_;
  // The destination of the timeline.

  Info_Collection info_collection_;
  // A binding of name to rt_info.  This is the mapping for every
  // rt_info in the process.

  u_int tasks_;

  status_t status_;

  u_int output_level_;

  u_long frame_size_; /* 100 nanosec */
    // minimum frame size for all tasks

  u_long critical_set_frame_size_; /* 100 nanosec */
    // minimum frame size for guaranteed schedulable tasks

  double utilization_;
    // total utilization for all tasks

  double critical_set_utilization_;
    // minimum frame size for guaranteed schedulable tasks

  Preemption_Priority minimum_priority_queue_;
    // The platform-independent priority value of the Event Channel's
    // minimum priority dispatch queue.  The value of the maximum priority
    // dispatch queue is always 0.

  Preemption_Priority minimum_guaranteed_priority_queue_;
    // The platform-independent priority value of the minimum priority dispatch
    // queue whose operations are guaranteed to be schedulable.  The value of 
    // the maximum priority dispatch queue is always 0, -1 indicates none can
    // be guaranteed.

  ACE_Ordered_MultiSet <TimeLine_Entry_Link> *timeline_;
    // Ordered MultiSet of timeline entries.

  u_int up_to_date_;
    // indicates whether the a valid schedule has been generated since the last
    // relevant change (addition, alteration or removal of an RT_Info, etc.)
};

#if defined (__ACE_INLINE__)
#include "DynSched.i"
#endif /* __ACE_INLINE__ */

#endif /* SCHEDULER_H */

// EOF
