// -*- C++ -*-

#include "SSL_Connect_Handler.h"
#include "SSL_SOCK_Stream.h"

#include <openssl/err.h>

ACE_RCSID (ACE_SSL,
           SSL_Connect_Handler,
           "$Id$")


ACE_SSL_Connect_Handler::ACE_SSL_Connect_Handler (ACE_SSL_SOCK_Stream &s)
  : ssl_stream_ (s)
{
}

ACE_SSL_Connect_Handler::~ACE_SSL_Connect_Handler (void)
{
}

ACE_HANDLE
ACE_SSL_Connect_Handler::get_handle (void) const
{
  return this->ssl_stream_.get_handle ();
}

int
ACE_SSL_Connect_Handler::handle_input (ACE_HANDLE)
{
  return this->ssl_connect ();
}

int
ACE_SSL_Connect_Handler::handle_output (ACE_HANDLE)
{
  return this->ssl_connect ();
}

int
ACE_SSL_Connect_Handler::handle_close (ACE_HANDLE /* handle */,
                                       ACE_Reactor_Mask /* close_mask */)
{
  return this->ssl_stream_.close ();
}


int
ACE_SSL_Connect_Handler::ssl_connect (void)
{
  SSL *ssl = this->ssl_stream_.ssl ();

  // A race condition exists where data may be sent over an SSL
  // session after the SSL active connection is completed but before
  // this event handler is deregistered from the Reactor.
  // Specifically data meant to be handled by SSL_read() could end up
  // being handled by the SSL_connect() call below, resulting in an SSL
  // protocol error (i.e. "SSL_ERROR_SSL" error status).  The
  // following check avoids the race condition.
  if (SSL_is_init_finished (ssl))
    return 0;

  int status = ::SSL_connect (ssl);

  switch (::SSL_get_error (ssl, status))
    {
    case SSL_ERROR_NONE:
      // Start out with non-blocking disabled on the SSL stream.
      this->ssl_stream_.disable (ACE_NONBLOCK);

      // Active connection completed.
      break;

    case SSL_ERROR_WANT_WRITE:
    case SSL_ERROR_WANT_READ:
      // If data is still buffered within OpenSSL's internal buffer,
      // then force the Reactor to invoke the SSL connect event handler
      // (with the appropriate mask) before waiting for more events
      // (e.g. blocking on select()).  All pending data must be
      // processed before waiting for more events to come in on the
      // SSL handle.
      if (::SSL_pending (ssl))
        return 1;

      break;

    case SSL_ERROR_ZERO_RETURN:
      // The peer has notified us that it is shutting down via
      // the SSL "close_notify" message so we need to
      // shutdown, too.
      //
      // Removing this event handler causes the SSL stream to be
      // shutdown.
      return -1;

    case SSL_ERROR_SYSCALL:
      // On some platforms (e.g. MS Windows) OpenSSL does not
      // store the last error in errno so explicitly do so.
      //
      // Explicitly check for EWOULDBLOCK since it doesn't get
      // converted to an SSL_ERROR_WANT_{READ,WRITE} on some
      // platforms, such as AIX.
      if (ACE_OS::set_errno_to_last_error () == EWOULDBLOCK)
        return 0;

    default:
      ACE_SSL_Context::report_error ();

      return -1;
    }

  return 0;
}
