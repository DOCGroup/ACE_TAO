/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    sched
//
// = FILENAME
//    SchedEntry.h
//
// = CREATION DATE
//    7 February 1998
//
// = AUTHOR
//    Chris Gill <cdgill@cs.wustl.edu>
//
// ============================================================================

#ifndef SCHEDENTRY_H
#define SCHEDENTRY_H
#include "ace/pre.h"

#include "sched_export.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "ace/Containers.h"

//////////////////////
// Helper Functions //
//////////////////////


// Forward declaration of classes.
class Task_Entry;
class Task_Entry_Link;
class Dispatch_Entry;
class Dispatch_Entry_Link;
class Dispatch_Proxy_Iterator;


class TAO_RTSched_Export Dispatch_Entry_Link
// = TITLE
//        Dispatch Entry Link
//
// = DESCRIPTION
//    Light-weight sortable "smart pointer" to a dispatch entry.
//
{
public:

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;

  Dispatch_Entry_Link (Dispatch_Entry &d);
    // Constructor.

  Dispatch_Entry_Link (const Dispatch_Entry_Link &d);
    // Copy constructor.

  ~Dispatch_Entry_Link ();
    // Destructor.

  int operator < (const Dispatch_Entry_Link &d) const;
    // Less than comparison operator.

  Dispatch_Entry &dispatch_entry () const;
    // Accessor for reference to the dispatch entry.

private:

  Dispatch_Entry &dispatch_entry_;
    // Dispatch entry to which the smart pointer refers.
};



