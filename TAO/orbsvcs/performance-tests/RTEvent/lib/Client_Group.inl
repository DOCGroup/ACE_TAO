/**
 * @file Client_Group.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE
Client_Group::Client_Group (void)
{
}

ACE_INLINE Supplier *
Client_Group::supplier (void) const
{
  return this->supplier_.in ();
}

ACE_INLINE Consumer *
Client_Group::consumer (void) const
{
  return this->consumer_.in ();
}

ACE_INLINE Loopback_Supplier *
Client_Group::loopback_supplier (void) const
{
  return this->loopback_pair_.loopback_supplier ();
}

ACE_INLINE Loopback_Consumer *
Client_Group::loopback_consumer (void) const
{
  return this->loopback_pair_.loopback_consumer ();
}
