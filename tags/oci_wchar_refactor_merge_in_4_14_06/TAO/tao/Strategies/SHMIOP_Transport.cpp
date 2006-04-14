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
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/Resume_Handle.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/GIOP_Message_Lite.h"

ACE_RCSID (Strategies, SHMIOP_Transport, "$Id$")

TAO_SHMIOP_Transport::TAO_SHMIOP_Transport (TAO_SHMIOP_Connection_Handler *handler,
                                            TAO_ORB_Core *orb_core,
                                            CORBA::Boolean flag)
  : TAO_Transport (TAO_TAG_SHMEM_PROFILE,
                   orb_core),
    connection_handler_ (handler),
    messaging_object_ (0)
{
/*
 * Hook to customize the messaging object when the concrete messaging
 * object is known a priori. In this case, the flag is ignored.
 */
//@@ MESSAGING_SPL_COMMENT_HOOK_START
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
//@@ MESSAGING_SPL_COMMENT_HOOK_END
}

TAO_SHMIOP_Transport::~TAO_SHMIOP_Transport (void)
{
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
TAO_SHMIOP_Transport::send (iovec *iov, int iovcnt,
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
TAO_SHMIOP_Transport::recv (char *buf,
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

  return 0;
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

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
