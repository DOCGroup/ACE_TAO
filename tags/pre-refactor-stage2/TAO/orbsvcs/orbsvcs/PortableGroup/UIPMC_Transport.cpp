// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "UIPMC_Transport.h"

#include "UIPMC_Connection_Handler.h"
#include "UIPMC_Acceptor.h"
#include "UIPMC_Profile.h"
#include "UIPMC_Wait_Never.h"
#include "tao/Acceptor_Registry.h"
#include "tao/operation_details.h"
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
# include "UIPMC_Transport.i"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (PortableGroup,
           UIPMC_Transport,
           "$Id$")


// Local MIOP Definitions:

// Note: We currently support packet fragmentation on transmit, but
//       do not support reassembly.

// Limit the number of fragments that we can divide a message
// into.
#define MIOP_MAX_FRAGMENTS    (4)
#define MIOP_MAX_HEADER_SIZE  (272) // See MIOP Spec.  Must be a multiple of 8.
#define MIOP_MAX_DGRAM_SIZE   (ACE_MAX_DGRAM_SIZE)

#define MIOP_MAGIC_OFFSET             (0)
#define MIOP_VERSION_OFFSET           (4)
#define MIOP_FLAGS_OFFSET             (5)
#define MIOP_PACKET_LENGTH_OFFSET     (6)
#define MIOP_PACKET_NUMBER_OFFSET     (8)
#define MIOP_NUMBER_OF_PACKETS_OFFSET (12)
#define MIOP_ID_LENGTH_OFFSET         (16)
#define MIOP_MIN_LENGTH_ID            (0)
#define MIOP_MAX_LENGTH_ID            (252)
#define MIOP_ID_DEFAULT_LENGTH        (12)
#define MIOP_ID_CONTENT_OFFSET        (20)
#define MIOP_HEADER_PADDING           (0)   // The ID field needs to be padded to
                                            // a multiple of 8 bytes.
#define MIOP_HEADER_SIZE              (MIOP_ID_CONTENT_OFFSET   \
                                       + MIOP_ID_DEFAULT_LENGTH \
                                       + MIOP_HEADER_PADDING)
#define MIOP_MIN_HEADER_SIZE          (MIOP_ID_CONTENT_OFFSET   \
                                       + MIOP_MIN_LENGTH_ID     \
                                       + (8 - MIOP_MIN_LENGTH_ID) /* padding */)

static const CORBA::Octet miop_magic[4] = { 0x4d, 0x49, 0x4f, 0x50 }; // 'M', 'I', 'O', 'P'


TAO_UIPMC_Transport::TAO_UIPMC_Transport (TAO_UIPMC_Connection_Handler *handler,
                                          TAO_ORB_Core *orb_core,
                                          CORBA::Boolean /*flag*/)
  : TAO_Transport (TAO_TAG_UIPMC_PROFILE,
                   orb_core)
  , connection_handler_ (handler)
  , messaging_object_ (0)
{
  if (connection_handler_ != 0)
    {
      // REFCNT: Matches one of
      // TAO_Transport::connection_handler_close() or
      // TAO_Transport::close_connection_shared.
      this->connection_handler_->incr_refcount();
    }

  // Use the normal GIOP object
  ACE_NEW (this->messaging_object_,
           TAO_GIOP_Message_Base (orb_core,
                                  ACE_MAX_DGRAM_SIZE));

  // Replace the default wait strategy with our own
  // since we don't support waiting on anything.
  delete this->ws_;
  ACE_NEW (this->ws_,
           TAO_UIPMC_Wait_Never (this));
}

TAO_UIPMC_Transport::~TAO_UIPMC_Transport (void)
{
  ACE_ASSERT(this->connection_handler_ == 0);
  delete this->messaging_object_;
}

ACE_Event_Handler *
TAO_UIPMC_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_UIPMC_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Pluggable_Messaging *
TAO_UIPMC_Transport::messaging_object (void)
{
  return this->messaging_object_;
}

struct MIOP_Packet
{
  iovec iov[ACE_IOV_MAX];
  int iovcnt;
  int length;
};

class ACE_Message_Block_Data_Iterator
{
public:
  /// Constructor
  ACE_Message_Block_Data_Iterator (iovec *iov, int iovcnt);

