// -*- C++ -*-

#include "SSL_SOCK_Connector.h"
#include "SSL_Connect_Handler.h"

#include "ace/INET_Addr.h"
#include "ace/Synch_T.h"
#include "ace/Log_Msg.h"

#include <openssl/err.h>

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "SSL_SOCK_Connector.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID (ACE_SSL,
           SSL_SOCK_Connector,
           "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SSL_SOCK_Connector)


ACE_SSL_SOCK_Connector::~ACE_SSL_SOCK_Connector (void)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::~ACE_SSL_SOCK_Connector");
}


int
ACE_SSL_SOCK_Connector::ssl_connect (ACE_SSL_SOCK_Stream &new_stream)
{
  if (SSL_is_init_finished (new_stream.ssl ()))
    return 0;

  // Check if a connection is already pending for the given SSL
  // structure.
  if (!SSL_in_connect_init (new_stream.ssl ()))
    ::SSL_set_connect_state (new_stream.ssl ());

  int status = ::SSL_connect (new_stream.ssl ());

  if (::SSL_get_error (new_stream.ssl (), status) != SSL_ERROR_NONE)
    {
      ACE_SSL_Context::report_error ();

      return -1;
    }

  return 0;
}

int
ACE_SSL_SOCK_Connector::ssl_connect (ACE_SSL_SOCK_Stream &new_stream,
                                     const ACE_Time_Value *max_wait_time)
{
  SSL *ssl = new_stream.ssl ();

  if (SSL_is_init_finished (ssl))
    return 0;

  // Check if a connection is already pending for the given SSL
  // structure.
  if (!SSL_in_connect_init (ssl))
    ::SSL_set_connect_state (ssl);

  // Register an event handler to complete the non-blocking SSL
  // connect.  A specialized event handler is necessary since since
  // the ACE Connector strategies are not designed for protocols
  // that require additional handshakes after the initial connect.
  ACE_SSL_Connect_Handler eh (new_stream);
  ACE_Reactor_Mask reactor_mask =
    ACE_Event_Handler::READ_MASK |
    ACE_Event_Handler::WRITE_MASK;

  if (this->reactor_->register_handler (
        new_stream.get_handle (),
        &eh,
        reactor_mask) == -1)
    return -1;

  ACE_Time_Value tv;
  if (max_wait_time != 0)
    tv += (*max_wait_time);  // Make a copy.

  ACE_Time_Value *timeout = (max_wait_time == 0 ? 0 : &tv);

  // In case a thread other than the one running the Reactor event
  // loop performs the passive SSL connection establishment, transfer
  // ownership of the Reactor to the current thread.  Control will be
  // passed back to the previous owner when accepting or rejecting the
  // passive SSL connection.
  ACE_thread_t old_owner;

  if (this->reactor_->owner (ACE_Thread::self (),
                             &old_owner) != 0)
    return -1;  // Failed to transfer ownership!  Should never happen!

  // Have the Reactor complete the SSL active connection.  Run the
  // event loop until the active connection is completed.  Since
  // the Reactor is used, this isn't a busy wait.
  while (SSL_in_connect_init (ssl))
    {
      // Before blocking in the Reactor, do an SSL_connect() in case
      // OpenSSL buffered additional data sent within an SSL record
      // during session negotiation.  The buffered data must be
      // handled prior to entering the Reactor event loop since the
      // Reactor may end up waiting indefinitely for data that has
      // already arrived.
      int status = ::SSL_connect (ssl);

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
          //
          // Removing the event handler from the Reactor causes the
          // SSL stream to be shutdown.
          (void) this->reactor_->remove_handler (&eh, reactor_mask);
          (void) this->reactor_->owner (old_owner);

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

  // SSL active connection was completed.  Deregister the event
  // handler from the Reactor, but don't close it.
  (void) this->reactor_->remove_handler (&eh,
                                         reactor_mask |
                                         ACE_Event_Handler::DONT_CALL);

  // Transfer control of the Reactor to the previous owner.
  return this->reactor_->owner (old_owner);
}

int
ACE_SSL_SOCK_Connector::connect (ACE_SSL_SOCK_Stream &new_stream,
                                 const ACE_Addr &remote_sap,
                                 const ACE_Time_Value *timeout,
                                 const ACE_Addr &local_sap,
                                 int reuse_addr,
                                 int flags,
                                 int perms,
                                 int protocol_family,
                                 int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::connect");

  if (this->connector_.connect (new_stream.peer (),
                                remote_sap,
                                timeout,
                                local_sap,
                                reuse_addr,
                                flags,
                                perms,
                                protocol_family,
                                protocol) == -1)
    return -1;
  else if (new_stream.get_handle () == ACE_INVALID_HANDLE)
    new_stream.set_handle (new_stream.peer ().get_handle ());

  // Enable non-blocking, if required.
  if (timeout != 0 && new_stream.enable (ACE_NONBLOCK) == 0)
    return this->ssl_connect (new_stream, timeout);
  else
    return this->ssl_connect (new_stream);
}

int
ACE_SSL_SOCK_Connector::connect (ACE_SSL_SOCK_Stream &new_stream,
                                 const ACE_Addr &remote_sap,
                                 ACE_QoS_Params qos_params,
                                 const ACE_Time_Value *timeout,
                                 const ACE_Addr &local_sap,
                                 ACE_Protocol_Info *protocolinfo,
                                 ACE_SOCK_GROUP g,
                                 u_long flags,
                                 int reuse_addr,
                                 int perms,
                                 int protocol_family,
                                 int protocol)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::connect");

  if (this->connector_.connect (new_stream.peer (),
                                remote_sap,
                                qos_params,
                                timeout,
                                local_sap,
                                protocolinfo,
                                g,
                                flags,
                                reuse_addr,
                                perms,
                                protocol_family,
                                protocol) == -1)
    return -1;
  else if (new_stream.get_handle () == ACE_INVALID_HANDLE)
    new_stream.set_handle (new_stream.peer ().get_handle ());

  // Enable non-blocking, if required.
  if (timeout != 0 && new_stream.enable (ACE_NONBLOCK) == 0)
    return this->ssl_connect (new_stream, timeout);
  else
    return this->ssl_connect (new_stream);
}

