/* -*- C++ -*- */
// $Id$

// LSOCK_Dgram.i

ASYS_INLINE void
ACE_LSOCK_Dgram::set_handle (ACE_HANDLE h)
{
  ACE_TRACE ("ACE_LSOCK_Dgram::set_handle");
  this->ACE_SOCK_Dgram::set_handle (h);
  this->ACE_LSOCK::set_handle (h);
}

ASYS_INLINE ACE_HANDLE
ACE_LSOCK_Dgram::get_handle (void) const
{
  ACE_TRACE ("ACE_LSOCK_Dgram::get_handle");
  return this->ACE_SOCK_Dgram::get_handle ();
}

