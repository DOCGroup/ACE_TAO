/* -*- C++ -*- */
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

#ifndef DYNSCHED_H
#define DYNSCHED_H
#include /**/ "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Map_Manager.h"
#include "ace/Message_Block.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "SchedEntry.h"
#include "sched_export.h"

class TAO_RTSched_Export ACE_DynScheduler
  // = TITLE
  //    Dispatch scheduling interface.
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
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Config_Info Config_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;
  typedef RtecScheduler::Dispatching_Type_t Dispatching_Type;
  typedef RtecScheduler::Scheduling_Anomaly Scheduling_Anomaly;
  typedef RtecScheduler::Anomaly_Severity Anomaly_Severity;

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
    , ST_UNKNOWN_PRIORITY
    , ST_TASK_ALREADY_REGISTERED
    , ST_NO_TASKS_REGISTERED
    , ST_BAD_DEPENDENCIES_ON_TASK
    , ST_BAD_INTERNAL_POINTER
    , ST_VIRTUAL_MEMORY_EXHAUSTED
    , TWO_WAY_DISJUNCTION
    , TWO_WAY_CONJUNCTION
    , UNRECOGNIZED_INFO_TYPE

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
    , ST_UNRESOLVED_REMOTE_DEPENDENCIES
    , ST_UNRESOLVED_LOCAL_DEPENDENCIES
    , ST_INVALID_PRIORITY_ORDERING
    , UNABLE_TO_OPEN_SCHEDULE_FILE
    , UNABLE_TO_WRITE_SCHEDULE_FILE
  };



  /////////////////////////////
  // public member functions //
  /////////////////////////////

  virtual ~ACE_DynScheduler ();
    // public dtor

  // = Utility function for outputting the textual
  //   representation of a status_t value.
  static const char * status_message (status_t status);

  // = Utility function for creating an entry for determining
  //   the severity of an anomaly detected during scheduling.
  static Anomaly_Severity anomaly_severity (status_t status);

  // = Utility function for creating an entry for the
  //   log of anomalies detected during scheduling.
  static Scheduling_Anomaly * create_anomaly (status_t status);


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

  status_t lookup_config_info (Preemption_Priority priority,
                               Config_Info* &config_info);
  // Obtains a Config_Info based on its priority.

  status_t
    schedule (ACE_Unbounded_Set<Scheduling_Anomaly *> &anomaly_set);
  // This sets up the data structures, invokes the internal scheduling method.

  status_t output_timeline (const char *filename, const char *heading);
  // this prints the entire set of timeline outputs to the specified file


  // = Access a thread priority.
  virtual int priority (const handle_t handle,
                        OS_Priority &priority,
                        Sub_Priority &subpriority,
                        Preemption_Priority &preemption_prio);
  // "priority" is the OS thread priority that was assigned to the Task that
  // was assigned "handle".  "subpriority" combines the dynamic and static
  // subpriorities of the Task that was assigned handle.  "preemption_prio"
  // is a platform-independent priority queue number, ranging from a
  // highest priority value of 0 to the lowest priority value, which is
  // returned by "minimum_priority_queue ()".  Returns 0 on success,
  // or -1 if an invalid handle was supplied.

  // = Access the platform-independent priority value of the lowest-priority
  //   thread.
   Preemption_Priority minimum_priority_queue () const;

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

  static void export_to_file (RT_Info*, FILE* file);
  static void export_to_file (RT_Info&, FILE* file);

  // accessors for the minimal and maximal dispatch entry id in the schedule
  u_long min_dispatch_id () const;
  u_long max_dispatch_id () const;

  virtual int dispatch_configuration (const Preemption_Priority &p_priority,
                                      OS_Priority& priority,
                                      Dispatching_Type & d_type);
  // provide the thread priority and queue type for the given priority level

