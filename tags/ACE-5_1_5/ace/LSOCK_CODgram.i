/* -*- C++ -*- */
// $Id$

// LSOCK_CODgram.i

// Do nothing constructor. 

ASYS_INLINE
ACE_LSOCK_CODgram::ACE_LSOCK_CODgram (void)
{
  ACE_TRACE ("ACE_LSOCK_CODgram::ACE_LSOCK_CODgram");
}

ASYS_INLINE void
ACE_LSOCK_CODgram::set_handle (ACE_HANDLE h)
{
  ACE_TRACE ("ACE_LSOCK_CODgram::set_handle");
  this->ACE_SOCK_CODgram::set_handle (h);
  this->ACE_LSOCK::set_handle (h);
}

ASYS_INLINE ACE_HANDLE
ACE_LSOCK_CODgram::get_handle (void) const
{
  ACE_TRACE ("ACE_LSOCK_CODgram::get_handle");
  return this->ACE_SOCK_CODgram::get_handle ();
}
