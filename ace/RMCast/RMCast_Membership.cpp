//
// $Id$
//

#include "RMCast_Membership.h"
#include "RMCast_Proxy.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Membership.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Membership, "$Id$")

ACE_RMCast_Membership::~ACE_RMCast_Membership (void)
{
}

int
ACE_RMCast_Membership::ack (ACE_RMCast::Ack &ack)
{
  Proxy_Iterator end = this->proxies_.end ();
  Proxy_Iterator i = this->proxies_.begin ();
  if (i == end)
    return 0;

  ACE_RMCast::Ack next_ack;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (ack.highest_in_sequence < this->highest_in_sequence_)
      {
        // @@ This violates an invariant of the class, shouldn't
        // happen...
        return -1;
      }
    else if (ack.highest_in_sequence == this->highest_in_sequence_)
      {
        // Nothing new, just continue....
        return 0;
      }
    // Possible update, re-evaluate the story...

    ACE_UINT32 highest_in_sequence = (*i)->highest_in_sequence ();
    ACE_UINT32 highest_received = (*i)->highest_received ();
    ++i;

    for (; i != end; ++i)
      {
        ACE_UINT32 s = (*i)->highest_in_sequence ();
        if (s < highest_in_sequence)
          highest_in_sequence = s;
        ACE_UINT32 r = (*i)->highest_received ();
        if (r > highest_received)
          highest_received = r;
      }
    if (this->highest_in_sequence_ >= highest_in_sequence
        || this->highest_received_ < highest_received)
      {
        // No change....
        return 0;
      }
    this->highest_in_sequence_ = highest_in_sequence;
    this->highest_received_ = highest_received;
    if (this->next () == 0)
      return 0;
    next_ack.source = ack.source;
    next_ack.highest_in_sequence = this->highest_in_sequence_;
    next_ack.highest_received = this->highest_received_;
  }
  // @@ This looks like a race condition, next() is checked inside the
  // lock and used outside, but it is not: the application is only
  // supposed to change next() at construction time.
  return this->next ()->ack (next_ack);
}

int
ACE_RMCast_Membership::join (ACE_RMCast::Join &join)
{
  if (join.source == 0)
    return 0;

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (this->proxies_.insert (join.source) == -1)
      return -1;
  }

  if (this->next () == 0)
    return 0;

  return this->next ()->join (join);
}

int
ACE_RMCast_Membership::leave (ACE_RMCast::Leave &leave)
{
  if (leave.source == 0)
    return 0;

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (this->proxies_.remove (leave.source) == -1)
      return 0;
  }

  if (this->next () == 0)
    return 0;

  return this->next ()->leave (leave);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set<ACE_RMCast_Proxy*>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
