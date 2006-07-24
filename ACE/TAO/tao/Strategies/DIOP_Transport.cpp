// $Id$

#include "tao/Strategies/DIOP_Transport.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "tao/Strategies/DIOP_Connection_Handler.h"
#include "tao/Strategies/DIOP_Acceptor.h"
#include "tao/Strategies/DIOP_Profile.h"
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

ACE_RCSID (tao, DIOP_Transport, "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DIOP_Transport::TAO_DIOP_Transport (TAO_DIOP_Connection_Handler *handler,
                                        TAO_ORB_Core *orb_core,
                                        CORBA::Boolean flag)
  : TAO_Transport (TAO_TAG_DIOP_PROFILE,
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
               TAO_GIOP_Message_Lite (orb_core,
                                      ACE_MAX_DGRAM_SIZE));
                                      }
  else
    {
      // Use the normal GIOP object
      ACE_NEW (this->messaging_object_,
               TAO_GIOP_Message_Base (orb_core,
                                      this,
                                      ACE_MAX_DGRAM_SIZE));
    }

//@@ MESSAGING_SPL_COMMENT_HOOK_END

}

TAO_DIOP_Transport::~TAO_DIOP_Transport (void)
{
  delete this->messaging_object_;
}

ACE_Event_Handler *
TAO_DIOP_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_DIOP_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Pluggable_Messaging *
TAO_DIOP_Transport::messaging_object (void)
{
  return this->messaging_object_;
}

ssize_t
TAO_DIOP_Transport::send (iovec *iov, int iovcnt,
                          size_t &bytes_transferred,
                          const ACE_Time_Value *)
{
  const ACE_INET_Addr &addr = this->connection_handler_->addr ();

  ssize_t bytes_to_send = 0;
  for (int i = 0; i < iovcnt; i++)
     bytes_to_send += iov[i].iov_len;

  this->connection_handler_->dgram ().send (iov,
                                            iovcnt,
                                            addr);
  // @@ Michael:
  // Always return a positive number of bytes sent, as we do
  // not handle sending errors in DIOP.

  bytes_transferred = bytes_to_send;

  return 1;
}

ssize_t
TAO_DIOP_Transport::recv (char *buf,
                          size_t len,
                          const ACE_Time_Value * /* max_wait_time */)
{
  ACE_INET_Addr from_addr;

  ssize_t n = this->connection_handler_->dgram ().recv (buf,
                                                        len,
                                                        from_addr);

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO_DIOP_Transport::recv_i: received %d bytes from %s:%d %d\n",
                  n,
                  ACE_TEXT_CHAR_TO_TCHAR (from_addr.get_host_name ()),
                  from_addr.get_port_number (),
                  errno));
    }

  // Most of the errors handling is common for
  // Now the message has been read
  if (n == -1 && TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - %p \n"),
                  ACE_TEXT ("TAO - read message failure ")
                  ACE_TEXT ("recv () \n")));
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

  // Remember the from addr to eventually use it as remote
  // addr for the reply.
  this->connection_handler_->addr (from_addr);

  return n;
}

int
TAO_DIOP_Transport::handle_input (TAO_Resume_Handle &rh,
                                  ACE_Time_Value *max_wait_time,
                                  int /*block*/)
{
  // If there are no messages then we can go ahead to read from the
  // handle for further reading..

  // The buffer on the stack which will be used to hold the input
  // messages
  char buf [ACE_MAX_DGRAM_SIZE + ACE_CDR::MAX_ALIGNMENT];

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


  // Read the message into the  message block that we have created on
  // the stack.
  ssize_t n = this->recv (message_block.rd_ptr (),
                          message_block.space (),
                          max_wait_time);

  // If there is an error return to the reactor..
  if (n <= 0)
    {
      if (n == -1)
        this->tms_->connection_closed ();

      return n;
    }

  // Set the write pointer in the stack buffer
  message_block.wr_ptr (n);

  // Make a node of the message block..
  TAO_Queued_Data qd (&message_block);
  size_t mesg_length;

  // Parse the incoming message for validity. The check needs to be
  // performed by the messaging objects.
  if (this->messaging_object ()->parse_next_message (message_block, 
                                                     qd,
                                                     mesg_length) == -1) 
    return -1;

  if (qd.missing_data_ == TAO_MISSING_DATA_UNDEFINED)
    {
      // parse/marshal error
      return -1;
    }
  
  if (message_block.length () > mesg_length)
    {
      // we read too much data
      return -1;
    }

  // NOTE: We are not performing any queueing nor any checking for
  // missing data. We are assuming that ALL the data would be got in a
  // single read.

  // Process the message
  return this->process_parsed_messages (&qd, rh);
}


int
TAO_DIOP_Transport::register_handler (void)
{
  // @@ Michael:
  //
  // We do never register register the handler with the reactor
  // as we never need to be informed about any incoming data,
  // assuming we only use one-ways.
  // If we would register and ICMP Messages would arrive, e.g
  // due to a not reachable server, we would get informed - as this
  // disturbs the general DIOP assumptions of not being
  // interested in any network failures, we ignore ICMP messages.
  return 0;
}


int
TAO_DIOP_Transport::send_request (TAO_Stub *stub,
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
TAO_DIOP_Transport::send_message (TAO_OutputCDR &stream,
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
TAO_DIOP_Transport::send_message_shared (TAO_Stub *stub,
                                         int message_semantics,
                                         const ACE_Message_Block *message_block,
                                         ACE_Time_Value *max_wait_time)
{
  int result;

  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->handler_lock_, -1);

    result =
      this->send_message_shared_i (stub, message_semantics,
                                   message_block, max_wait_time);
  }

  if (result == -1)
    {
      this->close_connection ();
    }

  return result;
}

int
TAO_DIOP_Transport::messaging_init (CORBA::Octet major,
                                    CORBA::Octet minor)
{
  this->messaging_object_->init (major, minor);
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
