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
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Event_Service_Constants.h"

class ACE_Scheduler
  // = TITLE
  //    Thread scheduler interface.
  //
  // = DESCRIPTION
  //    This virtual base class is the interface to either an off-line
  //    scheduler, or to the necessary on-line component of the Scheduler.
{
public:

  //////////////////////////////
  // public type declarations //
  //////////////////////////////

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Thread_Priority;
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
    NOT_SCHEDULED = -1        // the schedule () method has not been called yet
    , FAILED = -1
    , SUCCEEDED
    , ST_UNKNOWN_TASK
    , ST_TASK_ALREADY_REGISTERED
    , ST_BAD_DEPENDENCIES_ON_TASK
    , ST_NULL_DEPENDENCY_POINTER
    , ST_VIRTUAL_MEMORY_EXHAUSTED

    // The following are only used by the runtime Scheduler.
    , TASK_COUNT_MISMATCH     // only used by schedule ()
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


    // forward declaration of task entry class
  class Task_Entry;

  class Dispatch_Entry
  {
  public:

    // ctor
	Dispatch_Entry (Preemption_Priority priority,
                    Time arrival,
                    Time deadline,
                    Time execution_time,
                    Task_Entry &task_entry);

    // copy ctor
    Dispatch_Entry (const Dispatch_Entry &d);

    u_long dispatch_id ();
    Preemption_Priority priority ();
    Time arrival ();
    Time deadline ();
    Time execution_time ();
    Task_Entry &task_entry ();    
    Dispatch_Entry *previous_instance ();
    void previous_instance (Dispatch_Entry *p);

  private:

    // stores the next dispatch entry id to be used
    static u_long next_id_;

    // the id of the current dispatch entry
    u_long dispatch_id_;

    // the scheduler priority of the current dispatch entry
    Preemption_Priority priority_;

    // the arrival time of the current dispatch entry
    Time arrival_;

    // the deadline of the current dispatch entry
    Time deadline_;

    // the execution time of the current dispatch entry
    // (used for timelines: this may only be a portion of
    // total total execution time of the task)
    Time execution_time_;

    // stores the id of the current dispatch entry
    Task_Entry &task_entry_;

    Dispatch_Entry *previous_instance_;
  };

  class Task_Entry_Link
  {
  public:

    // ctor
    Task_Entry_Link (Task_Entry &caller,
                     Task_Entry &called,
                     CORBA::Long number_of_calls,
                     Dependency_Type dependency_type);

    // accessor: number of calls
	CORBA::Long number_of_calls () const;

    // accessor: dependency type
	Dependency_Type dependency_type () const;

	// accessor: calling task entry
    Task_Entry &caller () const;
    
	// accessor: called task entry
	Task_Entry &called () const;

  private:

	// 
    CORBA::Long number_of_calls_;

    // the calling operation
    Task_Entry &caller_;

    // the called operation
    Task_Entry &called_;

	// the type of call dependency
	Dependency_Type dependency_type_;
  };

  typedef ACE_Unbounded_Set <Task_Entry_Link *> LINK_SET;

    // Wrapper for the RT_Info, which aggregates all its dispatches
  class Task_Entry
  {
  public:
    // info for DFS traversal, topological sort of call graph
    enum DFS_Status {NOT_VISITED, VISITED, FINISHED};

    // ctor
    Task_Entry ();

    // dtor
    ~Task_Entry ();

    // merge oneway calls according to info type, update
    // relevant scheduling characteristics for this entry
    void merge_oneway_calls ();

    // get pointer to underlying RT_Info
    RT_Info *rt_info ();

    // get effective period for the task entry
    Period effective_period ();

    // set/get time when node was discovered in DFS traversal
    void discovered (long l);
    long discovered ();

    // set/get time when node was finished in DFS traversal
    void finished (long l);
    long finished ();
   
    // set/get DFS traversal status of node
    void dfs_status (DFS_Status ds);
    DFS_Status dfs_status ();

    // set/get flag indicating whether node is a thread delineator
    void is_thread_delineator (int i);
    int is_thread_delineator ();

    // get set of links to Task Entries which this entry calls
    LINK_SET & calls ();

    // get set of links to Task Entries which call this entry
    LINK_SET & callers ();

    // get the type of Info the entry wraps
    Info_Type info_type ();

    // effective execution time for the task entry
    double effective_execution_time ();

  private:

    // perform disjunctive merge of arrival times of oneway calls:
    // all arrival times of all oneway callers are duplicated by the
    // multiplier and repetition over the new frame size and merged
    void disjunctive_merge ();

    // perform conjunctive merge of arrival times of oenway calls:
    // all arrival times of all oneway callers are duplicated by the
    // multiplier and repetition over the new frame size and then
    // iteratively merged by choosing the maximal arrival time at
    // the current position in each queue (iteration is in lockstep
    // over all queues, and ends when any queue ends).
    void conjunctive_merge ();

    // pointer to the underlying RT_Info
    RT_Info *rt_info_;

    // effective period for the task entry
    Period effective_period_;

    // queue of arrivals in the effective period
    ACE_Unbounded_Queue<Dispatch_Entry *> dispatch_queue_;
        
    // count of the arrivals in the effective period
    u_long arrival_count_;

    DFS_Status dfs_status_;
    long discovered_;
    long finished_;

    // info for identifying threads in the oneway call graph
    int is_thread_delineator_;

    // get set of links to Task Entries which this entry calls
    LINK_SET calls_;

    // get set of links to Task Entries which call this entry
    LINK_SET callers_;
  };

  /////////////////////////////
  // public member functions //
  /////////////////////////////

  virtual ~ACE_Scheduler ();
    // public dtor

  // = Utility function for outputting the textual representation of a
  //   status_t value to a FILE.
  static void output (FILE *, const status_t);

  // = Initialize the scheduler.
  virtual void init (const int minimum_priority,
                     const int maximum_priority,
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
  virtual status_t register_task (RT_Info *, handle_t &handle);
  // If the Task registration succeeds, this function returns SUCCEEDED
  // and sets "handle" to a unique identifier for the task.
  // Otherwise, it returns either VIRTUAL_MEMORY_EXHAUSTED or
  // TASK_ALREADY_REGISTERED sets the handle to 0.  (A task may
  // only be registered once.)

  virtual status_t get_rt_info (Object_Name name, RT_Info* &rtinfo);
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

  virtual status_t lookup_rt_info (handle_t handle, RT_Info* &rtinfo);
  // Obtains an RT_Info based on its "handle".

  virtual status_t schedule (void);
  // This sets up the data structures, invokes the internal scheduling method.

  // = Access a thread priority.
// CDG - TBD - put this back in, but with dynamic subpriority as well as static
//  virtual int priority (const handle_t handle,
//                        OS_Thread_Priority &priority,
//                        Sub_Priority &subpriority,
//                        Preemption_Priority &preemption_prio);
  // Defines "priority" as the priority that was assigned to the Task that
  // was assigned "handle".  Defines "subpriority"
  // as the relative ordering (due to oneway call graph) within the priority.
  // Returns 0 on success, or -1 if an invalid handle was supplied.
  // Queue numbers are platform-independent priority values, ranging from
  // a highest priority value of 0 to the lowest priority value, which is
  // returned by "minimum_priority_queue ()".

  // = Access the platform-independent priority value of the lowest-priority
  //   thread.

  u_int minimum_priority_queue () const;
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

// CDG - TBD - modify these to take a Task_Entry and show all its dispatches
  static void export(RT_Info*, FILE* file);
  static void export(RT_Info&, FILE* file);

  //////////////////////////////////////////
  // public pure virtual member functions //
  //////////////////////////////////////////

  virtual void print_schedule () = 0;
    // Display the schedule, task-by-task.

protected:

  ////////////////////////////////
  // protected member functions //
  ////////////////////////////////

  ACE_Scheduler ();


  virtual status_t schedule_threads (void);
  // thread scheduling method: sets up array of pointers to task 
  // entries that are threads, calls internal thread scheduling method

  virtual status_t schedule_dispatches (void);
  // dispatch scheduling method: sets up an array of dispatch entries,
  // calls internal dispatch scheduling method.

  // = Set the minimum priority value.
  void minimum_priority_queue (const u_int minimum_priority_queue_number);

  // = Set the number of tasks.
  void tasks (const u_int tasks);

  // = Set the number of threads.
  void threads (const u_int threads);

  // = Set the current scheduler status.
  void status (const status_t new_status);

  /////////////////////////////////////////////
  // protected pure virtual member functions //
  /////////////////////////////////////////////

  virtual status_t schedule_threads_i (void) = 0;
  // internal thread scheduling method: this orders the threads, assigns
  // static priority and dynamic and static subpriority to them.

  virtual status_t schedule_dispatches_i (void) = 0;
  // internal dispatch scheduling method: this actually orders the dispatches.

  ////////////////////////////
  // protected data members //
  ////////////////////////////

  int minimum_priority_;
  // The minimum priority value that the application specified (in
  // its call to init ()).

  int maximum_priority_;
  // The maximum priority value that the application specified (in
  // its call to init ()).

  Task_Entry *task_entries_;
  // Collection of known tasks.

  Task_Entry **ordered_task_entries_;
  // An array of pointers to task entries which wrap RT_Infos. It is 
  // sorted by the DFS finishing time and then the resulting topological
  // over the call graph is used both to check for call chain cycles and
  // to correctly propagate scheduling information away from the threads.

  ACE_Unbounded_Set <Task_Entry *> *thread_delineators_;
    // identifies task entries whose underlying RT_Infos delineate threads

  Task_Entry **ordered_thread_entries_;
  // An array of pointers to task entries which initiate call chains.
  // It is sorted by the schedule_threads method defined in the derived class.

  ACE_Unbounded_Set <Dispatch_Entry *> *dispatch_entries_;
    // identifies task entries whose underlying RT_Infos delineate threads

  Dispatch_Entry **ordered_dispatch_entries_;
  // An array of pointers to  dispatch entries. It is sorted by the 
  // schedule_dispatches method defined in the derived class.

private:

  ///////////////////////////////
  // private type declarations //
  ///////////////////////////////

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
      : handle_ (handle), entry_point_ (entry_point), start_ (start),
        stop_ (stop), next_start_ (next_start) {}
  };
 

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

  virtual status_t create_timeline (const char *filename);
  // Create a timeline.

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

  status_t store_schedule (const char *filename);
  // Export all dispatch entries to the named file.

  status_t store_rt_infos (const char *filename);
  // Export all RT_Infos to the named file.

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

  u_int threads_;

  status_t status_;

  u_int output_level_;

  u_long frame_size_; /* millisec */
    // minimum frame size for all tasks

  u_long critical_set_frame_size_; /* millisec */
    // minimum frame size for guaranteed schedulable tasks

  double utilization_;
    // total utilization for all tasks

  double critical_set_utilization_;
    // minimum frame size for guaranteed schedulable tasks

  ACE_Sched_Priority minimum_priority_queue_;
    // The platform-independent priority value of the Event Channel's
    // minimum priority dispatch queue.  The value of the maximum priority
    // dispatch queue is always 0.

  ACE_Sched_Priority minimum_guaranteed_priority_queue_;
    // The platform-independent priority value of the minimum priority dispatch
    // queue whose operations are guaranteed to be schedulable.  The value of 
    // the maximum priority dispatch queue is always 0.

  ACE_Unbounded_Queue <Timeline_Entry> *timeline_;
  // For storing timelines.

  u_int up_to_date_;
    // indicates whether the a valid schedule has been generated since the last
    // relevant change (addition, alteration or removal of an RT_Info, etc.)

};

#if defined (__ACE_INLINE__)
#include "DynSched.i"
#endif /* __ACE_INLINE__ */

#endif /* SCHEDULER_H */

// EOF
