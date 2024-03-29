// -*- C++ -*-

//=============================================================================
/**
 *  @file   Leader_Follower.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_LEADER_FOLLOWER_H
#define TAO_LEADER_FOLLOWER_H

#include /**/ "ace/pre.h"
#include "ace/os_include/os_errno.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/New_Leader_Generator.h"
#include "tao/LF_Follower.h"
#include "ace/Reverse_Lock_T.h"
#include "ace/Intrusive_List.h"
#include "ace/Intrusive_List_Node.h"
#include "ace/OS_NS_Thread.h"
#include "ace/Event_Handler.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LF_Event;
class TAO_Transport;
class TAO_ORB_Core;
class TAO_ORB_Core_TSS_Resources;

/**
 * @class TAO_Leader_Follower
 *
 * @brief TAO_Leader_Follower
 *
 * TAO_Leader_Follower
 */
class TAO_Export TAO_Leader_Follower
{
public:
  /// Constructor
  TAO_Leader_Follower (TAO_ORB_Core *orb_core,
                       TAO_New_Leader_Generator *new_leader_generator = 0);

  /// Destructor
  ~TAO_Leader_Follower ();

  /// The current thread has become a server thread (i.e. called
  /// ORB::run), update any flags and counters.
  int set_event_loop_thread (ACE_Time_Value *max_wait_time);

  /// The current thread is not a server thread anymore, reset any
  /// flags and counters.
  void reset_event_loop_thread ();

  /// This thread is going to perform an upcall, it will no longer be
  /// an event loop thread.
  void set_upcall_thread ();

  /// Is there any thread running as a leader?
  bool leader_available () const;

  /// A server thread is making a request.
  void set_client_thread ();

  /// A server thread has finished is making a request.
  void reset_client_thread ();

  /// Wait on the Leader/Followers loop until one event happens.
  /**
   * @param event The event we wait for, the loop iterates until the
   * event is successful, or it fails due to timeout, and error or a
   * connection closed.
   * @param transport The transport attached to the event
   * @param max_wait_time Limit the time spent on the loop
   * @return Returns -1 on error, 0 or non-zero value
   *  otherwise.
   *
   * @todo Document this better, split the Follower code to the
   * TAO_LF_Follower class, we probably don't need the transport
   * object.
   */
  int wait_for_event (TAO_LF_Event *event,
                      TAO_Transport *transport,
                      ACE_Time_Value *max_wait_time);

  /// The current thread has become the leader thread in the
  /// client side leader-follower set.
  void set_client_leader_thread ();

  /// The current thread is no longer the leader thread in the client
  /// side leader-follower set.
  void reset_client_leader_thread ();

  /// Sets the thread ID of the leader thread in the leader-follower
  /// model
  void set_client_leader_thread (ACE_thread_t thread_ID);

  /// Checks if we are a leader thread
  bool is_client_leader_thread () const;

  /// Sets the client leader avoidance flag true. This is used with the
  /// noupcall wait strategy to allow the creation of dynamic threads
  /// when possible
  void set_avoid_client_leader ();

  /**
   * A leader thread is relinquishing its role, unless there are more
   * leader threads running pick up a follower (if there is any) to
   * play the leader role.
   */
  int elect_new_leader ();

  /** @name Follower creation/destruction
   *
   * The Leader/Followers set acts as a factory for the Follower
   * objects.  Followers are used to represent a thread blocked
   * waiting in the Follower set.
   *
   * The Leader/Followers abstraction keeps a list of the waiting
   * followers, so it can wake up one when the leader thread stops
   * handling events.
   *
   * For performance reasons the Leader/Followers set uses a pool (or
   * free-list) to keep Follower objects unattached to any thread.  It
   * could be tempting to use TSS to keep such followers, after all a
   * thread can only need one such Follower object, however, that does
   * not work with multiple Leader/Followers sets, consult this bug
   * report for more details:
   *
   * http://bugzilla.dre.vanderbilt.edu/show_bug.cgi?id=296
   *
   */
  //@{
  /// Allocate a new follower to the caller.
  TAO_LF_Follower *allocate_follower ();

  /// The caller has finished using a follower.
  void release_follower (TAO_LF_Follower *);
  //@}

  /** @name Follower Set Operations
   *
   */
  //@{
  /// Add a new follower to the set
  void add_follower (TAO_LF_Follower *follower);

  /// Removes a follower from the leader-follower set
  void remove_follower (TAO_LF_Follower *follower);

  /// Checks if there are any followers available
  /**
   * @return true if there follower set is not empty
   */
  bool follower_available () const;

  //@}

  /// Get a reference to the underlying mutex
  TAO_SYNCH_MUTEX &lock ();