class TAO_RTSched_Export Task_Entry
// = TITLE
//    Task Entry.
//
// = DESCRIPTION
//    Wrapper for the RT_Info, which aggregates all its dispatches.
//
{
public:

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;

  // Info for DFS traversal, topological sort of call graph.
  enum DFS_Status {NOT_VISITED, VISITED, FINISHED};

  // Status of merging dispatches.
  enum Propagation_Status {SUCCEEDED,
                           TWO_WAY_DISJUNCTION,
                           TWO_WAY_CONJUNCTION,
                           INTERNAL_ERROR,
                           UNRECOGNIZED_INFO_TYPE};

  // Constructor.
  Task_Entry ();

  // Destructor.
  ~Task_Entry ();

  // Merges dispatches according to info type, update
  // relevant scheduling characteristics for this entry.
  // Returns 0 if all is well, or -1 if an error occurred.
  Propagation_Status merge_dispatches (
    ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
    ACE_CString & unresolved_locals,
    ACE_CString & unresolved_remotes);

  // Gets the pointer to the underlying RT_Info.
  RT_Info *rt_info () const;

  // Sets the pointer to the underlying RT_Info.
  void rt_info (RT_Info *info);

  // Gets the effective period for the task entry.
  Period effective_period () const;

  // Sets the effective period for the task entry.
  void effective_period (Period p);

  // Sets when the node was discovered in DFS traversal.
  void discovered (long l);

  // Gets when the node was discovered in DFS traversal.
  long discovered () const;

  // Sets when the node was finished in DFS traversal.
  void finished (long l);

  // Gets when the node was finished in DFS traversal.
  long finished () const;

  // Sets DFS traversal status of the node.
  void dfs_status (DFS_Status ds);

  // Gets DFS traversal status of the node.
  DFS_Status dfs_status () const;

  // Sets a flag indicating whether node is a thread delineator.
  void is_thread_delineator (int i);

  // Gets the flag indicating whether node is a thread delineator.
  int is_thread_delineator () const;

  // Sets a flag indicating whether node has unresolved remote dependencies.
  void has_unresolved_remote_dependencies (int i);

  // Gets the flag indicating whether node has unresolved remote dependencies.
  int has_unresolved_remote_dependencies () const;

  // Sets a flag indicating whether node has unresolved local dependencies.
  void has_unresolved_local_dependencies (int i);

  // Gets the flag indicating whether node has unresolved local dependencies.
  int has_unresolved_local_dependencies () const;

  // Gets the set of links to Task Entries which this entry calls.
  ACE_Unbounded_Set <Task_Entry_Link *> & calls ();

  // Gets the set of links to Task Entries which call this entry.
  ACE_Unbounded_Set <Task_Entry_Link *> & callers ();

  // Gets the set of arrivals in the effective period.
  ACE_Ordered_MultiSet<Dispatch_Entry_Link> &dispatches ();

  // Gets the type of RT_Info the entry wraps.
  Info_Type info_type () const;

  // Gets the effective execution time for the task entry.
  u_long effective_execution_time () const;

private:

  // Prohibits calls of the given type: currently used to enforce
  // the notion that two-way calls to disjunctive or conjunctive
  // RT_Infos do not have any defined meaning, and thus should be
  // considered dependency specification errors: if these constraints
  // are removed in the future, this method should be removed as well.
  // Returns 0 if all is well, or -1 if an error has occurred.
  int prohibit_dispatches (Dependency_Type dt);

  // Performs a disjunctive merge of arrival times of calls of the given
  // type: all arrival times of all callers of that type are duplicated by
  // the multiplier and repetition over the new frame size and merged.
  // Returns 0 if all is well, or -1 if an error has occurred.
  int disjunctive_merge (
    Dependency_Type dt,
    ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
    ACE_CString & unresolved_locals,
    ACE_CString & unresolved_remotes);

  // Performs a conjunctive merge of arrival times of calls of the given
  // type: all arrival times of all callers of that type are duplicated
  // by the multiplier and repetition over the new frame size and then
  // iteratively merged by choosing the maximal arrival time at
  // the current position in each queue (iteration is in lockstep
  // over all queues, and ends when any queue ends).  Returns 0 if
  // all is well, or -1 if an error has occurred.
  int conjunctive_merge (
    Dependency_Type dt,
    ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
    ACE_CString & unresolved_locals,
    ACE_CString & unresolved_remotes);

  // This static method is used to reframe an existing dispatch set
  // to the given new period multiplier, creating new instances of
  // each existing dispatch (with adjusted arrival and deadline)
  // in each successive sub-frame.  Returns 1 if the set was reframed
  // to a new period, 0 if the set was not changed (the new period
  // was not a multiple of the old one), or -1 if an error occurred.
  static int reframe (ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
                      Task_Entry &owner,
                      ACE_Ordered_MultiSet <Dispatch_Entry_Link> &set,
                      u_long &set_period, u_long new_period);

  // This static method is used to merge an existing dispatch set,
  // multiplied by the given multipliers for the period and number of
  // instances in each period of each existing dispatch, into the
  // given "into" set, without affecting the "from set". Returns 1 if
  // the source set was correctly merged into the destination set,
  // 0 if nothing happened, and -1 if an error occurred.
  static int merge_frames (ACE_Unbounded_Set <Dispatch_Entry *> &dispatch_entries,
                           Task_Entry &owner,
                           ACE_Ordered_MultiSet <Dispatch_Entry_Link> &dest,
                           ACE_Ordered_MultiSet <Dispatch_Entry_Link> &src,
                           u_long &dest_period,
                           u_long src_period,
                           u_long number_of_calls = 1,
                           u_long starting_dest_sub_frame = 0);

  // A pointer to the underlying RT_Info.
  RT_Info *rt_info_;

  // The effective period for the task entry.
  u_long effective_period_;

  // The set of arrivals in the entry's effective period.
  ACE_Ordered_MultiSet<Dispatch_Entry_Link> dispatches_;

  // Depth-first-search status of the entry.
  DFS_Status dfs_status_;

  // Depth-first-search discovery order of the entry.
  long discovered_;

  // Depth-first-search completion order of the entry.
  long finished_;

  // Flag identifying threads in the call graph.
  int is_thread_delineator_;

  // Flag indicating whether or not there are unresolved remote
  // dependencies in the entry's dependency call chain.
  int has_unresolved_remote_dependencies_;

  // Flag indicating whether or not there are unresolved local
  // dependencies in the entry's dependency call chain.
  int has_unresolved_local_dependencies_;

  // Set of links to Task Entries which this entry calls.
  ACE_Unbounded_Set <Task_Entry_Link *> calls_;

  // Set of links to Task Entries which call this entry.
  ACE_Unbounded_Set <Task_Entry_Link *> callers_;

};


