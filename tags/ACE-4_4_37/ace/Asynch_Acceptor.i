/* -*- C++ -*- */
// $Id$

// Asynch_Acceptor.i

template <class HANDLER> ACE_INLINE int
ACE_Asynch_Acceptor<HANDLER>::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}