  /// Get the next data block that has a size less than or equal
  /// to max_length.  Return the length of the block returned.
  size_t next_block (size_t max_length,
                     iovec &block);

private:
  enum State
  {
    INTER_BLOCK,
    INTRA_BLOCK
  };

  iovec *iov_;
  int iovcnt_;

  // Point internal to a message block, if we have to split one up.
  char *iov_ptr_;
  int iov_index_;

  // Length used in a split message block.
  size_t iov_len_left_;

  // Current message iterator state.
  State state_;

};

ACE_Message_Block_Data_Iterator::ACE_Message_Block_Data_Iterator (iovec *iov, int iovcnt) :
  iov_ (iov),
  iovcnt_ (iovcnt),
  iov_ptr_ (0),
  iov_index_ (0),
  iov_len_left_ (0),
  state_ (INTER_BLOCK)
{
}

size_t
ACE_Message_Block_Data_Iterator::next_block (size_t max_length,
                                             iovec &block)
{
  if (this->state_ == INTER_BLOCK)
    {
      // Check that there are some iovec buffers left.
      if (this->iov_index_ >= this->iovcnt_)
        return 0;


      size_t current_iov_len =
                    this->iov_[this->iov_index_].iov_len;

      if (current_iov_len <= max_length)
        {
          // Return the full data portion.
          block.iov_len = ACE_static_cast (u_long, current_iov_len);
          block.iov_base = this->iov_[this->iov_index_].iov_base;

          // Go to the next block.
          this->iov_index_++;

          return current_iov_len;
        }
      else
        {
          // Let the caller use the first part of this
          // message block.
          block.iov_len = ACE_static_cast (u_long, max_length);
          block.iov_base = this->iov_[this->iov_index_].iov_base;

          // Break up the block.
          this->iov_len_left_ = current_iov_len - max_length;
          this->iov_ptr_ =
            ACE_reinterpret_cast (char *,
                                  ACE_reinterpret_cast (char *, block.iov_base)
                                  + max_length);
          this->state_ = INTRA_BLOCK;

          return max_length;
        }
    }
  else
    {
      // Currently scanning a split block.
      if (this->iov_len_left_ <= max_length)
        {
          // Return everything that's left in the block.
          block.iov_len = ACE_static_cast (u_long, this->iov_len_left_);
          block.iov_base = this->iov_ptr_;

          // Go to the next block.
          this->iov_index_++;

          // Update the state.
          this->state_ = INTER_BLOCK;

          return this->iov_len_left_;
        }
      else
        {
          // Split a little more off the block.
          block.iov_len = ACE_static_cast (u_long, this->iov_len_left_);
          block.iov_base = this->iov_ptr_;

          this->iov_len_left_ -= max_length;
          this->iov_ptr_ += max_length;
          return max_length;
        }
    }
}

void
TAO_UIPMC_Transport::write_unique_id (TAO_OutputCDR &miop_hdr, unsigned long unique)
{
  // We currently construct a unique ID for each MIOP message by
  // concatenating the address of the buffer to a counter.  We may
  // also need to use a MAC address or something more unique to
  // fully comply with the MIOP specification.

  static unsigned long counter = 1;  // Don't worry about race conditions on counter,
                                     // since buffer addresses can't be the same if
                                     // this is being called simultaneously.

  CORBA::Octet unique_id[MIOP_ID_DEFAULT_LENGTH];

  unique_id[0] = ACE_static_cast (CORBA::Octet, unique & 0xff);
  unique_id[1] = ACE_static_cast (CORBA::Octet, (unique & 0xff00) >> 8);
  unique_id[2] = ACE_static_cast (CORBA::Octet, (unique & 0xff0000) >> 16);
  unique_id[3] = ACE_static_cast (CORBA::Octet, (unique & 0xff000000) >> 24);

  unique_id[4] = ACE_static_cast (CORBA::Octet, counter & 0xff);
  unique_id[5] = ACE_static_cast (CORBA::Octet, (counter & 0xff00) >> 8);
  unique_id[6] = ACE_static_cast (CORBA::Octet, (counter & 0xff0000) >> 16);
  unique_id[7] = ACE_static_cast (CORBA::Octet, (counter & 0xff000000) >> 24);

  unique_id[8] = 0;
  unique_id[9] = 0;
  unique_id[10] = 0;
  unique_id[11] = 0;

  miop_hdr.write_ulong (MIOP_ID_DEFAULT_LENGTH);
  miop_hdr.write_octet_array (unique_id, MIOP_ID_DEFAULT_LENGTH);
}

