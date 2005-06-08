/**
 * @file Client_Pair.inl
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE
Client_Pair::Client_Pair (void)
{
}

ACE_INLINE Supplier *
Client_Pair::supplier (void) const
{
  return this->supplier_.in ();
}

ACE_INLINE Consumer *
Client_Pair::consumer (void) const
{
  return this->consumer_.in ();
}
