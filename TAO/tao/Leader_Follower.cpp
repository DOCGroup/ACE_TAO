// $Id$

#include "Leader_Follower.h"
#include "Resource_Factory.h"

#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "Leader_Follower.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Leader_Follower, "$Id$")

TAO_Leader_Follower::~TAO_Leader_Follower (void)
{
  // Hand the reactor back to the resource factory.
  this->orb_core_->resource_factory ()->reclaim_reactor (this->reactor_);
  this->reactor_ = 0;
}

TAO_Leader_Follower::TAO_Follower_Node::TAO_Follower_Node (TAO_SYNCH_CONDITION* follower_ptr)
  : follower_ (follower_ptr),
    next_ (0)
{

}


TAO_Leader_Follower::TAO_Follower_Queue::TAO_Follower_Queue (void)
  : head_ (0),
    tail_ (0)
{

}

int
TAO_Leader_Follower::TAO_Follower_Queue::insert (TAO_Follower_Node* node)
{
  if (this->head_ == 0) {
    this->head_ = node;
    this->tail_ = node;
    // Make sure that we don't have garbage in the case when the same node
    // is added a second time. This is necessary as the nodes are on the
    // stack.
    node->next_ = 0;
  }
  else
    {
      // Add the node to the tail and modify the pointers
      TAO_Follower_Node* temp = this->tail_;
      temp->next_ = node;
      this->tail_ = node;
      node->next_ = 0;
    }
  return 0;
}

int
TAO_Leader_Follower::TAO_Follower_Queue::remove (TAO_Follower_Node* node)
{
  TAO_Follower_Node* prev = 0;
  TAO_Follower_Node* curr = 0;

  // No followers in queue, return
  if (this->head_ == 0)
    return -1;

  // Check is for whether we have the same condition variable on the
  // queue rather than the same node structure which wraps it.
  for (curr = this->head_;
       curr != 0 && curr->follower_ != node->follower_;
       curr = curr->next_)
    {
      prev = curr;
    }

  // Entry not found in the queue
  if (curr == 0)
    return -1;
  // Entry found at the head of the queue
  else if (prev == 0)
    this->head_ = this->head_->next_;
  else
    prev->next_ = curr->next_;
  // Entry at the tail
  if (curr->next_ == 0)
    this->tail_ = prev;

  return 0;
}


TAO_SYNCH_CONDITION*
TAO_Leader_Follower::get_next_follower (void)
{
  // If the queue is empty return
  if (this->follower_set_.is_empty())
    return 0;

  TAO_Follower_Node* next_follower = this->follower_set_.head_;

  TAO_SYNCH_CONDITION *cond = next_follower->follower_;

#if defined (TAO_DEBUG_LEADER_FOLLOWER)
  ACE_DEBUG ((LM_DEBUG,
              "TAO (%P|%t) LF::get_next_follower - "
              "follower is %x\n",
              cond));
#endif /* TAO_DEBUG_LEADER_FOLLOWER */

  // We *must* remove it when we signal it so the same condition is
  // not signalled for both wake up as a follower and as the next
  // leader.
  // The follower may not be there if the reply is received while the
  // consumer is not yet waiting for it (i.e. it send the request but
  // has not blocked to receive the reply yet)
  (void) this->remove_follower (next_follower); // Ignore errors..

  return cond;
}

int
TAO_Leader_Follower::wait_for_client_leader_to_complete (ACE_Time_Value *max_wait_time)
{
  int result = 0;
  ACE_Countdown_Time countdown (max_wait_time);

  // Note that we are waiting.
  ++this->event_loop_threads_waiting_;

  while (this->client_thread_is_leader_ &&
         result != -1)
    {
      if (max_wait_time == 0)
        {
          if (this->event_loop_threads_condition_.wait () == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t): TAO_Leader_Follower::wait_for_client_leader_to_complete - ")
                          ACE_TEXT ("Condition variable wait failed\n")));

              result = -1;
            }
        }
      else
        {
          countdown.update ();
          ACE_Time_Value tv = ACE_OS::gettimeofday ();
          tv += *max_wait_time;
          if (this->event_loop_threads_condition_.wait (&tv) == -1)
            {
              if (errno != ETIME)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t): TAO_Leader_Follower::wait_for_client_leader_to_complete - ")
                            ACE_TEXT ("Condition variable wait failed\n")));

              result = -1;
            }
        }
    }

  // Reset waiting state.
  --this->event_loop_threads_waiting_;

  return result;
}

