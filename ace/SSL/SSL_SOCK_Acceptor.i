/* -*- C++ -*- */
// $Id$

// SSL_SOCK_Acceptor.i

#if defined (ACE_HAS_SSL)

// Do nothing routine for constructor.

ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::ACE_SSL_SOCK_Acceptor");
}

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



int
ACE_SSL_SOCK_Acceptor::open (const ACE_Addr &local_sap,
                                int reuse_addr,
                                int protocol_family,
                                int backlog,
                                int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::open");
  return acceptor_.open (local_sap,
                         reuse_addr,
                         protocol_family,
                         backlog,
                         protocol);
}
int
ACE_SSL_SOCK_Acceptor::close (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::close ()");
  return acceptor_.close ();
}


ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor");
}


int
ACE_SSL_SOCK_Acceptor::control (int cmd, void* dummy) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::control");
  return acceptor_.control (cmd, dummy);
}

ACE_HANDLE
ACE_SSL_SOCK_Acceptor::get_handle (void) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::get_handle");
  return acceptor_.get_handle ();
}

void
ACE_SSL_SOCK_Acceptor::set_handle (ACE_HANDLE handle)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::set_handle");
  acceptor_.set_handle (handle);
}

int
ACE_SSL_SOCK_Acceptor::get_local_addr (ACE_Addr &addr) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::get_local_addr");
  return acceptor_.get_local_addr (addr);
}

void
ACE_SSL_SOCK_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::dump");
  acceptor_.dump ();
}

#endif /* ACE_HAS_SSL */
