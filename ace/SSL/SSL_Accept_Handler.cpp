// -*- C++ -*-

#include "SSL_Accept_Handler.h"
#include "SSL_SOCK_Stream.h"

#include <openssl/err.h>

ACE_RCSID (ACE_SSL,
           SSL_Accept_Handler,
           "$Id$")


ACE_SSL_Accept_Handler::ACE_SSL_Accept_Handler (ACE_SSL_SOCK_Stream &s)
  : ssl_stream_ (s)
{
}

ACE_SSL_Accept_Handler::~ACE_SSL_Accept_Handler (void)
{
}

ACE_HANDLE
ACE_SSL_Accept_Handler::get_handle (void) const
{
  return this->ssl_stream_.get_handle ();
}

int
ACE_SSL_Accept_Handler::handle_input (ACE_HANDLE)
{
  return this->ssl_accept ();
}

int
ACE_SSL_Accept_Handler::handle_output (ACE_HANDLE)
{
  return this->ssl_accept ();
}

int
ACE_SSL_Accept_Handler::handle_close (ACE_HANDLE /* handle */,
                                      ACE_Reactor_Mask /* close_mask */)
{
  return this->ssl_stream_.close ();
}


int
ACE_SSL_Accept_Handler::ssl_accept (void)
{
  SSL *ssl = this->ssl_stream_.ssl ();

  // A race condition exists where data may be sent over an SSL
  // session after the SSL passive connection is completed but before
  // this event handler is deregistered from the Reactor.
  // Specifically data meant to be handled by SSL_read() could end up
  // being handled by the SSL_accept() call below, resulting in an SSL
  // protocol error (i.e. "SSL_ERROR_SSL" error status).  The
  // following check avoids the race condition.
  if (SSL_is_init_finished (ssl))
    return 0;

  int status = ::SSL_accept (ssl);

  switch (::SSL_get_error (ssl, status))
    {
    case SSL_ERROR_NONE:
      break;

    case SSL_ERROR_WANT_WRITE:
    case SSL_ERROR_WANT_READ:
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
      ACE_OS::set_errno_to_last_error ();

    default:
      ACE_SSL_Context::report_error ();

      return -1;
    }

  return 0;
}
