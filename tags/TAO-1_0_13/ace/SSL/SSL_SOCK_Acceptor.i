// -*- C++ -*-
// $Id$

// SSL_SOCK_Acceptor.i

#if defined (ACE_HAS_SSL)

// Do nothing routine for constructor.

ACE_INLINE
ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor");
}

ACE_INLINE
ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
                                              int reuse_addr,
                                              int protocol_family,
                                              int backlog,
                                              int protocol)
  : acceptor_ (local_sap, reuse_addr, protocol_family, backlog, protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor");
}
// Performs the timed accept operation.

ACE_INLINE
ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor (const ACE_Addr &local_sap,
                                              ACE_Protocol_Info *protocolinfo,
                                              ACE_SOCK_GROUP g,
                                              u_long flags,
                                              int reuse_addr,
                                              int protocol_family,
                                              int backlog = ACE_DEFAULT_BACKLOG,
                                              int protocol = 0)
  : acceptor_ (local_sap,
               protocolinfo,
               g,
               flags,
               reuse_addr,
               protocol_family,
               backlog,
               protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor");
}

ACE_INLINE int
ACE_SSL_SOCK_Acceptor::open (const ACE_Addr &local_sap,
                             int reuse_addr,
                             int protocol_family,
                             int backlog,
                             int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::open");
  return this->acceptor_.open (local_sap,
                               reuse_addr,
                               protocol_family,
                               backlog,
                               protocol);
}

ACE_INLINE int
ACE_SSL_SOCK_Acceptor::close (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::close ()");
  return this->acceptor_.close ();
}


ACE_INLINE
ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor");
}

ACE_INLINE int
ACE_SSL_SOCK_Acceptor::control (int cmd, void* dummy) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::control");
  return this->acceptor_.control (cmd, dummy);
}

ACE_INLINE ACE_HANDLE
ACE_SSL_SOCK_Acceptor::get_handle (void) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::get_handle");
  return this->acceptor_.get_handle ();
}

ACE_INLINE void
ACE_SSL_SOCK_Acceptor::set_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::set_handle");
  this->acceptor_.set_handle (handle);
}

ACE_INLINE int
ACE_SSL_SOCK_Acceptor::get_local_addr (ACE_Addr &addr) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::get_local_addr");
  return this->acceptor_.get_local_addr (addr);
}

ACE_INLINE void
ACE_SSL_SOCK_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::dump");
  this->acceptor_.dump ();
}

#endif /* ACE_HAS_SSL */
