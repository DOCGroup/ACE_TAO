// $Id$

#include "JAWS/IO_Acceptor.h"

JAWS_IO_Acceptor::JAWS_IO_Acceptor (void)
{
}

JAWS_IO_Acceptor::~JAWS_IO_Acceptor (void)
{
}

int
JAWS_IO_Acceptor::open (const ACE_INET_Addr &)
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
JAWS_IO_Acceptor::accept (size_t)
{
  return -1;
}

int
JAWS_IO_Synch_Acceptor::open (const ACE_INET_Addr &local_sap)
{
  return this->acceptor_->open (local_sap);
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


int
JAWS_IO_Asynch_Acceptor::open (const ACE_INET_Addr &address)
{
  ACE_UNUSED_ARG (address);
  return -1;
}

int
JAWS_IO_Asynch_Acceptor::accept (size_t bytes_to_read)
{
#if defined (ACE_WIN32)
  // This only works on Win32 platforms
  return this->acceptor_->accept (bytes_to_read);
#else
  ACE_UNUSED_ARG (bytes_to_read);
  return -1;
#endif /* defined (ACE_WIN32) */
}




#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<JAWS_IO_Synch_Acceptor, ACE_SYNCH_MUTEX>;
template class ACE_Singleton<JAWS_IO_Asynch_Acceptor, ACE_SYNCH_MUTEX>;
template class ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Singleton<JAWS_IO_Synch_Acceptor, ACE_SYNCH_MUTEX>
#pragma instantiate  ACE_Singleton<JAWS_IO_Asynch_Acceptor, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
