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
  //  ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack\n"));
  Proxy_Iterator end = this->proxies_.end ();
  Proxy_Iterator i = this->proxies_.begin ();
  if (i == end)
    return 0;

  // ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[2]\n"));
  ACE_RMCast::Ack next_ack;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (ack.next_expected < this->next_expected_)
      {
        // @@ This violates an invariant of the class, shouldn't
        // happen...
        // ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[3]\n"));
        return -1;
      }
    else if (ack.next_expected == this->next_expected_)
      {
        // Nothing new, just continue....
        // ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[4]\n"));
        return 0;
      }
    // Possible update, re-evaluate the story...

    ACE_UINT32 next_expected = (*i)->next_expected ();
    ACE_UINT32 highest_received = (*i)->highest_received ();
    ++i;

    for (; i != end; ++i)
      {
        ACE_UINT32 s = (*i)->next_expected ();
        if (s < next_expected)
          next_expected = s;
        ACE_UINT32 r = (*i)->highest_received ();
        if (r > highest_received)
          highest_received = r;
      }
#if 0
    // @@TODO: this is an important feature, disabled until it is
    // fully debugged
    if (this->next_expected_ >= next_expected
        || this->highest_received_ >= highest_received)
      {
        // No change....
        // ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[5]\n"));
        return 0;
      }
#endif /* 0 */
    this->next_expected_ = next_expected;
    this->highest_received_ = highest_received;
    if (this->next () == 0)
      return 0;
    next_ack.source = ack.source;
    next_ack.next_expected = this->next_expected_;
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
    // @@TODO: This may change the next Ack to send up, should
    // recompute and send the right message if that was the case.
  }

  return this->ACE_RMCast_Module::join (join);
}

int
ACE_RMCast_Membership::leave (ACE_RMCast::Leave &leave)
{
  if (leave.source == 0)
    return 0;

  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    (void) this->proxies_.remove (leave.source);
    // @@TODO: This may change the next Ack to send up, should
    // recompute and send the right message if that was the case.
  }

  return this->ACE_RMCast_Module::leave (leave);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set<ACE_RMCast_Proxy*>;
template class ACE_Unbounded_Set_Iterator<ACE_RMCast_Proxy*>;
template class ACE_Node<ACE_RMCast_Proxy*>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
