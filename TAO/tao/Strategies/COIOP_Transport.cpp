// $Id$

#include "tao/Strategies/COIOP_Transport.h"

#if defined (TAO_HAS_COIOP) && (TAO_HAS_COIOP != 0)

#include "tao/Strategies/COIOP_Connection_Handler.h"
#include "tao/Strategies/COIOP_Acceptor.h"
#include "tao/Strategies/COIOP_Profile.h"
#include "tao/Acceptor_Registry.h"
#include "tao/operation_details.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Resume_Handle.h"
#include "tao/GIOP_Message_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_COIOP_Transport::TAO_COIOP_Transport (TAO_COIOP_Connection_Handler *handler,
                                          TAO_ORB_Core *orb_core)
  : TAO_Transport (TAO_TAG_COIOP_PROFILE,
                   orb_core)
  , connection_handler_ (handler)
{
}

TAO_COIOP_Transport::~TAO_COIOP_Transport (void)
{
}

ACE_Event_Handler *
TAO_COIOP_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_COIOP_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

ssize_t
TAO_COIOP_Transport::send (iovec *,
                          int ,
                          size_t &bytes_transferred,
                          const ACE_Time_Value *)
{
  // We don't send data over the wire with COIOP
  bytes_transferred = 0;

  return 1;
}

ssize_t
TAO_COIOP_Transport::recv (char *, size_t , const ACE_Time_Value *)
{
  return 0;
}

int
TAO_COIOP_Transport::handle_input (TAO_Resume_Handle &,
                                  ACE_Time_Value *)
{
  return 0;
}


int
TAO_COIOP_Transport::register_handler (void)
{
  // We do never register register the handler with the reactor
  // as we never need to be informed about any incoming data,
  // assuming we only use one-ways.
  // If we would register and ICMP Messages would arrive, e.g
  // due to a not reachable server, we would get informed - as this
  // disturbs the general COIOP assumptions of not being
  // interested in any network failures, we ignore ICMP messages.
  return 0;
}


int
TAO_COIOP_Transport::send_request (TAO_Stub *,
                                  TAO_ORB_Core *,
                                  TAO_OutputCDR &,
                                  TAO_Message_Semantics,
                                  ACE_Time_Value *)
{
  return 0;
}

int
TAO_COIOP_Transport::send_message (TAO_OutputCDR &,
                                   TAO_Stub *,
                                   TAO_ServerRequest *,
                                   TAO_Message_Semantics,
                                   ACE_Time_Value *)
{
  return 1;
}

int
TAO_COIOP_Transport::send_message_shared (TAO_Stub *,
                                          TAO_Message_Semantics,
                                          const ACE_Message_Block *,
                                          ACE_Time_Value *)
{
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */
