// $Id$

#include "JAWS/Data_Block.h"
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
JAWS_IO_Acceptor::open (const ACE_HANDLE &)
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

ACE_HANDLE
JAWS_IO_Acceptor::get_handle (void)
{
  return ACE_INVALID_HANDLE;
}

int
JAWS_IO_Synch_Acceptor::open (const ACE_INET_Addr &local_sap)
{
  return this->acceptor_.open (local_sap);
}

int
JAWS_IO_Synch_Acceptor::open (const ACE_HANDLE &socket)
{
  ACE_HANDLE handle = this->acceptor_.get_handle ();
  if (handle == socket)
    return 0;

  if (handle != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->acceptor_.get_handle ());
  this->acceptor_.set_handle (socket);

  return 0;
}

int
JAWS_IO_Synch_Acceptor::accept (ACE_SOCK_Stream &new_stream,
                                ACE_Addr *remote_addr,
                                ACE_Time_Value *timeout,
                                int restart,
                                int reset_new_handle) const
{
  return this->acceptor_.accept (new_stream, remote_addr, timeout,
                                 restart, reset_new_handle);
}

ACE_HANDLE
JAWS_IO_Synch_Acceptor::get_handle (void)
{
  return this->acceptor_.get_handle ();
}


int
JAWS_IO_Asynch_Acceptor::open (const ACE_INET_Addr &address)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  // Tell the acceptor to listen on this->port_, which makes an
  // asynchronous I/O request to the OS.
  return this->acceptor_.open (address, JAWS_Data_Block::JAWS_DATA_BLOCK_SIZE);
#else
  ACE_UNUSED_ARG (address);
  return -1;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
}

int
JAWS_IO_Asynch_Acceptor::open (const ACE_HANDLE &socket)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  ACE_HANDLE handle = this->acceptor_.get_handle ();
  if (handle == socket)
    return 0;

  if (handle != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->acceptor_.get_handle ());
  this->acceptor_.set_handle (socket);

  return 0;
#else
  ACE_UNUSED_ARG (socket);
  return -1;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
}

int
JAWS_IO_Asynch_Acceptor::accept (size_t bytes_to_read)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  // Do nothing, since asynchronous accepts have already been created.
  return 0;
#else
  ACE_UNUSED_ARG (bytes_to_read);
  return -1;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
}

ACE_HANDLE
JAWS_IO_Asynch_Acceptor::get_handle (void)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  return this->acceptor_.get_handle ();
#else
  return ACE_INVALID_HANDLE;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
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
