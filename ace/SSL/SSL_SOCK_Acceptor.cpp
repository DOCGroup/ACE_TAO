// -*- C++ -*-
//
// $Id$


#include "SSL_SOCK_Acceptor.h"
#include "SSL_Accept_Handler.h"

#include "ace/Log_Msg.h"

ACE_ALLOC_HOOK_DEFINE(ACE_SSL_SOCK_Acceptor)

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Acceptor.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID (ACE_SSL,
           SSL_SOCK_Acceptor,
           "$Id$")

ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::~ACE_SSL_SOCK_Acceptor");
}

int
ACE_SSL_SOCK_Acceptor::shared_accept_start (ACE_Time_Value *timeout,
					    int restart,
					    int &in_blocking_mode) const
{
  ACE_TRACE ("ACE_SSL_SOCK_Acceptor::shared_accept_start");

  ACE_HANDLE handle = this->get_handle ();

  // Handle the case where we're doing a timed <accept>.
  if (timeout != 0)
    {
      if (ACE::handle_timed_accept (handle,
				    timeout,
				    restart) == -1)
	return -1;
      else
	{
	  in_blocking_mode = ACE_BIT_DISABLED (ACE::get_flags (handle),
					       ACE_NONBLOCK);
	  // Set the handle into non-blocking mode if it's not already
	  // in it.
	  if (in_blocking_mode
	      && ACE::set_flags (handle,
				 ACE_NONBLOCK) == -1)
	    return -1;
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
  if (in_blocking_mode)
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);

      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (this->get_handle (),
                      ACE_NONBLOCK);
      ACE::clr_flags (new_handle,
                      ACE_NONBLOCK);
    }

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  if (reset_new_handle)
    // Reset the event association inherited by the new handle.
    ::WSAEventSelect ((SOCKET) new_handle, 0, 0);
#else
  ACE_UNUSED_ARG (reset_new_handle);
#endif /* ACE_WIN32 */

  if (new_handle == ACE_INVALID_HANDLE)
    return -1;

  return 0;
}

int
ACE_SSL_SOCK_Acceptor::ssl_accept (ACE_SSL_SOCK_Stream &new_stream) const
{
  if (SSL_is_init_finished (new_stream.ssl ()))
    return 0;

  if (!SSL_in_accept_init (new_stream.ssl ()))
    ::SSL_set_accept_state (new_stream.ssl ());

  int status = ::SSL_accept (new_stream.ssl ());

  switch (::SSL_get_error (new_stream.ssl (), status))
    {
    case SSL_ERROR_NONE:
      break;
    case SSL_ERROR_ZERO_RETURN:
      // The peer has notified us that it is shutting down via
      // the SSL "close_notify" message so we need to
      // shutdown, too.
      (void) new_stream.close ();
      return -1;
    default:
      ACE_SSL_Context::report_error ();

      return -1;
    }

  return 0;
}

