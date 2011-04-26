// -*- C++ -*-

//=============================================================================
/**
 *  @file   Leader_Follower.h
 *
 *  $Id$
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
  ~TAO_Leader_Follower (void);

  /// The current thread has become a server thread (i.e. called
  /// ORB::run), update any flags and counters.
  int set_event_loop_thread (ACE_Time_Value *max_wait_time);

  /// The current thread is not a server thread anymore, reset any
  /// flags and counters.
  void reset_event_loop_thread (void);

  /// This thread is going to perform an upcall, it will no longer be
  /// an event loop thread.
  void set_upcall_thread (void);

  /// Is there any thread running as a leader?
  bool leader_available (void) const;

  /// A server thread is making a request.
  void set_client_thread (void);

  /// A server thread has finished is making a request.
  void reset_client_thread (void);

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
  void set_client_leader_thread (void) ;

  /// The current thread is no longer the leader thread in the client
  /// side leader-follower set.
  void reset_client_leader_thread (void) ;

  /// Sets the thread ID of the leader thread in the leader-follower
  /// model
  void set_client_leader_thread (ACE_thread_t thread_ID);

  /// Checks if we are a leader thread
  bool is_client_leader_thread (void) const;

  /**
   * A leader thread is relinquishing its role, unless there are more
   * leader threads running pick up a follower (if there is any) to
   * play the leader role.
   */
  int elect_new_leader (void);

  /** @name Follower creation/destructions
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
  TAO_LF_Follower *allocate_follower (void);

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
  bool follower_available (void) const;

  //@}

  /// Get a reference to the underlying mutex
  TAO_SYNCH_MUTEX &lock (void);

  /// Provide a pre-initialized reverse lock for the Leader/Followers
  /// set.
  /**
   * The Leader/Followers set mutex must be release during some long
   * running operations.  This helper class simplifies the process of
   * releasing and reacquiring said mutex.
   */
  ACE_Reverse_Lock<TAO_SYNCH_MUTEX> &reverse_lock (void);

  /// Check if there are any client threads running
  bool has_clients (void) const;

  /// Accesor to the reactor
  ACE_Reactor *reactor (void);

  /// Called when we are out of leaders.
  void no_leaders_available (void);

  /// Set the new leader generator.
  void set_new_leader_generator(TAO_New_Leader_Generator *new_leader_generator);

private:
  /// Shortcut to obtain the TSS resources of the orb core.
  TAO_ORB_Core_TSS_Resources *get_tss_resources (void) const;

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
  /// Remote a follower from the Followers set and promote it to the
  /// leader role.
  /**
   * This is a helper routine for elect_new_leader(), after verifying
   * that all the pre-conditions are satisfied the Follower set is
   * changed and the promoted Follower is signaled.
   */
  int elect_new_leader_i (void);

  //@}

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
};

class TAO_Export TAO_LF_Client_Thread_Helper
{
public:
  /// Constructor
  TAO_LF_Client_Thread_Helper (TAO_Leader_Follower &leader_follower);

  /// Destructor
  ~TAO_LF_Client_Thread_Helper (void);

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
  ~TAO_LF_Client_Leader_Thread_Helper (void);

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
