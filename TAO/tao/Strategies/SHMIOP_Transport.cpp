// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "SHMIOP_Transport.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "SHMIOP_Connection_Handler.h"
#include "SHMIOP_Profile.h"
#include "tao/Timeprobe.h"
#include "tao/CDR.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/Wait_Strategy.h"
#include "tao/Sync_Strategies.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Resume_Handle.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/GIOP_Message_Lite.h"


#if !defined (__ACE_INLINE__)
# include "SHMIOP_Transport.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (Strategies, SHMIOP_Transport, "$Id$")

TAO_SHMIOP_Transport::TAO_SHMIOP_Transport (TAO_SHMIOP_Connection_Handler *handler,
                                            TAO_ORB_Core *orb_core,
                                            CORBA::Boolean flag)
  : TAO_Transport (TAO_TAG_SHMEM_PROFILE,
                   orb_core),
    connection_handler_ (handler),
    messaging_object_ (0)
{
  if (connection_handler_ != 0)
    {
      // REFCNT: Matches one of
      // TAO_Transport::connection_handler_close() or
      // TAO_Transport::close_connection_shared.
      this->connection_handler_->incr_refcount();
    }
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

TAO_SHMIOP_Transport::~TAO_SHMIOP_Transport (void)
{
  ACE_ASSERT(this->connection_handler_ == 0);
  delete this->messaging_object_;
}

ACE_Event_Handler *
TAO_SHMIOP_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_SHMIOP_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Pluggable_Messaging *
TAO_SHMIOP_Transport::messaging_object (void)
{
  return this->messaging_object_;
}


ssize_t
TAO_SHMIOP_Transport::send_i (iovec *iov, int iovcnt,
                              size_t &bytes_transferred,
                              const ACE_Time_Value *max_wait_time)
{
  bytes_transferred = 0;
  for (int i = 0; i < iovcnt; ++i)
    {
      ssize_t retval =
        this->connection_handler_->peer ().send (iov[i].iov_base,
                                                 iov[i].iov_len,
                                                 max_wait_time);
      if (retval > 0)
        bytes_transferred += retval;
      if (retval <= 0)
        return retval;
    }
  return bytes_transferred;
}

ssize_t
TAO_SHMIOP_Transport::recv_i (char *buf,
                              size_t len,
                              const ACE_Time_Value *max_wait_time)
{
  ssize_t n = 0;

  int read_break = 0;

  while (!read_break)
    {
      n = this->connection_handler_->peer ().recv (buf,
                                                   len,
                                                   max_wait_time);

      // If we get a EWOULBLOCK we try to read again.
      if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
          n = 0;
          continue;
        }

      // If there is anything else we just drop out of the loop.
      read_break = 1;
    }

  if (n == -1)
    {
      if (TAO_debug_level > 3 && errno != ETIME)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - %p \n"),
                      ACE_TEXT ("TAO - read message failure ")
                      ACE_TEXT ("recv_i () \n")));
        }
    }
  else if (n == 0)
    {
      n = -1;
    }
  return n;
}


int
TAO_SHMIOP_Transport::consolidate_message (ACE_Message_Block &incoming,
                                           ssize_t missing_data,
                                           TAO_Resume_Handle &rh,
                                           ACE_Time_Value *max_wait_time)
{
  // Calculate the actual length of the load that we are supposed to
  // read which is equal to the <missing_data> + length of the buffer
  // that we have..
  size_t payload = missing_data + incoming.length ();

  // Grow the buffer to the size of the message
  ACE_CDR::grow (&incoming,
                 payload);

  // .. do a read on the socket again.
  ssize_t bytes = 0;

  // As this used for transports where things are available in one
  // shot this looping should not create any problems.
  for (size_t n = missing_data;
       n != 0;
       n -= bytes)
    {
      // We would have liked to use something like a recv_n ()
      // here. But at the time when the code was written, the MEM_Stream
      // classes had poor support  for recv_n (). Till a day when we
      // get proper recv_n (), let us stick with this. The other
      // argument that can be said against this is that, this is the
      // bad layer in which this is being done ie. recv_n is
      // simulated. But...
      bytes = this->recv (incoming.wr_ptr (),
                          n,
                          max_wait_time);

      if (bytes == 0 ||
          bytes == -1)
        {
          return -1;
        }

      incoming.wr_ptr (bytes);
    }

  TAO_Queued_Data pqd (&incoming);

  // With SHMIOP we would not have any missing data...
  pqd.missing_data_ = 0;

  this->messaging_object ()->get_message_data (&pqd);

  // Now we have a full message in our buffer. Just go ahead and
  // process that
  return this->process_parsed_messages (&pqd, rh);
}


int
TAO_SHMIOP_Transport::register_handler_i (void)
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - SHMIOP_Transport::register_handler %d\n",
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
TAO_SHMIOP_Transport::send_request (TAO_Stub *stub,
                                  TAO_ORB_Core *orb_core,
                                  TAO_OutputCDR &stream,
                                  int message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  message_semantics) == -1)
    return -1;

  if (this->send_message (stream,
                          stub,
                          message_semantics,
                          max_wait_time) == -1)

    return -1;
  this->first_request_sent();

  return this->idle_after_send ();
}

int
TAO_SHMIOP_Transport::send_message (TAO_OutputCDR &stream,
                                  TAO_Stub *stub,
                                  int message_semantics,
                                  ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object_->format_message (stream) != 0)
    return -1;

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = this->send_message_shared (stub,
                                         message_semantics,
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
TAO_SHMIOP_Transport::messaging_init (CORBA::Octet major,
                                    CORBA::Octet minor)
{
  this->messaging_object_->init (major,
                                 minor);
  return 1;
}

TAO_Connection_Handler *
TAO_SHMIOP_Transport::invalidate_event_handler_i (void)
{
  TAO_Connection_Handler * eh = this->connection_handler_;
  this->connection_handler_ = 0;
  return eh;
}

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
