// $Id$

#include "tao/Leader_Follower.h"
#include "tao/Resource_Factory.h"

#if !defined (__ACE_INLINE__)
# include "tao/Leader_Follower.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Leader_Follower, "$Id$")

TAO_Leader_Follower::~TAO_Leader_Follower (void)
{
  delete this->reactor_;
}

ACE_SYNCH_CONDITION*
TAO_Leader_Follower::get_next_follower (void)
{
  ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION *> iterator (
    this->follower_set_);

  if (iterator.first () == 0)
    // means set is empty
    return 0;

  ACE_SYNCH_CONDITION *cond = *iterator;

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
  (void) this->remove_follower (cond); // Ignore errors..

  return cond;
}

int
TAO_Leader_Follower::wait_for_client_leader_to_complete (ACE_Time_Value *max_wait_time)
{
  ACE_Countdown_Time countdown (max_wait_time);

  while (this->client_thread_is_leader_)
    {
      ACE_SYNCH_CONDITION *condition_variable =
        this->orb_core_->leader_follower_condition_variable ();

      if (this->add_follower (condition_variable) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot add to condition variable collection\n"),
                            -1);
        }

      if (max_wait_time == 0)
        {
          if (condition_variable->wait () == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "Condition variable wait failed\n"),
                                -1);
            }
        }
      else
        {
          countdown.update ();
          ACE_Time_Value tv = ACE_OS::gettimeofday ();
          tv += *max_wait_time;
          if (condition_variable->wait (&tv) == -1)
            {
              if (errno != ETIME)
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "Condition variable wait failed\n"),
                                  -1);
              return -1;
            }
        }
    }

  return 0;
}

ACE_Reactor *
TAO_Leader_Follower::reactor (void)
{
  if (this->reactor_ == 0)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock (), 0);
      if (this->reactor_ == 0)
        {
          this->reactor_ =
            this->orb_core_->resource_factory ()->get_reactor ();
        }
    }
  return this->reactor_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_SYNCH_CONDITION*>;
template class ACE_Unbounded_Set<ACE_SYNCH_CONDITION*>;
template class ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_SYNCH_CONDITION*>
#pragma instantiate ACE_Unbounded_Set<ACE_SYNCH_CONDITION*>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_SYNCH_CONDITION*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
