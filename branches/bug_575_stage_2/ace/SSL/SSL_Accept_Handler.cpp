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
  // A race condition exists where data may be sent over an SSL
  // session after the SSL passive connection is completed but before
  // this event handler is deregistered from the Reactor.
  // Specifically data meant to be handled by SSL_read() could end up
  // being handled by the SSL_accept() call below, resulting in an SSL
  // protocol error (i.e. "SSL_ERROR_SSL" error status).  The
  // following check avoids the race condition.
  if (SSL_is_init_finished (this->ssl_stream_.ssl ()))
    return 0;

  long verify_error = 0;

  // The SSL_accept() call is wrapped in a do/while(SSL_pending())
  // loop to force a full SSL record (SSL is a record-oriented
  // protocol, not a stream-oriented one) to be read prior to
  // returning to the Reactor.  This is necessary to avoid some subtle
  // problems where data from another record is potentially handled
  // before the current record is fully handled.
  do
    {
      int status = ::SSL_accept (this->ssl_stream_.ssl ());

      status = ::SSL_get_error (this->ssl_stream_.ssl (), status);
      switch (status)
        {
        case SSL_ERROR_NONE:
          verify_error =
            ::SSL_get_verify_result (this->ssl_stream_.ssl ());

          if (verify_error != X509_V_OK)
            {
#ifndef ACE_NDEBUG
              ACE_DEBUG ((LM_DEBUG,
                          "(%P|%t) X.509 certificate verification "
                          "error:%s\n",
                          ::X509_verify_cert_error_string (verify_error)));
#endif  /* ACE_NDEBUG */

              return -1;
            }

          return 0;

        case SSL_ERROR_WANT_WRITE:
        case SSL_ERROR_WANT_READ:
          break;

        default:
#ifndef ACE_NDEBUG
          ERR_print_errors_fp (stderr);
#endif  /* ACE_NDEBUG */
          return -1;
        }
    }
  while (::SSL_pending (this->ssl_stream_.ssl ()));

  return 0;
}
