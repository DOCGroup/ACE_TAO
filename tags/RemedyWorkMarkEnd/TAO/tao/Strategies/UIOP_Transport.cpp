// $Id$

#include "tao/Strategies/UIOP_Transport.h"

#if TAO_HAS_UIOP == 1

#include "tao/Strategies/UIOP_Connection_Handler.h"
#include "tao/Strategies/UIOP_Profile.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIOP_Transport::TAO_UIOP_Transport (TAO_UIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core)
  : TAO_Transport (TAO_TAG_UIOP_PROFILE,
                   orb_core)
  , connection_handler_ (handler)
{
}

TAO_UIOP_Transport::~TAO_UIOP_Transport (void)
{
}

ACE_Event_Handler *
TAO_UIOP_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_UIOP_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

ssize_t
TAO_UIOP_Transport::send (iovec *iov, int iovcnt,
                          size_t &bytes_transferred,
                          const ACE_Time_Value *max_wait_time)
{
  ssize_t const retval =
    this->connection_handler_->peer ().sendv (iov, iovcnt, max_wait_time);

  if (retval > 0)
    bytes_transferred = retval;

  return retval;
}

ssize_t
TAO_UIOP_Transport::recv (char *buf,
                          size_t len,
                          const ACE_Time_Value *max_wait_time)
{
  const ssize_t n = this->connection_handler_->peer ().recv (buf,
                                                             len,
                                                             max_wait_time);

  // Most of the errors handling is common for
  // Now the message has been read
  if (n == -1 &&
      TAO_debug_level > 4 &&
      errno != ETIME)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - UIOP_Transport::recv, %p %p\n"),
                  ACE_TEXT ("TAO - read message failure ")
                  ACE_TEXT ("recv ()\n")));
    }

  // Error handling
  if (n == -1)
    {
      if (errno == EWOULDBLOCK)
        return 0;

      return -1;
    }
  // @@ What are the other error handling here??
  else if (n == 0)
    {
      return -1;
    }

  return n;
}

int
TAO_UIOP_Transport::send_request (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  TAO_OutputCDR &stream,
                                  TAO_Message_Semantics message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core, message_semantics) == -1)
    return -1;

  if (this->send_message (stream, stub, message_semantics, max_wait_time) == -1)
    return -1;

  this->first_request_sent();

  return 0;
}

int
TAO_UIOP_Transport::send_message (TAO_OutputCDR &stream,
                                  TAO_Stub *stub,
                                  TAO_Message_Semantics message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object ()->format_message (stream, stub) != 0)
    return -1;

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  // This guarantees to send all data (bytes) or return an error.
  const ssize_t n = this->send_message_shared (stub,
                                               message_semantics,
                                               stream.begin (),
                                               max_wait_time);

  if (n == -1)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) closing transport %d after fault %p\n"),
                    this->id (),
                    ACE_TEXT ("send_message ()\n")));

      return -1;
    }

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_UIOP */
