/* -*- C++ -*- */
// $Id$

// Multihomed_INET_Addr.i

// Accessor implementations
ACE_INLINE
size_t
ACE_Multihomed_INET_Addr::get_num_secondary_addresses() const
{
  return this->secondaries.size();
}
