/* -*- C++ -*- */
// $Id$

ACE_INLINE int
TAO_ECG_Mcast_Socket::increment (void)
{
  return TAO_Synch_Refcountable::increment ();
}

ACE_INLINE int
TAO_ECG_Mcast_Socket::refcount (void) const
{
  return TAO_Synch_Refcountable::refcount ();
}