protected:

  ////////////////////////////////
  // protected member functions //
  ////////////////////////////////

  ACE_DynScheduler ();

  status_t schedule_threads (
    ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set);
  // thread scheduling method: sets up array of pointers to task
  // entries that are threads, calls internal thread scheduling method

  status_t schedule_dispatches (
    ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set);
  // dispatch scheduling method: sets up an array of dispatch entries,
  // calls internal dispatch scheduling method.

  virtual status_t store_assigned_info (void);
  // = store assigned information back into the RT_Infos

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

  virtual status_t assign_priorities (
    Dispatch_Entry **dispatches,
    u_int count,
    ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set) = 0;
  // = assign priorities to the sorted dispatches

  virtual status_t assign_subpriorities (
    Dispatch_Entry **dispatches,
    u_int count,
    ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set) = 0;
  // = assign dynamic and static sub-priorities to the sorted dispatches

  virtual status_t
    schedule_timeline_entry (Dispatch_Entry &dispatch_entry,
                             ACE_Unbounded_Queue <Dispatch_Entry *>
                               &reschedule_queue) = 0;
  // = schedule a dispatch entry into the timeline being created

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

  ACE_Unbounded_Set <Config_Info *> *config_info_entries_;
  // Collection of dispatch configuration entries.

  ACE_Unbounded_Set <Dispatch_Entry *> *expanded_dispatches_;
    // expanded set of dispatch entries (all dispatch entries produced by
    // expanding sub-frames to the total frame size during timeline creation)

  Dispatch_Entry **ordered_dispatch_entries_;
  // An array of pointers to  dispatch entries. It is
  // sorted by the schedule_dispatches method.

  u_int dispatch_entry_count_;
  // the number of dispatch entries in the schedule

  u_int threads_;
  // the number of dispatch entries in the schedule

  ACE_Ordered_MultiSet <TimeLine_Entry_Link> *timeline_;
  // Ordered MultiSet of timeline entries.

private:

  ///////////////////////////////
  // private type declarations //
  ///////////////////////////////

  typedef ACE_CString EXT;
  typedef RT_Info *INT;

#if defined (ACE_HAS_THREADS)
  typedef TAO_SYNCH_MUTEX SYNCH;
  typedef TAO_SYNCH_RECURSIVE_MUTEX LOCK;
#else
  typedef ACE_Null_Mutex SYNCH;
  typedef ACE_Null_Mutex LOCK;
#endif /* ACE_HAS_THREADS */

  typedef ACE_Map_Manager<EXT, INT, TAO_SYNCH_MUTEX> Info_Collection;
  typedef ACE_Map_Iterator<EXT, INT, TAO_SYNCH_MUTEX> Info_Collection_Iterator;
  typedef ACE_Map_Entry<EXT, INT> Info_Collection_Entry;

  //////////////////////////////
  // private member functions //
  //////////////////////////////

  status_t create_timeline ();
  // Create a timeline.

  status_t output_dispatch_timeline (const char *filename);
  status_t output_dispatch_timeline (FILE *file);
  // this prints a dispatch timeline to the specified file

  status_t output_preemption_timeline (const char *filename);
  status_t output_preemption_timeline (FILE *file);
  // this prints a preemption timeline to the specified file

  status_t output_viewer_timeline (const char *filename);
  status_t output_viewer_timeline (FILE *file);
  // this prints a scheduling viewer timeline to the specified file

  status_t output_dispatch_priorities (const char *filename);
  status_t output_dispatch_priorities (FILE *file);
  // this prints the scheduling parameters and assigned priorities to the specified file

  // = Set up the task entry data structures
  status_t setup_task_entries (void);

  // = Relate the task entries according to the
  //   dependencies of the underlying RT_Infos
  status_t relate_task_entries (void);

  // recursively traverse dependency graph, relating
  // task entries and performing DFS start/end marking
  status_t relate_task_entries_recurse (long &time, Task_Entry &entry);

  // identify thread delimiters
  status_t
    identify_threads (ACE_CString & unresolved_locals,
                      ACE_CString & unresolved_remotes);

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

  status_t
    propagate_dispatches (
      ACE_Unbounded_Set<RtecScheduler::Scheduling_Anomaly *> &anomaly_set,
      ACE_CString & unresolved_locals,
      ACE_CString & unresolved_remotes);
  // propagate the dispatch information from the
  // threads throughout the call graph

  status_t calculate_utilization_params ();
  // calculate utilization, frame size, etc.

  // the following functions are not implememented
  ACE_UNIMPLEMENTED_FUNC(ACE_DynScheduler (const ACE_DynScheduler &))
  ACE_UNIMPLEMENTED_FUNC(ACE_DynScheduler &operator= (const ACE_DynScheduler &))

  //////////////////////////
  // private data members //
  //////////////////////////

  LOCK lock_;
  // This protects access to the scheduler during configuration runs.

  ACE_Unbounded_Set <RT_Info *> rt_info_entries_;
  // Collection of known tasks.

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

  u_int up_to_date_;
    // indicates whether the a valid schedule has been generated since the last
    // relevant change (addition, alteration or removal of an RT_Info, etc.)

  u_long min_dispatch_id_;

  u_long max_dispatch_id_;

};

#if defined (__ACE_INLINE__)
#include "DynSched.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* DYNSCHED_H */

// EOF
