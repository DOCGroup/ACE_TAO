/* -*- C++ -*- */
// $Id$

int
TAO_ECG_Mcast_Socket::increment (void)
{
  return TAO_Synch_Refcountable::increment ();
}

int
TAO_ECG_Mcast_Socket::refcount (void) const
{
  return TAO_Synch_Refcountable::refcount ();
}

