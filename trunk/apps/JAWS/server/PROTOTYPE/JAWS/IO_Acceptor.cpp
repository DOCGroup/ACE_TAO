// $Id$

#include "JAWS/IO_Acceptor.h"

int
JAWS_IO_Acceptor::open (const ACE_Addr &, int, int, int, int)
{
  return -1;
}

int
JAWS_IO_Acceptor::accept (ACE_SOCK_Stream &, ACE_Addr *, ACE_Time_Value *,
                          int, int) const
{
  return -1;
}

int
JAWS_IO_Acceptor::open (const ACE_INET_Addr &, size_t, int, int, int,
                        ACE_Proactor *)
{
  return -1;
}

int
JAWS_IO_Acceptor::accept (size_t)
{
  return -1;
}

int
JAWS_IO_Synch_Acceptor::open (const ACE_Addr &local_sap,
                              int reuse_addr,
                              int protocol_family,
                              int backlog,
                              int protocol)
{
  return this->acceptor_->open (local_sap, reuse_addr, protocol_family,
                                backlog, protocol);
}

int
JAWS_IO_Synch_Acceptor::accept (ACE_SOCK_Stream &new_stream,
                                ACE_Addr *remote_addr,
                                ACE_Time_Value *timeout,
                                int restart,
                                int reset_new_handle) const
{
  return this->acceptor_->accept (new_stream, remote_addr, timeout,
                                  restart, reset_new_handle);
}


#if defined (ACE_WIN32)
// This only works on Win32 platforms

int
JAWS_IO_Asynch_Acceptor::open (const ACE_INET_Addr &address,
                               size_t bytes_to_read,
                               int pass_addresses,
                               int backlog,
                               int reuse_addr,
                               ACE_Proactor *proactor)
{
  return this->acceptor_->open (address, bytes_to_read, pass_address,
                                backlog, reuse_addr, proactor);
}

int
JAWS_IO_Asynch_Acceptor::accept (size_t bytes_to_read)
{
  return this->acceptor_->accept (bytes_to_read);
}

#endif /* defined (ACE_WIN32) */