// Try to complete a non-blocking connection.

int
ACE_SSL_SOCK_Connector::complete (ACE_SSL_SOCK_Stream &new_stream,
                                  ACE_Addr *remote_sap,
                                  const ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::complete");

  if (this->connector_.complete (new_stream.peer (),
                                 remote_sap,
                                 tv) == -1)
    return -1;
  else if (new_stream.get_handle () == ACE_INVALID_HANDLE)
    new_stream.set_handle (new_stream.peer ().get_handle ());

  return this->ssl_connect (new_stream, tv);
}


ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector (
  ACE_SSL_SOCK_Stream &new_stream,
  const ACE_Addr &remote_sap,
  const ACE_Time_Value *timeout,
  const ACE_Addr &local_sap,
  int reuse_addr,
  int flags,
  int perms,
  int protocol_family,
  int protocol,
  ACE_Reactor *reactor)
  : connector_ (),
    reactor_ (reactor)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector");
  if (this->connect (new_stream,
		     remote_sap,
		     timeout,
		     local_sap,
		     reuse_addr,
		     flags,
		     perms,
		     protocol_family,
		     protocol) == -1
      && timeout != 0
      && !(errno == EWOULDBLOCK || errno == ETIME))
    ACE_ERROR ((LM_ERROR,
		ACE_TEXT ("%p\n"),
		ACE_TEXT ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector")));
}

ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector (
  ACE_SSL_SOCK_Stream &new_stream,
  const ACE_Addr &remote_sap,
  ACE_QoS_Params qos_params,
  const ACE_Time_Value *timeout,
  const ACE_Addr &local_sap,
  ACE_Protocol_Info *protocolinfo,
  ACE_SOCK_GROUP g,
  u_long flags,
  int reuse_addr,
  int perms,
  int protocol_family,
  int protocol,
  ACE_Reactor *reactor)
  : connector_ (),
    reactor_ (reactor)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector");

  if (this->connect (new_stream,
		     remote_sap,
		     qos_params,
		     timeout,
		     local_sap,
                     protocolinfo,
                     g,
		     flags,
                     reuse_addr,
		     perms,
		     protocol_family,
		     protocol) == -1
      && timeout != 0
      && !(errno == EWOULDBLOCK || errno == ETIME))
    ACE_ERROR ((LM_ERROR,
		ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_SSL_SOCK_Connector::ACE_SSL_SOCK_Connector")));
}
