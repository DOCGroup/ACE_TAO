/* -*- C++ -*- */
// $Id$

// UPIPE_Acceptor.i

ASYS_INLINE int
ACE_UPIPE_Acceptor::remove (void)
{
  ACE_TRACE ("ACE_UPIPE_Acceptor::remove");
  return this->ACE_SPIPE_Acceptor::remove ();
}
