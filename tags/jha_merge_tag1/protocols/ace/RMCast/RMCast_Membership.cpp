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
ACE_RMCast_Membership::has_members (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);

  Proxy_Iterator end = this->proxies_.end ();
  Proxy_Iterator i = this->proxies_.begin ();
  return (i != end);
}

int
ACE_RMCast_Membership::ack (ACE_RMCast::Ack &ack)
{
  //ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack (%d:%d)\n",
  //            ack.next_expected, ack.highest_received));

  ACE_RMCast::Ack next_ack;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (ack.next_expected < this->next_expected_)
      {
        // @@ This violates an invariant of the class, shouldn't
        // happen...
        //ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[2]\n"));
        return 0;
      }
    else if (ack.next_expected == this->next_expected_)
      {
        // Nothing new, just continue....
        //ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[3]\n"));
        return 0;
      }

    int r = this->compute_ack_i (ack.source, next_ack);
    if (r < 0)
      return r;
    if (r == 1)
      return 0;
  }

  //ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[4] (%d:%d)\n",
  //            next_ack.next_expected, next_ack.highest_received));

  return this->ACE_RMCast_Module::ack (next_ack);
}

int
ACE_RMCast_Membership::join (ACE_RMCast::Join &join)
{
  if (join.source == 0)
    return 0;

  {
    //  ACE_DEBUG ((LM_DEBUG,
    //            "RMCast_Membership::join - %d\n",
    //            long(join.source)));

    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    if (this->proxies_.insert (join.source) == -1)
      return -1;
  }
  (void) this->generate_ack (join.source);

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
  }
  (void) this->generate_ack (leave.source);

  return this->ACE_RMCast_Module::leave (leave);
}

int
ACE_RMCast_Membership::generate_ack (ACE_RMCast_Proxy *proxy)
{
  ACE_RMCast::Ack next_ack;
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
    int r = this->compute_ack_i (proxy, next_ack);
    if (r < 0)
      return r;
    if (r == 1)
      return 0;
  }

  //ACE_DEBUG ((LM_DEBUG, "ACE_RMCast_Membership::ack[6] (%d:%d)\n",
  //            next_ack.next_expected, next_ack.highest_received));

  return this->ACE_RMCast_Module::ack (next_ack);
}

int
ACE_RMCast_Membership::compute_ack_i (ACE_RMCast_Proxy *source,
                                      ACE_RMCast::Ack &next_ack)
{
  Proxy_Iterator end = this->proxies_.end ();
  Proxy_Iterator i = this->proxies_.begin ();
  if (i == end)
    return 1;

  //ACE_DEBUG ((LM_DEBUG,
  //            "RMCast_Membership::generate_ack[1] - %d -> (%d:%d)\n",
  //            long(*i),
  //            (*i)->next_expected (),
  //            (*i)->highest_received ()));

  ACE_UINT32 next_expected = (*i)->next_expected ();
  ACE_UINT32 highest_received = (*i)->highest_received ();
  ++i;

  for (; i != end; ++i)
    {
      //    ACE_DEBUG ((LM_DEBUG,
      //            "RMCast_Membership::generate_ack[2] - %d -> (%d:%d)\n",
      //            long(*i),
      //            (*i)->next_expected (),
      //            (*i)->highest_received ()));

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
      ACE_DEBUG ((LM_DEBUG,
                  "RMCast_Membership::generate_ack[3]\n"));
      return 1;
    }
#endif /* 0 */
  this->next_expected_ = next_expected;
  this->highest_received_ = highest_received;
  next_ack.source = source;
  next_ack.next_expected = this->next_expected_;
  next_ack.highest_received = this->highest_received_;

  //ACE_DEBUG ((LM_DEBUG,
  //            "RMCast_Membership::generate_ack[4] - (%d:%d)\n",
  //            next_ack.next_expected,
  //            next_ack.highest_received));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Set<ACE_RMCast_Proxy*>;
template class ACE_Unbounded_Set_Iterator<ACE_RMCast_Proxy*>;
template class ACE_Node<ACE_RMCast_Proxy*>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
