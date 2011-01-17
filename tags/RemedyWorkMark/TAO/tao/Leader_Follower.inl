// -*- C++ -*-
//
// $Id$

// ****************************************************************

#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB_Core.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Leader_Follower::TAO_Leader_Follower (TAO_ORB_Core* orb_core,
                                          TAO_New_Leader_Generator *new_leader_generator)
  : orb_core_ (orb_core),
    reverse_lock_ (lock_),
    leaders_ (0),
    clients_ (0),
    reactor_ (0),
    client_thread_is_leader_ (0),
    event_loop_threads_waiting_ (0),
    event_loop_threads_condition_ (lock_),
    new_leader_generator_ (new_leader_generator)
{
}

ACE_INLINE TAO_ORB_Core_TSS_Resources *
TAO_Leader_Follower::get_tss_resources (void) const
{
  return this->orb_core_->get_tss_resources ();
}

ACE_INLINE int
TAO_Leader_Follower::follower_available (void) const
{
  return !this->follower_set_.is_empty ();
}

ACE_INLINE void
TAO_Leader_Follower::no_leaders_available (void)
{
  if (this->new_leader_generator_)
    this->new_leader_generator_->no_leaders_available ();
}

ACE_INLINE int
TAO_Leader_Follower::elect_new_leader (void)
{
  if (this->leaders_ == 0)
    {
      if (this->event_loop_threads_waiting_)
        {
          return this->event_loop_threads_condition_.broadcast ();
        }
      else if (this->follower_available ())
        {
          return this->elect_new_leader_i ();
        }
      else
        {
          this->no_leaders_available ();
        }
    }
  return 0;
}

ACE_INLINE int
TAO_Leader_Follower::set_event_loop_thread (ACE_Time_Value *max_wait_time)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();

  // Make sure that there is no other client thread run the show.  If
  // we are the client thread running the show, then it is ok.
  if (this->client_thread_is_leader_ &&
      tss->client_leader_thread_ == 0)
    {
      int result =
        this->wait_for_client_leader_to_complete (max_wait_time);

      if (result != 0)
        return result;
    }

  // If <event_loop_thread_> == 0 and <client_leader_thread_> == 0, we
  // are running the event loop for the first time.  Therefore,
  // increment the leaders.  Otherwise, simply increment
  // <event_loop_thread_> since either (a) if <event_loop_thread_> !=
  // 0 this is a nested call to the event loop, or (b)
  // <client_leader_thread_> != 0 this is a call to the event loop
  // while we are a client leader.
  if (tss->event_loop_thread_ == 0 &&
      tss->client_leader_thread_ == 0)
    ++this->leaders_;

  ++tss->event_loop_thread_;

  return 0;
}

ACE_INLINE void
TAO_Leader_Follower::reset_event_loop_thread_i (TAO_ORB_Core_TSS_Resources *tss)
{
  // Always decrement <event_loop_thread_>. If <event_loop_thread_>
  // reaches 0 and we are not a client leader, we are done with our
  // duties of running the event loop. Therefore, decrement the
  // leaders.  Otherwise, we just got done with a nested call to the
  // event loop or a call to the event loop when we were the client
  // leader.
  --tss->event_loop_thread_;

  if (tss->event_loop_thread_ == 0 &&
      tss->client_leader_thread_ == 0)
    --this->leaders_;
}

ACE_INLINE void
TAO_Leader_Follower::reset_event_loop_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->event_loop_thread_ > 0)
    this->reset_event_loop_thread_i (tss);
}

ACE_INLINE TAO_SYNCH_MUTEX &
TAO_Leader_Follower::lock (void)
{
  return this->lock_;
}

ACE_INLINE void
TAO_Leader_Follower::set_upcall_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();

  if (tss->event_loop_thread_ > 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock ());
      this->reset_event_loop_thread_i (tss);

      this->elect_new_leader ();
    }
}

ACE_INLINE int
TAO_Leader_Follower::leader_available (void) const
{
  return this->leaders_ != 0;
}

ACE_INLINE void
TAO_Leader_Follower::set_client_leader_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  ++this->leaders_;
  ++this->client_thread_is_leader_;
  ++tss->client_leader_thread_;
}

ACE_INLINE void
TAO_Leader_Follower::reset_client_leader_thread (void)
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  --tss->client_leader_thread_;
  --this->leaders_;
  --this->client_thread_is_leader_;
}

ACE_INLINE int
TAO_Leader_Follower::is_client_leader_thread (void) const
{
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  return tss->client_leader_thread_ != 0;
}

ACE_INLINE void
TAO_Leader_Follower::add_follower (TAO_LF_Follower *follower)
{
  this->follower_set_.push_back (follower);
}

ACE_INLINE void
TAO_Leader_Follower::remove_follower (TAO_LF_Follower *follower)
{
  this->follower_set_.remove (follower);
}

ACE_INLINE ACE_Reverse_Lock<TAO_SYNCH_MUTEX> &
TAO_Leader_Follower::reverse_lock (void)
{
  return this->reverse_lock_;
}

ACE_INLINE int
TAO_Leader_Follower::has_clients (void) const
{
  return this->clients_;
}

ACE_INLINE void
TAO_Leader_Follower::set_new_leader_generator(TAO_New_Leader_Generator *new_leader_generator)
{
  this->new_leader_generator_ = new_leader_generator;
}

// ****************************************************************

ACE_INLINE
TAO_LF_Client_Thread_Helper::TAO_LF_Client_Thread_Helper (TAO_Leader_Follower &leader_follower)
  : leader_follower_ (leader_follower)
{
  this->leader_follower_.set_client_thread ();
}

ACE_INLINE
TAO_LF_Client_Thread_Helper::~TAO_LF_Client_Thread_Helper (void)
{
  this->leader_follower_.reset_client_thread ();
}

ACE_INLINE
TAO_LF_Client_Leader_Thread_Helper::TAO_LF_Client_Leader_Thread_Helper (TAO_Leader_Follower &leader_follower)
  : leader_follower_ (leader_follower)
{
  this->leader_follower_.set_client_leader_thread ();
}

ACE_INLINE
TAO_LF_Client_Leader_Thread_Helper::~TAO_LF_Client_Leader_Thread_Helper (void)
{
  this->leader_follower_.reset_client_leader_thread ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