  /// Provide a pre-initialized reverse lock for the Leader/Followers
  /// set.
  /**
   * The Leader/Followers set mutex must be release during some long
   * running operations.  This helper class simplifies the process of
   * releasing and reacquiring said mutex.
   */
  ACE_Reverse_Lock<TAO_SYNCH_MUTEX> &reverse_lock ();

  /// Check if there are any client threads running
  bool has_clients () const;

  /// Accessor to the reactor
  ACE_Reactor *reactor ();

  /// Called when we are out of leaders.
  bool no_leaders_available ();

  /// Set the new leader generator.
  void set_new_leader_generator(TAO_New_Leader_Generator *new_leader_generator);

  /// Method to support deferring an event till later (f.i. in
  /// cases where transport()->wait_strategy()->can_process_upcalls()
  /// returns false).
  int defer_event (ACE_Event_Handler*);

private:
  /// Shortcut to obtain the TSS resources of the orb core.
  TAO_ORB_Core_TSS_Resources *get_tss_resources () const;

  /// Wait for the client leader to complete.
  int wait_for_client_leader_to_complete (ACE_Time_Value *max_wait_time);

  /**
   * Implement the reset_event_loop_thread() method, once the TSS
   * resources have been acquired.
   * Also used in the set_upcall_thread.
   */
  void reset_event_loop_thread_i (TAO_ORB_Core_TSS_Resources *tss);

  /** @name Follower Set Operations
   *
   */
  //@{
  /// Remove a follower from the Followers set and promote it to the
  /// leader role.
  /**
   * This is a helper routine for elect_new_leader(), after verifying
   * that all the pre-conditions are satisfied the Follower set is
   * changed and the promoted Follower is signaled.
   */
  int elect_new_leader_i ();

  //@}

  /// Method to allow the Leader_Follower to resume deferred events
  /// when it is opportune to do so.
  void resume_events ();

private:
  /// The orb core
  TAO_ORB_Core *orb_core_;

  /// To synchronize access to the members.
  TAO_SYNCH_MUTEX lock_;

  /// Do protect the access to the following three members
  ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock_;

  /// Implement the Leader/Followers set using an intrusive list
  typedef ACE_Intrusive_List<TAO_LF_Follower> Follower_Set;
  Follower_Set follower_set_;

  /// Use a free list to allocate and release Follower objects
  Follower_Set follower_free_list_;

  /**
   * Count the number of active leaders.
   * There could be many leaders in the thread pool (i.e. calling
   * ORB::run), and the same leader could show up multiple times as it
   * receives nested upcalls and sends more requests.
   */
  int leaders_;

  /// Count the number of active clients, this is useful to know when
  /// to deactivate the reactor
  int clients_;

  /// The reactor
  ACE_Reactor *reactor_;

  /// Flag to indicate that it is preferable to start a new thread, if possible
  /// rather than become a client leader. In particular when using the no-upcall
  /// wait strategy along with the dynamic thread pool, this is the case. It is
  /// still possible for a client leader to emerge in the case when no additional
  /// threads may be generated.
  bool avoid_client_leader_;

  /// Is a client thread the current leader?
  int client_thread_is_leader_;

  /// Are server threads waiting for the client leader to complete?
  int event_loop_threads_waiting_;

  /// Condition variable for server threads waiting for the client
  /// leader to complete.
  TAO_SYNCH_CONDITION event_loop_threads_condition_;

  /// Leader/Follower class uses this method to notify the system that
  /// we are out of leaders.
  TAO_New_Leader_Generator *new_leader_generator_;

  /// Class used to register deferred event handlers.
  class Deferred_Event
  : public ACE_Intrusive_List_Node<Deferred_Event>
  {
  public:
    Deferred_Event (ACE_Event_Handler* h);

    ACE_Event_Handler* handler () const;
  private:
    ACE_Event_Handler_var eh_;
  };

  /// The set of deferred event handlers.
  typedef ACE_Intrusive_List<Deferred_Event> Deferred_Event_Set;
  Deferred_Event_Set deferred_event_set_;
};

class TAO_Export TAO_LF_Client_Thread_Helper
{
public:
  /// Constructor
  TAO_LF_Client_Thread_Helper (TAO_Leader_Follower &leader_follower);

  /// Destructor
  ~TAO_LF_Client_Thread_Helper ();

private:
  /// Reference to leader/followers object.
  TAO_Leader_Follower &leader_follower_;
};

class TAO_Export TAO_LF_Client_Leader_Thread_Helper
{
public:
  /// Constructor
  TAO_LF_Client_Leader_Thread_Helper (TAO_Leader_Follower &leader_follower);

  /// Destructor
  ~TAO_LF_Client_Leader_Thread_Helper ();

private:
  /// Reference to leader/followers object.
  TAO_Leader_Follower &leader_follower_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Leader_Follower.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_LEADER_FOLLOWER_H */