ACE_Reactor *
TAO_Leader_Follower::reactor (void)
{
  if (this->reactor_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock (), 0);
      if (this->reactor_ == 0)
        {
          this->reactor_ =
            this->orb_core_->resource_factory ()->get_reactor ();
        }
    }
  return this->reactor_;
}

void
TAO_Leader_Follower::set_client_thread (void)
{
  // If we were a leader thread or an event loop thread, give up
  // leadership.
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->event_loop_thread_ ||
      tss->client_leader_thread_)
    {
      --this->leaders_;
    }

  if (this->clients_ == 0 &&
      this->orb_core_->has_shutdown ())
    {
      // The ORB has shutdown and we are the first client after
      // that. This means that the reactor is disabled, we must
      // re-enable it if we want to receive any replys...
      this->orb_core_->reactor ()->reset_reactor_event_loop ();
    }
  this->clients_++;
}

void
TAO_Leader_Follower::reset_client_thread (void)
{
  // If we were a leader thread or an event loop thread, take back
  // leadership.
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
  if (tss->event_loop_thread_ ||
      tss->client_leader_thread_)
    {
      ++this->leaders_;
    }

  this->clients_--;
  if (this->clients_ == 0 && this->orb_core_->has_shutdown ())
    {
      // The ORB has shutdown and we are the last client thread, we
      // must stop the reactor to ensure that any server threads go
      // away.
      this->orb_core_->reactor ()->end_reactor_event_loop ();
    }
}

TAO_LF_Strategy::TAO_LF_Strategy ()
{
}

TAO_LF_Strategy::~TAO_LF_Strategy ()
{
}

TAO_Complete_LF_Strategy::TAO_Complete_LF_Strategy ()
{
}

TAO_Complete_LF_Strategy::~TAO_Complete_LF_Strategy ()
{
}

void
TAO_Complete_LF_Strategy::set_upcall_thread (TAO_Leader_Follower &leader_follower)
{
  leader_follower.set_upcall_thread ();
}

int
TAO_Complete_LF_Strategy::set_event_loop_thread (ACE_Time_Value *max_wait_time,
                                                 TAO_Leader_Follower &leader_follower)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, leader_follower.lock (), -1);

  return leader_follower.set_event_loop_thread (max_wait_time);
}

void
TAO_Complete_LF_Strategy::reset_event_loop_thread_and_elect_new_leader (int call_reset,
                                                                        TAO_Leader_Follower &leader_follower)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, leader_follower.lock ());

  if (call_reset)
    leader_follower.reset_event_loop_thread ();

  int result = leader_follower.elect_new_leader ();

  if (result == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TAO (%P|%t) Failed to wake up ")
                ACE_TEXT ("a follower thread\n")));
}

TAO_Null_LF_Strategy::TAO_Null_LF_Strategy ()
{
}

TAO_Null_LF_Strategy::~TAO_Null_LF_Strategy ()
{
}

void
TAO_Null_LF_Strategy::set_upcall_thread (TAO_Leader_Follower &)
{
}

int
TAO_Null_LF_Strategy::set_event_loop_thread (ACE_Time_Value *, 
                                             TAO_Leader_Follower &)
{
  return 0;
}

void
TAO_Null_LF_Strategy::reset_event_loop_thread_and_elect_new_leader (int,
                                                                    TAO_Leader_Follower &)
{
}