// Wrapper for dependencies between RT_Infos
class TAO_RTSched_Export Task_Entry_Link
{
public:

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;

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

  // the number of calls of the operation
  CORBA::Long number_of_calls_;

  // the calling operation
  Task_Entry &caller_;

  // the called operation
  Task_Entry &called_;

  // the type of call dependency
  Dependency_Type dependency_type_;
};


class TAO_RTSched_Export Dispatch_Entry
{
// = TITLE
//        Dispatch Entry
//
// = DESCRIPTION
//     Descriptor object for a single dispatch of an operation.
//
public:

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;

  typedef u_long Dispatch_Id;

  // ctor
  Dispatch_Entry (Time arrival,
                  Time deadline,
                  Preemption_Priority priority,
                  OS_Priority os_priority,
                  Task_Entry &task_entry,
                  Dispatch_Entry *original_dispatch = 0);

  // copy ctor
  Dispatch_Entry (const Dispatch_Entry &d);

  // id accessor
  Dispatch_Id dispatch_id () const;

  // arrival accessor
  Time arrival () const;

  // deadline accessor
  Time deadline () const;

  // scheduler priority accessor and mutator
  Preemption_Priority priority () const;
  void priority (Preemption_Priority p);

  // scheduler priority accessor and mutator
  OS_Priority OS_priority () const;
  void OS_priority (OS_Priority p);

  // dynamic subpriority accessor and mutator
  Sub_Priority dynamic_subpriority () const;
  void dynamic_subpriority (Sub_Priority p);

  // static subpriority accessor and mutator
  Sub_Priority static_subpriority () const;
  void static_subpriority (Sub_Priority p);

  // task entry accessor
  Task_Entry &task_entry () const;

  // LT comparator
  //  TBD - make this a global comparison operator
  //  instead of a class member function
  int operator < (const Dispatch_Entry &d) const;

  // accessor for pointer to original dispatch
  Dispatch_Entry *original_dispatch ();

private:
  // TBD - add reference counting to Dispatch Entry class,
  // make the link a friend, up/down count as links come and go,
  // and call entry dtor when ref count drops to 0

  // stores the next dispatch entry id to be used
  static Dispatch_Id next_id_;

  // the id of the current dispatch entry
  Dispatch_Id dispatch_id_;

  // scheduler priority of the current dispatch entry
  Preemption_Priority priority_;

  // OS priority of the current dispatch entry
  OS_Priority OS_priority_;

  // scheduler dynamic subpriority of the current dispatch entry
  Sub_Priority dynamic_subpriority_;

  // scheduler static subpriority of the current dispatch entry
  Sub_Priority static_subpriority_;

  // the arrival time of the current dispatch entry
  Time arrival_;

  // the deadline of the current dispatch entry
  Time deadline_;

  // stores the id of the related task entry
  Task_Entry &task_entry_;

  // stores a pointer to the original dispatch entry if this
  // is a dispatch generated by expanding the original frame
  Dispatch_Entry *original_dispatch_;

};