ssize_t
TAO_UIPMC_Transport::send_i (iovec *iov, int iovcnt,
                             size_t &bytes_transferred,
                             const ACE_Time_Value *)
{
  const ACE_INET_Addr &addr = this->connection_handler_->addr ();
  bytes_transferred = 0;

  // Calculate the bytes to send.  This value is only used for
  // error conditions to fake a good return.  We do this for
  // semantic consistency with DIOP, and since errors aren't
  // handled correctly from send_i (our fault).  If these
  // semantics are not desirable, the error handling problems
  // that need to be fixed can be found in
  // UIPMC_Connection_Handler::decr_refcount which will need to
  // deregister the connection handler from the UIPMC_Connector
  // cache.
  ssize_t bytes_to_send = 0;
  for (int i = 0; i < iovcnt; i++)
     bytes_to_send += iov[i].iov_len;

  MIOP_Packet fragments[MIOP_MAX_FRAGMENTS];
  MIOP_Packet *current_fragment;
  int num_fragments = 1;

  ACE_Message_Block_Data_Iterator mb_iter (iov, iovcnt);

  // Initialize the first fragment
  current_fragment = &fragments[0];
  current_fragment->iovcnt = 1;  // The MIOP Header
  current_fragment->length = MIOP_HEADER_SIZE;

  // Go through all of the message blocks.
  while (mb_iter.next_block (MIOP_MAX_DGRAM_SIZE - current_fragment->length,
                             current_fragment->iov[current_fragment->iovcnt]))
    {
      // Increment the length and iovcnt.
      current_fragment->length += current_fragment->iov[current_fragment->iovcnt].iov_len;
      current_fragment->iovcnt++;

      // Check if we've filled up this fragment or if we've run out of
      // iov entries.
      if (current_fragment->length == MIOP_MAX_DGRAM_SIZE ||
          current_fragment->iovcnt == ACE_IOV_MAX)
        {
          // Make a new fragment.
          num_fragments++;

          // Check if too many fragments
          if (num_fragments > MIOP_MAX_FRAGMENTS)
            {
              // This is an error as we do not send more.
              // Silently drop the message but log an error.

              // Pluggable_Messaging::transport_message only
              // cares if it gets -1 or 0 so we can return a
              // partial length and it will think all has gone
              // well.
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("\n\nTAO (%P|%t) ")
                              ACE_TEXT ("UIPMC_Transport::send_i ")
                              ACE_TEXT ("Message needs too many fragments (max is %d)\n"),
                              MIOP_MAX_FRAGMENTS));
                }

              // Pretend it is o.k.  See note by bytes_to_send calculation.
              bytes_transferred = bytes_to_send;
              return 1;
            }

          // Otherwise, initialize another fragment.
          current_fragment++;
          current_fragment->iovcnt = 1;  // The MIOP Header
          current_fragment->length = MIOP_HEADER_SIZE;
        }
    }

  // Build a generic MIOP Header.

  // Allocate space on the stack for the header (add 8 to account for
  // the possibility of adjusting for alignment).
  char header_buffer[MIOP_HEADER_SIZE + 8];
  TAO_OutputCDR miop_hdr (header_buffer, MIOP_HEADER_SIZE + 8);

  miop_hdr.write_octet_array (miop_magic, 4);   // Magic
  miop_hdr.write_octet (0x10);                  // Version
  CORBA::Octet *flags_field = ACE_reinterpret_cast (CORBA::Octet *,
                                                    miop_hdr.current ()->wr_ptr ());

  // Write flags octet:
  //  Bit        Description
  //   0         Endian
  //   1         Stop message flag (Assigned later)
  //   2 - 7     Set to 0
  miop_hdr.write_octet (TAO_ENCAP_BYTE_ORDER);  // Flags

  // Packet Length
  // NOTE: We can save pointers and write them later without byte swapping since
  //       in CORBA, the sender chooses the endian.
  CORBA::UShort *packet_length = ACE_reinterpret_cast (CORBA::UShort *,
                                                       miop_hdr.current ()->wr_ptr ());
  miop_hdr.write_short (0);

  // Packet number
  CORBA::ULong *packet_number = ACE_reinterpret_cast (CORBA::ULong *,
                                                      miop_hdr.current ()->wr_ptr ());
  miop_hdr.write_ulong (0);

  // Number of packets field
  miop_hdr.write_ulong (num_fragments);

  // UniqueId
  ptrdiff_t unique_id = ACE_reinterpret_cast (ptrdiff_t, iov);
  this->write_unique_id (miop_hdr,
                         ACE_static_cast (unsigned long, unique_id));

  // Send the buffers.
  current_fragment = &fragments[0];
  while (num_fragments > 0 &&
         current_fragment->iovcnt > 1)
    {
      // Fill in the packet length header field.
      *packet_length = current_fragment->length;

      // If this is the last fragment, set the stop message flag.
      if (num_fragments == 1)
        {
          *flags_field |= 0x02;
        }

      // Setup the MIOP header in the iov list.
      current_fragment->iov[0].iov_base = miop_hdr.current ()->rd_ptr ();
      current_fragment->iov[0].iov_len = MIOP_HEADER_SIZE;

      // Send the fragment. - Need to check for errors!!
      ssize_t rc = this->connection_handler_->dgram ().send (current_fragment->iov,
                                                             current_fragment->iovcnt,
                                                             addr);

      if (rc <= 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("\n\nTAO (%P|%t) ")
                          ACE_TEXT ("UIPMC_Transport::send")
                          ACE_TEXT (" %p\n\n"),
                          ACE_TEXT ("Error returned from transport:")));
            }

            // Pretend it is o.k.  See note by bytes_to_send calculation.
            bytes_transferred = bytes_to_send;
            return 1;
        }

      // Increment the number of bytes transferred, but don't
      // count the MIOP header that we added.
      bytes_transferred += rc - MIOP_HEADER_SIZE;

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_UIPMC_Transport::send_i: sent %d bytes to %s:%d\n",
                      rc,
                      addr.get_host_addr (),
                      addr.get_port_number ()));
        }

      // Go to the next fragment.
      (*packet_number)++;
      ++current_fragment;
      --num_fragments;
    }

  // Return total bytes transferred.
  return bytes_transferred;
}


