// -*- C++ -*-
// $Id$

#include "ace/SSL/SSL_SOCK_Connector.h"

#include "ace/OS/Handle_Set.h"
#include "ace/IPC/INET_Addr.h"
#include "ace/Threads/Synch_T.h"

#ifdef ACE_SUBSET_0
#include "ace/Log_Msg.h"
#endif

#include <openssl/err.h>

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SSL/SSL_SOCK_Connector.i"
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
ACE_SSL_SOCK_Connector::ssl_connect (ACE_SSL_SOCK_Stream &new_stream,
                                     const ACE_Time_Value *timeout)
{
  SSL *ssl = new_stream.ssl ();

  if (SSL_is_init_finished (ssl))
    return 0;

  // Check if a connection is already pending for the given SSL
  // structure.
  if (!SSL_in_connect_init (ssl))
    ::SSL_set_connect_state (ssl);

  ACE_HANDLE handle = new_stream.get_handle ();

  // We're going to call SSL_connect, optionally doing ACE::select and
  // retrying the SSL_connect, until the SSL handshake is done or
  // it fails.
  // To get the timeout affect, set the socket to nonblocking mode
  // before beginning if there is a timeout specified. If the timeout
  // is 0 (wait as long as it takes) then don't worry about the blocking
  // status; we'll block in SSL_connect if the socket is blocking, and
  // block in ACE::select if not.
  int reset_blocking_mode = 0;
  if (timeout != 0)
    {
      reset_blocking_mode = ACE_BIT_DISABLED (ACE::get_flags (handle),
                                              ACE_NONBLOCK);
          // Set the handle into non-blocking mode if it's not already
          // in it.
          if (reset_blocking_mode
              && ACE::set_flags (handle,
                                 ACE_NONBLOCK) == -1)
            return -1;
    }

  int status;
  do
    {
      // These handle sets are used to set up for whatever SSL_accept
      // says it wants next. They're reset on each pass around the loop.
      ACE_Handle_Set rd_handle;
      ACE_Handle_Set wr_handle;

      status = ::SSL_connect (ssl);
      switch (::SSL_get_error (ssl, status))
        {
        case SSL_ERROR_NONE:
          // Start out with non-blocking disabled on the SSL stream.
          new_stream.disable (ACE_NONBLOCK);
          status = 0;               // To tell caller about success
          break;                    // Done

        case SSL_ERROR_WANT_WRITE:
          wr_handle.set_bit (handle);
          status = 1;               // Wait for more activity
          break;

        case SSL_ERROR_WANT_READ:
          rd_handle.set_bit (handle);
          status = 1;               // Wait for more activity
          break;

        case SSL_ERROR_ZERO_RETURN:
          // The peer has notified us that it is shutting down via
          // the SSL "close_notify" message so we need to
          // shutdown, too.
          status = -1;
          break;

        case SSL_ERROR_SYSCALL:
          // On some platforms (e.g. MS Windows) OpenSSL does not
          // store the last error in errno so explicitly do so.
          //
          // Explicitly check for EWOULDBLOCK since it doesn't get
          // converted to an SSL_ERROR_WANT_{READ,WRITE} on some
          // platforms. If SSL_accept failed outright, though, don't
          // bother checking more. This can happen if the socket gets
          // closed during the handshake.
          if (ACE_OS::set_errno_to_last_error () == EWOULDBLOCK &&
              status == -1)
            {
              // Although the SSL_ERROR_WANT_READ/WRITE isn't getting
              // set correctly, the read/write state should be valid.
              // Use that to decide what to do.
              status = 1;               // Wait for more activity
              if (SSL_want_write (ssl))
                wr_handle.set_bit (handle);
              else if (SSL_want_read (ssl))
                rd_handle.set_bit (handle);
              else
                status = -1;            // Doesn't want anything - bail out
            }
          else
            status = -1;
          break;

        default:
          ACE_SSL_Context::report_error ();
          status = -1;
          break;
        }

      if (status == 1)
        {
          // Must have at least one handle to wait for at this point.
          ACE_ASSERT (rd_handle.num_set() == 1 || wr_handle.num_set () == 1);
          status = ACE::select (int (handle) + 1,
                                &rd_handle,
                                &wr_handle,
                                0,
                                timeout);
          // 0 is timeout, so we're done.
          // -1 is error, so we're done.
          // Could be both handles set (same handle in both masks) so set to 1.
          if (status >= 1)
            status = 1;
          else                 // Timeout or socket failure
            status = -1;
        }

    } while (status == 1 && !SSL_is_init_finished (ssl));

  if (reset_blocking_mode)
    {
      ACE_Errno_Guard eguard (errno);
      ACE::clr_flags (handle, ACE_NONBLOCK);
    }

  return (status == -1 ? -1 : 0);

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

  // Take into account the time to complete the basic TCP handshake
  // and the SSL handshake.
  ACE_Time_Value timeout_copy (*timeout);  // Need a scribblable copy
  ACE_Countdown_Time countdown (&timeout_copy);

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

  (void) countdown.update ();

  return this->ssl_connect (new_stream, &timeout_copy);
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

  // Take into account the time to complete the basic TCP handshake
  // and the SSL handshake.
  ACE_Time_Value timeout_copy (*timeout);  // Need a scribblable copy
  ACE_Countdown_Time countdown (&timeout_copy);

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

  (void) countdown.update ();

  return this->ssl_connect (new_stream, &timeout_copy);
}

// Try to complete a non-blocking connection.

int
ACE_SSL_SOCK_Connector::complete (ACE_SSL_SOCK_Stream &new_stream,
                                  ACE_Addr *remote_sap,
                                  const ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_SSL_SOCK_Connector::complete");

  // Take into account the time to complete the basic TCP handshake
  // and the SSL handshake.
  ACE_Time_Value timeout_copy (*tv);  // Need a scribblable copy
  ACE_Countdown_Time countdown (&timeout_copy);

  if (this->connector_.complete (new_stream.peer (),
                                 remote_sap,
                                 tv) == -1)
    return -1;
  else if (new_stream.get_handle () == ACE_INVALID_HANDLE)
    new_stream.set_handle (new_stream.peer ().get_handle ());

  (void) countdown.update ();

  return this->ssl_connect (new_stream, &timeout_copy);
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
  int protocol)
  : connector_ ()
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
  int protocol)
  : connector_ ()
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
