/**
 * @file Loopback_Pair.inl
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE
Loopback_Pair::Loopback_Pair ()
{
}

ACE_INLINE Loopback_Supplier *
Loopback_Pair::loopback_supplier () const
{
  return this->loopback_supplier_.in ();
}

ACE_INLINE Loopback_Consumer *
Loopback_Pair::loopback_consumer () const
{
  return this->loopback_consumer_.in ();
}
