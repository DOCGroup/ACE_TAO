
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
#include "ace/pre.h"

#include "tao/ORB_Core.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"

class TAO_Export TAO_Leader_Follower
{
public:
  /// Constructor
  TAO_Leader_Follower (TAO_ORB_Core *orb_core);

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
  int leader_available (void) const;

  /// A server thread is making a request.
  void set_client_thread (void);

  /// A server thread has finished is making a request.
  void reset_client_thread (void);

  /// The current thread has become the leader thread in the
  /// client side leader-follower set.
  void set_client_leader_thread (void) ;

  /// The current thread is no longer the leader thread in the client
  /// side leader-follower set.
  void reset_client_leader_thread (void) ;

  /// sets the thread ID of the leader thread in the leader-follower
  /// model
  void set_client_leader_thread (ACE_thread_t thread_ID);

  /// checks if we are a leader thread
  int is_client_leader_thread (void) const;

  /**
   * A leader thread is relinquishing its role, unless there are more
   * leader threads running pick up a follower (if there is any) to
   * play the leader role.
   */
  int elect_new_leader (void);

  /**
   * adds the a follower to the set of followers in the leader-
   * follower model
   * returns 0 on success, -1 on failure and 1 if the element is
   * already there.
   */
  int add_follower (TAO_SYNCH_CONDITION *follower_ptr);

  /// checks for the availablity of a follower
  /// returns 1 on available, 0 else
  int follower_available (void) const;

  /// removes a follower from the leader-follower set
  /// returns 0 on success, -1 on failure
  int remove_follower (TAO_SYNCH_CONDITION *follower_ptr);

  /// returns randomly a follower from the leader-follower set
  /// returns follower on success, else 0
  TAO_SYNCH_CONDITION *get_next_follower (void);

  /// Accessors
  TAO_SYNCH_MUTEX &lock (void);
  ACE_Reverse_Lock<TAO_SYNCH_MUTEX> &reverse_lock (void);

  /// Check if there are any client threads running
  int has_clients (void) const;

  /// Accesor to the reactor
  ACE_Reactor *reactor (void);

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

private:
  /// The orb core
  TAO_ORB_Core *orb_core_;

  /// To synchronize access to the members.
  TAO_SYNCH_MUTEX lock_;

  /// do protect the access to the following three members
  ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock_;

  ACE_Unbounded_Set<TAO_SYNCH_CONDITION *> follower_set_;
  // keep a set of followers around (protected)

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

class TAO_Export TAO_LF_Event_Loop_Thread_Helper
{
public:
  /// Constructor
  TAO_LF_Event_Loop_Thread_Helper (TAO_Leader_Follower &leader_follower);

  /// Destructor
  ~TAO_LF_Event_Loop_Thread_Helper (void);

  /// Calls <set_event_loop_thread> on the leader/followers object.
  int set_event_loop_thread (ACE_Time_Value *max_wait_time);

private:
  /// Reference to leader/followers object.
  TAO_Leader_Follower &leader_follower_;

  /// Remembers whether we have to call the reset method in the
  /// destructor.
  int call_reset_;
};

#if defined (__ACE_INLINE__)
# include "tao/Leader_Follower.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_LEADER_FOLLOWER_H */
