// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Leader_Follower.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_LEADER_FOLLOWER_H
#define TAO_LEADER_FOLLOWER_H

#include "tao/ORB_Core.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"

class TAO_Export TAO_Leader_Follower
{
public:
  TAO_Leader_Follower (TAO_ORB_Core *orb_core);
  // Constructor

  ~TAO_Leader_Follower (void);
  // Destructor

  int set_event_loop_thread (ACE_Time_Value *max_wait_time);
  // The current thread has become a server thread (i.e. called
  // ORB::run), update any flags and counters.

  void reset_event_loop_thread (void);
  // The current thread is not a server thread anymore, reset any
  // flags and counters.

  int leader_available (void) const;
  // Is there any thread running as a leader?

  void set_client_thread (void);
  // A server thread is making a request.

  void reset_client_thread (void);
  // A server thread has finished is making a request.

  void set_client_leader_thread (void) ;
  // The current thread has become the leader thread in the
  // client side leader-follower set.

  void reset_client_leader_thread (void) ;
  // The current thread is no longer the leader thread in the client
  // side leader-follower set.

  void set_client_leader_thread (ACE_thread_t thread_ID);
  // sets the thread ID of the leader thread in the leader-follower
  // model

  int is_client_leader_thread (void) const;
  // checks if we are a leader thread

  int elect_new_leader (void);
  // A leader thread is relinquishing its role, unless there are more
  // leader threads running pick up a follower (if there is any) to
  // play the leader role.

  int add_follower (ACE_SYNCH_CONDITION *follower_ptr);
  // adds the a follower to the set of followers in the leader-
  // follower model
  // returns 0 on success, -1 on failure and 1 if the element is
  // already there.

  int follower_available (void) const;
  // checks for the availablity of a follower
  // returns 1 on available, 0 else

  int remove_follower (ACE_SYNCH_CONDITION *follower_ptr);
  // removes a follower from the leader-follower set
  // returns 0 on success, -1 on failure

  ACE_SYNCH_CONDITION *get_next_follower (void);
  // returns randomly a follower from the leader-follower set
  // returns follower on success, else 0

  ACE_SYNCH_MUTEX &lock (void);
  ACE_Reverse_Lock<ACE_SYNCH_MUTEX> &reverse_lock (void);
  // Accessors

  int has_clients (void) const;
  // Check if there are any client threads running

  ACE_Reactor *reactor (void);
  // Accesor to the reactor

private:
  TAO_ORB_Core_TSS_Resources *get_tss_resources (void) const;
  // Shortcut to obtain the TSS resources of the orb core.

  int wait_for_client_leader_to_complete (ACE_Time_Value *max_wait_time);
  // Wait for the client leader to complete.

private:
  TAO_ORB_Core *orb_core_;
  // The orb core

  ACE_SYNCH_MUTEX lock_;
  // To synchronize access to the members.

  ACE_Reverse_Lock<ACE_SYNCH_MUTEX> reverse_lock_;
  // do protect the access to the following three members

  ACE_Unbounded_Set<ACE_SYNCH_CONDITION *> follower_set_;
  // keep a set of followers around (protected)

  int leaders_;
  // Count the number of active leaders.
  // There could be many leaders in the thread pool (i.e. calling
  // ORB::run), and the same leader could show up multiple times as it
  // receives nested upcalls and sends more requests.

  int clients_;
  // Count the number of active clients, this is useful to know when
  // to deactivate the reactor

  ACE_Reactor *reactor_;
  // The reactor

  int client_thread_is_leader_;
  // Is a client thread the current leader?

  int event_loop_threads_waiting_;
  // Are server threads waiting for the client leader to complete?

  ACE_SYNCH_CONDITION event_loop_threads_condition_;
  // Condition variable for server threads waiting for the client
  // leader to complete.
};

class TAO_Export TAO_LF_Client_Thread_Helper
{
public:
  TAO_LF_Client_Thread_Helper (TAO_Leader_Follower &leader_follower);
  // Constructor

  ~TAO_LF_Client_Thread_Helper (void);
  // Destructor

private:
  TAO_Leader_Follower &leader_follower_;
  // Reference to leader/followers object.
};

class TAO_Export TAO_LF_Client_Leader_Thread_Helper
{
public:
  TAO_LF_Client_Leader_Thread_Helper (TAO_Leader_Follower &leader_follower);
  // Constructor

  ~TAO_LF_Client_Leader_Thread_Helper (void);
  // Destructor

private:
  TAO_Leader_Follower &leader_follower_;
  // Reference to leader/followers object.
};

class TAO_Export TAO_LF_Event_Loop_Thread_Helper
{
public:
  TAO_LF_Event_Loop_Thread_Helper (TAO_Leader_Follower &leader_follower);
  // Constructor

  ~TAO_LF_Event_Loop_Thread_Helper (void);
  // Destructor

  int set_event_loop_thread (ACE_Time_Value *max_wait_time);
  // Calls <set_event_loop_thread> on the leader/followers object.

private:
  TAO_Leader_Follower &leader_follower_;
  // Reference to leader/followers object.

  int call_reset_;
  // Remembers whether we have to call the reset method in the
  // destructor.
};

#if defined (__ACE_INLINE__)
# include "tao/Leader_Follower.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_LEADER_FOLLOWER_H */
