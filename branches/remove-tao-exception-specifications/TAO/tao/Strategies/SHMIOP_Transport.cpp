// $Id$

#include "tao/Strategies/SHMIOP_Transport.h"

#if defined (TAO_HAS_SHMIOP) && (TAO_HAS_SHMIOP != 0)

#include "tao/Strategies/SHMIOP_Connection_Handler.h"
#include "tao/Strategies/SHMIOP_Profile.h"
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


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
               TAO_GIOP_Message_Base (orb_core, this));
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
TAO_SHMIOP_Transport::handle_input (TAO_Resume_Handle &rh, 
                                    ACE_Time_Value *max_wait_time, 
                                    int)
{
  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - SHMIOP_Transport[%d]::handle_input\n",
                  this->id ()));
    }

   // The buffer on the stack which will be used to hold the input
  // messages, compensate shrink due to alignment
  char buf [TAO_MAXBUFSIZE + ACE_CDR::MAX_ALIGNMENT];


#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
  (void) ACE_OS::memset (buf,
                         '\0',
                         sizeof buf);
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

  // Create a data block
  ACE_Data_Block db (sizeof (buf),
                     ACE_Message_Block::MB_DATA,
                     buf,
                     this->orb_core_->input_cdr_buffer_allocator (),
                     this->orb_core_->locking_strategy (),
                     ACE_Message_Block::DONT_DELETE,
                     this->orb_core_->input_cdr_dblock_allocator ());

  // Create a message block
  ACE_Message_Block message_block (&db,
                                   ACE_Message_Block::DONT_DELETE,
                                   this->orb_core_->input_cdr_msgblock_allocator ());


  // Align the message block
  ACE_CDR::mb_align (&message_block);

  const size_t missing_header_data = this->messaging_object ()->header_length ();

  if (missing_header_data == 0) 
    {
      return -1;
    }

  // .. do a read on the socket again.
  ssize_t bytes = 0;

  // As this used for transports where things are available in one
  // shot this looping should not create any problems.
  for (size_t m = missing_header_data;
       m != 0;
       m -= bytes)
    {
      bytes = 0; // reset

      // We would have liked to use something like a recv_n ()
      // here. But at the time when the code was written, the MEM_Stream
      // classes had poor support  for recv_n (). Till a day when we
      // get proper recv_n (), let us stick with this. The other
      // argument that can be said against this is that, this is the
      // bad layer in which this is being done ie. recv_n is
      // simulated. But...
      bytes = this->recv (message_block.wr_ptr (),
                          m,
                          max_wait_time);

      if (bytes == 0 ||
          bytes == -1)
        {
          return -1;
        }

      message_block.wr_ptr (bytes);
    }

    TAO_Queued_Data qd (&message_block);
    size_t mesg_length; // not used

    // Parse the incoming message for validity. The check needs to be
    // performed by the messaging objects.
    if (this->messaging_object ()->parse_next_message (message_block,
                                                       qd,
                                                       mesg_length) == -1)
      return -1;

    if (qd.missing_data_ == TAO_MISSING_DATA_UNDEFINED) 
      {
        // parse/marshal error happened
        return -1;
      }

    if (message_block.length () > mesg_length)
      {
        // we read too much data
        return -1;
      }
    
    if (message_block.space () < qd.missing_data_)
      {
        const size_t message_size = message_block.length ()
                                  + qd.missing_data_;

        // reallocate buffer with correct size on heap
	if (ACE_CDR::grow (&message_block, message_size) == -1)
          {
            if (TAO_debug_level > 0)
              {
                ACE_ERROR ((LM_ERROR,
                 "TAO (%P|%t) - SHMIOP_Transport[%d]::handle_input, "
                 "error growing message buffer\n",
                 this->id () ));
              }
            return -1;
          }

      }

    // As this used for transports where things are available in one
    // shot this looping should not create any problems.
    for (size_t n = qd.missing_data_;
       n != 0;
       n -= bytes)
    {
        bytes = 0; // reset

      // We would have liked to use something like a recv_n ()
      // here. But at the time when the code was written, the MEM_Stream
      // classes had poor support  for recv_n (). Till a day when we
      // get proper recv_n (), let us stick with this. The other
      // argument that can be said against this is that, this is the
      // bad layer in which this is being done ie. recv_n is
      // simulated. But...
        bytes = this->recv (message_block.wr_ptr (),
                          n,
                          max_wait_time);

      if (bytes == 0 ||
          bytes == -1)
        {
          return -1;
        }

        message_block.wr_ptr (bytes);

    }

  qd.missing_data_ = 0;

  // Now we have a full message in our buffer. Just go ahead and
  // process that
  if (this->process_parsed_messages (&qd, rh) == -1)
    {
      return -1;
    }
  
  return 0;
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

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_SHMIOP && TAO_HAS_SHMIOP != 0 */
