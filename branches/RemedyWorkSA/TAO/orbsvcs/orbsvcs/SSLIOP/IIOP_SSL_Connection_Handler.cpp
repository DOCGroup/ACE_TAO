// $Id$

#include "orbsvcs/SSLIOP/IIOP_SSL_Connection_Handler.h"
#include "orbsvcs/SSLIOP/IIOP_SSL_Transport.h"
#include "orbsvcs/SSLIOP/SSLIOP_Connection_Handler.h"
#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/SSLIOP/IIOP_SSL_Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_ENABLE_TIMEPROBES)

static const char * const TAO_IIOP_SSL_Connect_Timeprobe_Description[] =
{
  "IIOP_SSL_Connection_Handler::handle_input - start",
  "IIOP_SSL_Connection_Handler::handle_input - end",

  "IIOP_SSL_Connection_Handler::handle_locate - start",
  "IIOP_SSL_Connection_Handler::handle_locate - end",

  "IIOP_SSL_Connection_Handler::receive_request - end",

  "IIOP_SSL_Connection_Handler::send_request - start",
  "IIOP_SSL_Connection_Handler::send_request - end"};

enum
{
  // Timeprobe description table start key
  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_INPUT_START = 320,
  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_INPUT_END,

  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_LOCATE_START,
  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_LOCATE_END,

  TAO_IIOP_SSL_CONNECTION_HANDLER_RECEIVE_REQUEST_END

};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_IIOP_SSL_Connect_Timeprobe_Description,
                                  TAO_IIOP_SSL_CONNECTION_HANDLER_HANDLE_INPUT_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::IIOP_SSL_Connection_Handler::IIOP_SSL_Connection_Handler (
  ACE_Thread_Manager *t)
  : TAO_IIOP_Connection_Handler (t)
{
  // This constructor should *never* get called, it is just here to
  // make the compiler happy: the default implementation of the
  // Creation_Strategy requires a constructor with that signature, we
  // don't use that implementation, but some (most?) compilers
  // instantiate it anyway.
  ACE_ASSERT (0);
}


TAO::IIOP_SSL_Connection_Handler::IIOP_SSL_Connection_Handler (
  TAO_ORB_Core *orb_core)
  : TAO_IIOP_Connection_Handler (orb_core)
{
  // Delete the transport with TAO_IIOP_Connection_Handler.
  delete this->transport ();

  IIOP_SSL_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
           IIOP_SSL_Transport (this,
                               orb_core));

  // store this pointer
  this->transport (specific_transport);
}

TAO::IIOP_SSL_Connection_Handler::~IIOP_SSL_Connection_Handler (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
