/* -*- C++ -*- */
// $Id$

// SSL_SOCK_Connector.i

#if defined (ACE_HAS_SSL)

ASYS_INLINE
ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector (void)
  : non_ssl_connect_done_ (0)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector");
}

ASYS_INLINE
ACE_SSL_SOCK_Connector::~ACE_SSL_SOCK_Connector (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::~ACE_SSL_SOCK_Connector");
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

#endif /* ACE_HAS_SSL */
