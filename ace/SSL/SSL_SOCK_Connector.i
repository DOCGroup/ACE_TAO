/* -*- C++ -*- */
// $Id$

// SSL_SOCK_Connector.i

ASYS_INLINE
ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector (ACE_Reactor *reactor)
  : connector_ (),
    reactor_ (reactor)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector");
}

ASYS_INLINE int
ACE_SSL_SOCK_Connector::reset_new_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::reset_new_handle");
  return this->connector_.reset_new_handle (handle);
}

ASYS_INLINE void
ACE_SSL_SOCK_Connector::dump (void) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::dump");
}

ASYS_INLINE void
ACE_SSL_SOCK_Connector::reactor (ACE_Reactor *r)
{
  // NOTE: Be careful not to change the Reactor in the middle of a
  //       non-blocking connect().

  this->reactor_ = r;
}

ASYS_INLINE ACE_Reactor *
ACE_SSL_SOCK_Connector::reactor (void) const
{
  return this->reactor_;
}
