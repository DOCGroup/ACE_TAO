// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "UIOP_Transport.h"

#if TAO_HAS_UIOP == 1

#include "UIOP_Connection_Handler.h"
#include "UIOP_Profile.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Sync_Strategies.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/GIOP_Message_Lite.h"

#if !defined (__ACE_INLINE__)
# include "UIOP_Transport.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (Strategies, UIOP_Transport, "$Id$")


TAO_UIOP_Transport::TAO_UIOP_Transport (TAO_UIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Boolean flag)
  : TAO_Transport (TAO_TAG_UIOP_PROFILE,
                   orb_core)
  , connection_handler_ (handler)
  , messaging_object_ (0)
{
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
               TAO_GIOP_Message_Base (orb_core));
    }
}

TAO_UIOP_Transport::~TAO_UIOP_Transport (void)
{
  delete this->messaging_object_;
}

ACE_Event_Handler *
TAO_UIOP_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Pluggable_Messaging *
TAO_UIOP_Transport::messaging_object (void)
{
  return this->messaging_object_;
}

ssize_t
TAO_UIOP_Transport::send_i (iovec *iov, int iovcnt,
                            size_t &bytes_transferred,
                            const ACE_Time_Value *max_wait_time)
{
  ssize_t retval = this->connection_handler_->peer ().sendv (iov, iovcnt,
                                                             max_wait_time);
  if (retval > 0)
    bytes_transferred = retval;

  return retval;
}

ssize_t
TAO_UIOP_Transport::recv_i (char *buf,
                            size_t len,
                            const ACE_Time_Value *max_wait_time)
{
  ssize_t n = this->connection_handler_->peer ().recv (buf,
                                                       len,
                                                       max_wait_time);

  // Most of the errors handling is common for
  // Now the message has been read
  if (n == -1 &&
      TAO_debug_level > 4 &&
      errno != ETIME)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p \n"),
                  ACE_TEXT ("TAO - read message failure ")
                  ACE_TEXT ("recv_i () \n")));
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
TAO_UIOP_Transport::register_handler_i (void)
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - UIOP_Transport::register_handler %d\n",
                  this->id ()));
    }
  // @@ It seems like this method should go away, the right reactor is
  //    picked at object creation time.
  ACE_Reactor *r = this->orb_core_->reactor ();

  if (r == this->connection_handler_->reactor ())
    return 0;

  // Set the flag in the Connection Handler
  this->ws_->is_registered (1);

  // Register the handler with the reactor
  return  r->register_handler (this->connection_handler_,
                               ACE_Event_Handler::READ_MASK);
}


int
TAO_UIOP_Transport::send_request (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  TAO_OutputCDR &stream,
                                  int two_way,
                                  ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  two_way) == -1)
    return -1;

  if (this->send_message (stream,
                          stub,
                          two_way,
                          max_wait_time) == -1)

    return -1;

  return this->idle_after_send ();
}

int
TAO_UIOP_Transport::send_message (TAO_OutputCDR &stream,
                                  TAO_Stub *stub,
                                  int twoway,
                                  ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object_->format_message (stream) != 0)
    return -1;

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = this->send_message_i (stub,
                                    twoway,
                                    stream.begin (),
                                    max_wait_time);

  if (n == -1)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO: (%P|%t|%N|%l) closing transport %d after fault %p\n"),
                    this->id (),
                    ACE_TEXT ("send_message ()\n")));

      return -1;
    }

  return 1;
}


int
TAO_UIOP_Transport::messaging_init (CORBA::Octet major,
                                    CORBA::Octet minor)
{
  this->messaging_object_->init (major,
                                 minor);
  return 1;
}

void
TAO_UIOP_Transport::transition_handler_state_i (void)
{
  connection_handler_ = 0;
}

#endif  /* TAO_HAS_UIOP */
