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
#include "tao/GIOP_Message_Lite.h"

ACE_RCSID (tao, COIOP_Transport, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_COIOP_Transport::TAO_COIOP_Transport (TAO_COIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Boolean flag)
  : TAO_Transport (TAO_TAG_COIOP_PROFILE,
                   orb_core)
  , connection_handler_ (handler)
  , messaging_object_ (0)
{
/*
 * Hook to customize the messaging object when the concrete messaging
 * object is known a priori. In this case, the flag is ignored.
 */
//@@ MESSAGING_SPL_COMMENT_HOOK_START

  // @@ Michael: Set the input CDR size to ACE_MAX_DGRAM_SIZE so that
  //             we read the whole UDP packet on a single read.
  if (flag)
    {
      // Use the lite version of the protocol
      ACE_NEW (this->messaging_object_,
               TAO_GIOP_Message_Lite (orb_core));
    }
  else
    {
      // Use the normal GIOP object
      ACE_NEW (this->messaging_object_,
               TAO_GIOP_Message_Base (orb_core,
                                      this));
    }

//@@ MESSAGING_SPL_COMMENT_HOOK_END

}

TAO_COIOP_Transport::~TAO_COIOP_Transport (void)
{
  delete this->messaging_object_;
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

TAO_Pluggable_Messaging *
TAO_COIOP_Transport::messaging_object (void)
{
  return this->messaging_object_;
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
                                  ACE_Time_Value *,
                                  int /*block*/)
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
                                  int,
                                  ACE_Time_Value *)
{
  return 0;
}

int
TAO_COIOP_Transport::send_message (TAO_OutputCDR &,
                                  TAO_Stub *,
                                  int,
                                  ACE_Time_Value *)
{
  return 1;
}

int
TAO_COIOP_Transport::send_message_shared (TAO_Stub *,
                                         int,
                                         const ACE_Message_Block *,
                                         ACE_Time_Value *)
{
  return 1;
}

int
TAO_COIOP_Transport::messaging_init (CORBA::Octet major,
                                    CORBA::Octet minor)
{
  this->messaging_object_->init (major, minor);
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_COIOP && TAO_HAS_COIOP != 0 */