ssize_t
TAO_UIPMC_Transport::recv_i (char *buf,
                             size_t len,
                             const ACE_Time_Value * /*max_wait_time*/)
{
  ACE_INET_Addr from_addr;

  ssize_t n = this->connection_handler_->mcast_dgram ().recv (buf,
                                                              len,
                                                              from_addr);
  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO_UIPMC_Transport::recv_i: received %d bytes from %s:%d\n",
                  n,
                  from_addr.get_host_addr (),
                  from_addr.get_port_number ()));
    }

  // Make sure that we at least have a MIOP header.
  if (n < MIOP_MIN_HEADER_SIZE)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_UIPMC_Transport::recv_i: packet of size %d is too small from %s:%d\n",
                      n,
                      from_addr.get_host_addr (),
                      from_addr.get_port_number ()));
        }
      return 0;
    }

  // Check for MIOP magic bytes.
  if (buf[MIOP_MAGIC_OFFSET] != miop_magic [0] ||
      buf[MIOP_MAGIC_OFFSET + 1] != miop_magic [1] ||
      buf[MIOP_MAGIC_OFFSET + 2] != miop_magic [2] ||
      buf[MIOP_MAGIC_OFFSET + 3] != miop_magic [3])
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_UIPMC_Transport::recv_i: UIPMC packet didn't contain magic bytes.\n"));
        }

      return 0;
    }

  // Retrieve the byte order.
  // 0 = Big endian
  // 1 = Small endian
  CORBA::Octet byte_order = buf[MIOP_FLAGS_OFFSET] & 0x01;

  // Ignore the header version, other flags, packet length and number of packets.

  // Get the length of the ID.
  CORBA::ULong id_length;
