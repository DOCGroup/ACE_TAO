// SSL_SOCK_Acceptor.cpp
// $Id$
#define ACE_BUILD_DLL
#include "ace/SSL_SOCK_Acceptor.h"

#include "ace/Synch.h"
#include "ace/SSL.h"

#if defined (ACE_HAS_SSL)

ACE_ALLOC_HOOK_DEFINE(ACE_SSL_SOCK_Acceptor)

int
ACE_SSL_SOCK_Acceptor::shared_accept_start (ACE_Time_Value *timeout,
					    int restart,
					    int &in_blocking_mode) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::shared_accept_start");

  ACE_HANDLE handle = acceptor_.get_handle ();

  // Handle the case where we're doing a timed <accept>.

  if (timeout != 0) {
    if (ACE::handle_timed_accept (handle,
				  timeout,
				  restart) == -1)
      return(-1);

    else {

      in_blocking_mode = ACE_BIT_DISABLED (ACE::get_flags (handle),
					   ACE_NONBLOCK);

      // Set the handle into non-blocking mode if it's not already
      // in it.

      if (in_blocking_mode
	  && ACE::set_flags (handle, ACE_NONBLOCK) == -1)
	return(-1);
    }
  }

  return 0;
}

int
ACE_SSL_SOCK_Acceptor::shared_accept_finish (ACE_SSL_SOCK_Stream& new_stream,
					     int in_blocking_mode,
					     int reset_new_handle) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::shared_accept_finish ()");

  ACE_HANDLE new_handle = new_stream.get_handle ();

  // Check to see if we were originally in blocking mode, and if so,
  // set the <new_stream>'s handle and <this> handle to be in blocking
  // mode.

  if (in_blocking_mode) {

    // Save/restore errno.

    ACE_Errno_Guard error (errno);

    // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
    // originally.

    ACE::clr_flags (new_stream.get_handle (), ACE_NONBLOCK);
    ACE::clr_flags (new_handle, ACE_NONBLOCK);
  }

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  if (reset_new_handle)
    // Reset the event association inherited by the new handle.
    ::WSAEventSelect ((SOCKET) new_handle, 0, 0);
#else
  ACE_UNUSED_ARG (reset_new_handle);
#endif /* ACE_WIN32 */

  return((new_handle == ACE_INVALID_HANDLE) ? -1 : 0);
}

// General purpose routine for accepting new connections.
int
ACE_SSL_SOCK_Acceptor::accept (ACE_SSL_SOCK_Stream &new_stream,
                               ACE_Addr *remote_addr,
                               ACE_Time_Value *timeout,
                               int restart,
                               int reset_new_handle) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::accept");

  int in_blocking_mode = 1;

  if (this->shared_accept_start (timeout,
                                 restart,
                                 in_blocking_mode) == -1)
    return(-1);

  else {

    // On Win32 the third parameter to <accept> must be a NULL
    // pointer if we want to ignore the client's address.

    int *len_ptr = 0;
    sockaddr *addr = 0;
    int len = 0;

    if (remote_addr != 0) {

      len = remote_addr->get_size ();
      len_ptr = &len;
      addr = (sockaddr *) remote_addr->get_addr ();
    }

    if(timeout)
      ACE::set_flags (acceptor_.get_handle(), ACE_NONBLOCK);

    if(new_stream.get_handle() == ACE_INVALID_HANDLE) {

      do new_stream.set_handle (ACE_OS::accept (acceptor_.get_handle (),
						addr,
						len_ptr));

      while (new_stream.get_handle () == ACE_INVALID_HANDLE
	     && restart != 0
	     && errno == EINTR
	     && timeout == 0);

	if(errno == EAGAIN) return(errno);
    }

    // Reset the size of the addr, which is only necessary for UNIX
    // domain sockets.

    if (new_stream.get_handle () != ACE_INVALID_HANDLE
	&& remote_addr != 0)
      remote_addr->set_size (len);
  }

  if(!new_stream.ssl_init_finished()) {

    if(new_stream.get_SSL_fd() != new_stream.get_handle())
      new_stream.set_SSL_fd((int)new_stream.get_handle());
  }

  return(((new_stream.accept() == -1)
          && errno == EAGAIN
          && timeout == 0) ?
	 -1 :
	 this->shared_accept_finish (new_stream,
                                     in_blocking_mode,
                                     reset_new_handle));
}

int
ACE_SSL_SOCK_Acceptor::accept (ACE_SSL_SOCK_Stream &new_stream,
                               ACE_Accept_QoS_Params qos_params,
                               ACE_Addr *remote_addr,
                               ACE_Time_Value *timeout,
                               int restart,
                               int reset_new_handle) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::accept");

  int in_blocking_mode = 1;

  if (this->shared_accept_start (timeout,
				 restart,
				 in_blocking_mode) == -1)
    return(-1);

  else {

    // On Win32 the third parameter to <accept> must be a NULL
    // pointer if we want to ignore the client's address.

    int *len_ptr = 0;
    sockaddr *addr = 0;
    int len = 0;

    if (remote_addr != 0) {

      len = remote_addr->get_size ();

      len_ptr = &len;

      addr = (sockaddr *) remote_addr->get_addr ();
    }

    if(new_stream.get_handle() == ACE_INVALID_HANDLE) {

      do new_stream.set_handle (ACE_OS::accept (acceptor_.get_handle (),
						addr,
						len_ptr,
						qos_params));
      while (new_stream.get_handle () == ACE_INVALID_HANDLE
	     && restart != 0
	     && errno == EINTR
	     && timeout == 0);
    }

    // Reset the size of the addr, which is only necessary for UNIX
    // domain sockets.

    if (new_stream.get_handle () != ACE_INVALID_HANDLE
	&& remote_addr != 0)
      remote_addr->set_size (len);
  }

  if(!new_stream.ssl_init_finished()) {

    if(new_stream.get_SSL_fd() != new_stream.get_handle())
      new_stream.set_SSL_fd((int)new_stream.get_handle());
  }

  return(((new_stream.accept() == -1)
	  && errno == EAGAIN
	  && timeout == 0) ?
	 -1 :
	 this->shared_accept_finish (new_stream,
				     in_blocking_mode,
				     reset_new_handle));
}

int
ACE_SSL_SOCK_Acceptor::enable (int value) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::enable");
  switch (value)
    {
    case SIGURG:
    case ACE_SIGURG:
    case SIGIO:
    case ACE_SIGIO:
    case ACE_CLOEXEC:
      ACE_NOTSUP_RETURN (-1);
    case ACE_NONBLOCK:
      return acceptor_.enable (value);
    default:
      return -1;
    }
  return 0;
}
int
ACE_SSL_SOCK_Acceptor::disable (int value) const
{
  ACE_TRACE("ACE_SSL_SOCK_Acceptor::disable");
  switch (value)
    {
    case SIGURG:
    case ACE_SIGURG:
    case SIGIO:
    case ACE_SIGIO:
    case ACE_CLOEXEC:
      ACE_NOTSUP_RETURN (-1);
    case ACE_NONBLOCK:
      return acceptor_.disable (value);
    default:
      return -1;
    }
  return 0;
}

#endif /* ACE_HAS_SSL */