class TAO_RTSched_Export Dispatch_Proxy_Iterator
// = TITLE
//   This class implements an iterator abstraction over a virtual
//   frame size and number of calls, using an actual ordered
//   multiset of dispatch entries over an actual frame size.
//   It also serves as a proxy for the virtual dispatch to which
//   it refers.  Rhetorical question: is it possible to separate
//   the iterator and proxy abstractions here without defeating the
//   purpose of the design, which is to avoid constructing
//   superfluous dispatch entries (per the conjunctive merge use case) ?
{
public:

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;

  Dispatch_Proxy_Iterator (ACE_Ordered_MultiSet <Dispatch_Entry_Link> &set,
                           u_long actual_frame_size,
                           u_long virtual_frame_size,
                           u_long number_of_calls_ = 1,
                           u_long starting_sub_frame = 0);
    // ctor

  ////////////////////////
  // iterator interface //
  ////////////////////////

  int done () const;
    // returns 0 if there are more entries to see, 1 if not

  int first (u_int sub_frame = 0);
    // positions the iterator at the first entry of the passed
    // sub-frame, returns 1 if it could position the iterator
    // correctly, 0 if not, and -1 if an error occurred.

  int last ();
    // positions the iterator at the last entry of the total
    // frame, returns 1 if it could position the iterator
    // correctly, 0 if not, and -1 if an error occurred.

  int advance ();
    // positions the iterator at the next entry of the total
    // frame, returns 1 if it could position the iterator
    // correctly, 0 if not, and -1 if an error occurred.

  int retreat ();
    // positions the iterator at the previous entry of the total
    // frame, returns 1 if it could position the iterator
    // correctly, 0 if not, and -1 if an error occurred.

  /////////////////////
  // proxy interface //
  /////////////////////

  Time arrival () const;
    // returns the adjusted arrival time of the virtual entry

  Time deadline () const;
    // returns the adjusted deadline time of the virtual entry

  Preemption_Priority priority () const;
    // returns the scheduler priority of the virtual entry

  OS_Priority OS_priority () const;
    // returns the OS priority of the virtual entry


private:

  u_long number_of_calls_;
    // the number of calls corresponding to each actual dispatch

  u_long current_call_;
    // the current call number for this dispatch (zero based)

  u_long actual_frame_size_;
    // the frame size of the actual dispatches

  u_long virtual_frame_size_;
    // the virtaul frame size over which to iterate

  u_long current_frame_offset_;
    // the current offset into the virtual frame
    // (should be a multiple of the actual frame size)

  ACE_Ordered_MultiSet_Iterator <Dispatch_Entry_Link> iter_;
};



class TAO_RTSched_Export TimeLine_Entry
{
public:

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;

  // time slice constructor
  TimeLine_Entry (Dispatch_Entry &dispatch_entry,
                  Time start,
                  Time stop,
                  Time arrival,
                  Time deadline,
                  TimeLine_Entry *next = 0,
                  TimeLine_Entry *prev = 0);

  // dispatch entry accessor
  Dispatch_Entry &dispatch_entry () const;

  // accessors for time slice start and stop times (100 nanoseconds)
  Time start () const;
  Time stop () const;
  Time arrival () const;
  Time deadline () const;

  // accessor and mutator for next and prev slices for this dispatch
  TimeLine_Entry *next (void) const;
  void next (TimeLine_Entry *);
  TimeLine_Entry *prev (void) const;
  void prev (TimeLine_Entry *);

  int operator < (const TimeLine_Entry&) const;

private:

  // the dispatch entry to which the time slice corresponds
  Dispatch_Entry &dispatch_entry_;

  // priority time slice times (100 nanoseconds)
  Time start_;
  Time stop_;
  Time arrival_;
  Time deadline_;

  // next and previous priority time slices for this dispatch entry
  TimeLine_Entry *next_;
  TimeLine_Entry *prev_;

};

class TAO_RTSched_Export TimeLine_Entry_Link
{
public:

  typedef RtecScheduler::handle_t handle_t;
  typedef RtecScheduler::Dependency_Info Dependency_Info;
  typedef RtecScheduler::Preemption_Priority_t Preemption_Priority;
  typedef RtecScheduler::OS_Priority OS_Priority;
  typedef RtecScheduler::Preemption_Subpriority_t Sub_Priority;
  typedef RtecScheduler::RT_Info RT_Info;
  typedef RtecScheduler::Time Time;
  typedef RtecScheduler::Period_t Period;
  typedef RtecScheduler::Info_Type_t Info_Type;
  typedef RtecScheduler::Dependency_Type_t Dependency_Type;

  TimeLine_Entry_Link  (TimeLine_Entry &t);
  // ctor

  TimeLine_Entry &entry () const;
  // accessor for the underlying entry

  int operator < (const TimeLine_Entry_Link&) const;
  // comparison operator

private:

  TimeLine_Entry &entry_;
  // the underlying entry

};


#if defined (__ACE_INLINE__)
#include "SchedEntry.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* SCHEDENTRY_H */

// EOF