#if !defined (ACE_DISABLE_SWAP_ON_READ)
  if (byte_order == ACE_CDR_BYTE_ORDER)
    {
      id_length = *ACE_reinterpret_cast (ACE_CDR::ULong*, &buf[MIOP_ID_LENGTH_OFFSET]);
    }
  else
    {
      ACE_CDR::swap_4 (&buf[MIOP_ID_LENGTH_OFFSET],
                       ACE_reinterpret_cast (char*, &id_length));
    }
#else
  id_length = *ACE_reinterpret_cast (ACE_CDR::ULong*, &buf[MIOP_ID_LENGTH_OFFSET]);
#endif /* ACE_DISABLE_SWAP_ON_READ */

  // Make sure that the length field is legal.
  if (id_length > MIOP_MAX_LENGTH_ID ||
      ACE_static_cast (ssize_t, MIOP_ID_CONTENT_OFFSET + id_length) > n)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_UIPMC_Transport::recv_i: Invalid ID length.\n"));
        }

      return 0;
    }

  // Trim off the header for now.
  ssize_t miop_header_size = (MIOP_ID_CONTENT_OFFSET + id_length + 7) & ~0x7;
  if (miop_header_size > n)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_UIPMC_Transport::recv_i: MIOP packet not large enough for padding.\n"));
        }

      return 0;
    }

  n -= miop_header_size;
  ACE_OS::memmove (buf, buf + miop_header_size, n);

  return n;
}

int
TAO_UIPMC_Transport::handle_input_i (TAO_Resume_Handle &rh,
                                     ACE_Time_Value *max_wait_time,
                                     int /*block*/)
{
  // If there are no messages then we can go ahead to read from the
  // handle for further reading..

  // The buffer on the stack which will be used to hold the input
  // messages
  char buf [ACE_MAX_DGRAM_SIZE];

#if defined (ACE_HAS_PURIFY)
  (void) ACE_OS::memset (buf,
                         '\0',
                         sizeof buf);
#endif /* ACE_HAS_PURIFY */

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
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l) recv returned error on transport %d after fault %p\n"),
                      this->id (),
                      ACE_TEXT ("handle_input_i ()\n")));
        }

      if (n == -1)
        this->tms_->connection_closed ();

      return n;
    }

  // Set the write pointer in the stack buffer.
  message_block.wr_ptr (n);

  // Parse the incoming message for validity. The check needs to be
  // performed by the messaging objects.
  if (this->parse_incoming_messages (message_block) == -1)
    {
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l) parse_incoming_messages failed on transport %d after fault %p\n"),
                      this->id (),
                      ACE_TEXT ("handle_input_i ()\n")));
        }

      return -1;
    }

  // NOTE: We are not performing any queueing nor any checking for
  // missing data. We are assuming that ALL the data would be got in a
  // single read.

  // Make a node of the message block..
  TAO_Queued_Data qd (&message_block);

  // Extract the data for the node..
  this->messaging_object ()->get_message_data (&qd);

  // Process the message
  return this->process_parsed_messages (&qd, rh);
}

int
TAO_UIPMC_Transport::register_handler_i (void)
{
  // We never register register the handler with the reactor
  // as we never need to be informed about any incoming data,
  // assuming we only use one-ways.
  // If we would register and ICMP Messages would arrive, e.g
  // due to a not reachable server, we would get informed - as this
  // disturbs the general MIOP assumptions of not being
  // interested in any network failures, we ignore ICMP messages.
  return 0;
}

int
TAO_UIPMC_Transport::send_request (TAO_Stub *stub,
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

  return 0;
}

int
TAO_UIPMC_Transport::send_message (TAO_OutputCDR &stream,
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
TAO_UIPMC_Transport::messaging_init (CORBA::Octet major,
                                    CORBA::Octet minor)
{
  this->messaging_object_->init (major,
                                 minor);
  return 1;
}

TAO_Connection_Handler *
TAO_UIPMC_Transport::invalidate_event_handler_i (void)
{
  TAO_Connection_Handler * eh = this->connection_handler_;
  this->connection_handler_ = 0;
  return eh;
}