int
ACE_SSL_SOCK_Acceptor::ssl_accept (ACE_SSL_SOCK_Stream &new_stream,
                                   ACE_Time_Value *timeout) const
{
  SSL *ssl = new_stream.ssl ();

  if (SSL_is_init_finished (ssl))
    return 0;

  if (!SSL_in_accept_init (ssl))
    ::SSL_set_accept_state (ssl);

  // Register an event handler to complete the non-blocking SSL
  // accept.  A specialized event handler is necessary since since
  // the ACE Acceptor strategies are not designed for protocols
  // that require additional handshakes after the initial accept.
  ACE_SSL_Accept_Handler eh (new_stream);
  ACE_Reactor_Mask reactor_mask =
    ACE_Event_Handler::READ_MASK |
    ACE_Event_Handler::WRITE_MASK;

  // In case a thread other than the one running the Reactor event
  // loop
  // performs the passive SSL connection establishment, transfer
  // ownership of the Reactor to the current thread.  Control will be
  // passed back to the previous owner when accepting or rejecting the
  // passive SSL connection.
  ACE_thread_t old_owner;

  if (this->reactor_->owner (ACE_Thread::self (),
                             &old_owner) != 0)
    return -1;  // Failed to transfer ownership!  Should never happen!

  if (this->reactor_->register_handler (
        new_stream.get_handle (),
        &eh,
        reactor_mask) == -1)
    return -1;

  // Have the Reactor complete the SSL passive connection.  Run the
  // event loop until the passive connection is completed.  Since
  // the Reactor is used, this isn't a busy wait.
  while (SSL_in_accept_init (ssl))
    {
      // Before blocking in the Reactor, do an SSL_accept() in case
      // OpenSSL buffered additional data sent within an SSL record
      // during session negotiation.  The buffered data must be
      // handled prior to entering the Reactor event loop since the
      // Reactor may end up waiting indefinitely for data that has
      // already arrived.
      int status = ::SSL_accept (ssl);

      switch (::SSL_get_error (ssl, status))
        {
        case SSL_ERROR_NONE:
          break;

        case SSL_ERROR_WANT_WRITE:
        case SSL_ERROR_WANT_READ:
          // No data buffered by OpenSSL, so wait for data in the
          // Reactor.
          if (this->reactor_->handle_events (timeout) == -1
              || new_stream.get_handle () == ACE_INVALID_HANDLE)
            {
              (void) this->reactor_->remove_handler (&eh, reactor_mask);
              (void) this->reactor_->owner (old_owner);
              return -1;
            }

          break;

        case SSL_ERROR_ZERO_RETURN:
          // The peer has notified us that it is shutting down via
          // the SSL "close_notify" message so we need to
          // shutdown, too.
          (void) new_stream.close ();

          return -1;

        default:

#ifndef ACE_NDEBUG
          //ERR_print_errors_fp (stderr);
#endif  /* ACE_NDEBUG */

          (void) this->reactor_->remove_handler (&eh, reactor_mask);
          (void) this->reactor_->owner (old_owner);

          return -1;
        }

    }

  // SSL passive connection was completed.  Deregister the event
  // handler from the Reactor, but don't close it.
  (void) this->reactor_->remove_handler (&eh,
                                         reactor_mask |
                                         ACE_Event_Handler::DONT_CALL);


  ACE_DEBUG ((LM_DEBUG, "(%t) SSL accept completed.\n"));

  // Transfer control of the Reactor to the previous owner.
  return this->reactor_->owner (old_owner);
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

  int in_blocking_mode = 0;
  if (this->shared_accept_start (timeout,
                                 restart,
                                 in_blocking_mode) == -1)
    return -1;
  else
    {
      // On Win32 the third parameter to <accept> must be a NULL
      // pointer if we want to ignore the client's address.
      int *len_ptr = 0;
      sockaddr *addr = 0;
      int len = 0;

      if (remote_addr != 0)
        {
          len = remote_addr->get_size ();
          len_ptr = &len;
          addr = (sockaddr *) remote_addr->get_addr ();
        }

      ACE_HANDLE handle;
      do
        handle = ACE_OS::accept (this->get_handle (),
                                 addr,
                                 len_ptr);
      while (handle == ACE_INVALID_HANDLE
             && restart != 0
             && errno == EINTR
             && timeout == 0);

      // Reset the size of the addr, which is only necessary for UNIX
      // domain sockets.
      if (handle != ACE_INVALID_HANDLE
          && remote_addr != 0)
        remote_addr->set_size (len);

      new_stream.set_handle (handle);
    }

  if (this->shared_accept_finish (new_stream,
                                  in_blocking_mode,
                                  reset_new_handle) != 0)
    return -1;

  if (in_blocking_mode)
    return this->ssl_accept (new_stream);

  return this->ssl_accept (new_stream, timeout);
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

  int in_blocking_mode = 0;
  if (this->shared_accept_start (timeout,
				 restart,
				 in_blocking_mode) == -1)
    return -1;
  else
    {
      // On Win32 the third parameter to <accept> must be a NULL
      // pointer if we want to ignore the client's address.
      int *len_ptr = 0;
      sockaddr *addr = 0;
      int len = 0;

      if (remote_addr != 0)
        {
          len = remote_addr->get_size ();
          len_ptr = &len;
          addr = (sockaddr *) remote_addr->get_addr ();
        }

      ACE_HANDLE handle;
      do
        handle = ACE_OS::accept (this->get_handle (),
                                 addr,
                                 len_ptr,
                                 qos_params);
      while (handle == ACE_INVALID_HANDLE
             && restart != 0
             && errno == EINTR
             && timeout == 0);

      // Reset the size of the addr, which is only necessary for UNIX
      // domain sockets.
      if (handle != ACE_INVALID_HANDLE
          && remote_addr != 0)
        remote_addr->set_size (len);

      new_stream.set_handle (handle);
    }

  if (this->shared_accept_finish (new_stream,
                                  in_blocking_mode,
                                  reset_new_handle) != 0)
    return -1;

  if (in_blocking_mode)
    return this->ssl_accept (new_stream);

  return this->ssl_accept (new_stream, timeout);
}
