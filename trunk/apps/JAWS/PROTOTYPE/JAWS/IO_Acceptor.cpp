// $Id$

#include "JAWS/Data_Block.h"
#include "JAWS/IO_Acceptor.h"

ACE_RCSID(JAWS, IO_Acceptor, "$Id$")

JAWS_IO_Acceptor::JAWS_IO_Acceptor (void)
{
}

JAWS_IO_Acceptor::~JAWS_IO_Acceptor (void)
{
}

int
JAWS_IO_Acceptor::open (const ACE_INET_Addr &, int)
{
  return -1;
}

int
JAWS_IO_Acceptor::open (const ACE_HANDLE &)
{
  return -1;
}

void
JAWS_IO_Acceptor::close (void)
{
}

int
JAWS_IO_Acceptor::accept (ACE_SOCK_Stream &, ACE_Addr *, ACE_Time_Value *,
                          int, int) const
{
  return -1;
}

int
JAWS_IO_Acceptor::accept (size_t, const void *)
{
  return -1;
}

ACE_HANDLE
JAWS_IO_Acceptor::get_handle (void)
{
  return ACE_INVALID_HANDLE;
}

int
JAWS_IO_Synch_Acceptor::open (const ACE_INET_Addr &local_sap, int backlog)
{
  return this->acceptor_.open (local_sap, 1, PF_INET, backlog);
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

int
JAWS_IO_Synch_Acceptor::accept (size_t, const void *)
{
  return -1;
}

ACE_HANDLE
JAWS_IO_Synch_Acceptor::get_handle (void)
{
  return this->acceptor_.get_handle ();
}



JAWS_IO_Asynch_Acceptor::JAWS_IO_Asynch_Acceptor (void)
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  : acceptor_ (*(new ACE_Asynch_Acceptor<JAWS_Asynch_Handler>)),
    acceptor_ptr_ (&acceptor_)
#endif
{
}

JAWS_IO_Asynch_Acceptor::~JAWS_IO_Asynch_Acceptor (void)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  delete this->acceptor_ptr_;
  this->acceptor_ptr_ = 0;
#endif
}

int
JAWS_IO_Asynch_Acceptor::open (const ACE_INET_Addr &address, int backlog)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  // Tell the acceptor to listen on this->port_, which sets up an
  // asynchronous I/O request to the OS.

  return this->acceptor_.open (address,
                               JAWS_Data_Block::JAWS_DATA_BLOCK_SIZE,
                               1,
                               backlog,
                               1,
                               0,
                               0,
                               0,
                               0);

#else
  ACE_UNUSED_ARG (address);
  return -1;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
}

int
JAWS_IO_Asynch_Acceptor::open (const ACE_HANDLE &socket)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  ACE_HANDLE handle = this->handle_;
  if (handle == socket)
    return 0;

  if (handle != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (handle);
  this->handle_ = socket;

  return 0;
#else
  ACE_UNUSED_ARG (socket);
  return -1;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
}

int
JAWS_IO_Asynch_Acceptor::accept (size_t bytes_to_read, const void *act)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  return this->acceptor_.accept (bytes_to_read, act);
#else
  ACE_UNUSED_ARG (bytes_to_read);
  return -1;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
}

int
JAWS_IO_Asynch_Acceptor::accept (ACE_SOCK_Stream &, ACE_Addr *,
                                 ACE_Time_Value *, int, int) const
{
  return -1;
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


void
JAWS_IO_Asynch_Acceptor::close (void)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  delete this->acceptor_ptr_;
  this->acceptor_ptr_ = 0;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
template class ACE_Asynch_Acceptor<JAWS_Asynch_Handler>;
template class ACE_Singleton<JAWS_IO_Asynch_Acceptor, ACE_SYNCH_MUTEX>;
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
template class ACE_Singleton<JAWS_IO_Synch_Acceptor, ACE_SYNCH_MUTEX>;
template class ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX>;
template class ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_NULL_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
#pragma instantiate ACE_Asynch_Acceptor<JAWS_Asynch_Handler>
#pragma instantiate  ACE_Singleton<JAWS_IO_Asynch_Acceptor, ACE_SYNCH_MUTEX>
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */
#pragma instantiate  ACE_Singleton<JAWS_IO_Synch_Acceptor, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_LOCK_SOCK_Acceptor<ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
