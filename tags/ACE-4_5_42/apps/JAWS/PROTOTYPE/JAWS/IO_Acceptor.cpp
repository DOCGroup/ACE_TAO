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
JAWS_IO_Synch_Acceptor::open (const ACE_INET_Addr &local_sap, int)
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
JAWS_IO_Asynch_Acceptor::open (const ACE_INET_Addr &address, int backlog)
{
#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
  // Tell the acceptor to listen on this->port_, which makes an
  // asynchronous I/O request to the OS.

  // return this->acceptor_.open (address,
  // JAWS_Data_Block::JAWS_DATA_BLOCK_SIZE);

  // Create the listener socket
  this->handle_ = ACE_OS::socket (PF_INET, SOCK_STREAM, 0);
  if (this->handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("ACE_OS::socket")), -1);

  // Reuse the address
  int one = 1;
  if (ACE_OS::setsockopt (this->handle_,
                          SOL_SOCKET,
                          SO_REUSEADDR,
                          (const char*) &one,
                          sizeof one) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("ACE_OS::setsockopt")), -1);

  // If port is not specified, bind to any port.
  static ACE_INET_Addr sa ((const ACE_INET_Addr &) ACE_Addr::sap_any);

  if (address == sa && ACE::bind_port (this->handle_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("ACE::bind_port")), -1);

  // Bind to the specified port.
  if (ACE_OS::bind (this->handle_,
		    (sockaddr *) address.get_addr (),
		    address.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::bind"),
                      -1);

  // Start listening
  if (ACE_OS::listen (this->handle_, backlog) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::listen"), -1);

  return 0;
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
  return this->handle_;
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
