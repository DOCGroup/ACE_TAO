#include "SSLIOP_Accept_Strategy.h"


ACE_RCSID (SSLIOP,
           SSLIOP_Accept_Strategy,
           "$Id$")


TAO_SSLIOP_Accept_Strategy::TAO_SSLIOP_Accept_Strategy (
  TAO_ORB_Core * orb_core,
  const ACE_Time_Value & timeout)
  : TAO_Accept_Strategy<TAO_SSLIOP_Connection_Handler,
                        ACE_SSL_SOCK_ACCEPTOR> (orb_core),
    timeout_ (timeout)
{
}

int
TAO_SSLIOP_Accept_Strategy::accept_svc_handler (handler_type * svc_handler)
{
  ACE_TRACE ("TAO_SSLIOP_Accept_Strategy::accept_svc_handler");

  // The following code is basically the same code found in
  // ACE_Accept_Strategy::accept_svc_handler().  The only difference
  // is that a timeout value is passed to the peer acceptor's accept()
  // method.  A timeout is necessary to prevent malicious or
  // misbehaved clients from only completing the TCP handshake and not
  // the SSL handshake.  Without the timeout, a denial-of-service
  // vulnerability would exist where multiple incomplete SSL passive
  // connections (i.e. where only the TCP handshake is completed)
  // could result in the server process running out of file
  // descriptors.  That would be due to the SSL handshaking process
  // blocking/waiting for the handshake to complete.

  // The timeout value will be modified.  Make a copy.
  ACE_Time_Value timeout (this->timeout_);

  // Try to find out if the implementation of the reactor that we are
  // using requires us to reset the event association for the newly
  // created handle. This is because the newly created handle will
  // inherit the properties of the listen handle, including its event
  // associations.
  int reset_new_handle = this->reactor_->uses_event_associations ();

  if (this->peer_acceptor_.accept (svc_handler->peer (), // stream
                                   0,                // remote address
                                   &timeout,         // timeout
                                   1,                // restart
                                   reset_new_handle  // reset new handler
                                   ) == -1)
    {
      // Close down handler to avoid memory leaks.
      svc_handler->close (0);

      return -1;
    }
  else
    return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;
template class TAO_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

# pragma instantiate ACE_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>
# pragma instantiate TAO_Accept_Strategy<TAO_SSLIOP_Connection_Handler, ACE_SSL_SOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
