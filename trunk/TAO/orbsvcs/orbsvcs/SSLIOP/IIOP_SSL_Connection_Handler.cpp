#include "IIOP_SSL_Connection_Handler.h"
#include "IIOP_SSL_Transport.h"
#include "SSLIOP_Connection_Handler.h"
#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/debug.h"


ACE_RCSID (SSLIOP,
           IIOP_SSL_Connection_Handler,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "IIOP_SSL_Connection_Handler.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_IIOP_SSL_Connect_Timeprobe_Description[] =
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
  TAO_ORB_Core *orb_core,
  CORBA::Boolean /*flag*/,
  void *arg)
  : TAO_IIOP_Connection_Handler (
      orb_core,
      (static_cast<TAO::SSLIOP::Connection_Handler_State *> (arg))->tcp_properties)
{
  TAO::SSLIOP::Connection_Handler_State *s =
  static_cast<TAO::SSLIOP::Connection_Handler_State *> (arg);

  IIOP_SSL_Transport* specific_transport = 0;
  ACE_NEW (specific_transport,
           IIOP_SSL_Transport (this,
                               orb_core,
                               s->ssliop_current.in (),
                               0));

  // Delete the transport with TAO_IIOP_Connection_Handler.
  delete this->transport ();

  // store this pointer
  this->transport (specific_transport);
}

TAO::IIOP_SSL_Connection_Handler::~IIOP_SSL_Connection_Handler (void)
{
}

// ****************************************************************
