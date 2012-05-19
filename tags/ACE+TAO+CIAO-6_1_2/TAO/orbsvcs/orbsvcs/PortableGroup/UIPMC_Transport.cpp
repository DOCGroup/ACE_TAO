// $Id$

#ifndef TAO_UIPMC_TRANSPORT_CPP
#define TAO_UIPMC_TRANSPORT_CPP

#include "orbsvcs/PortableGroup/UIPMC_Profile.h"
#include "orbsvcs/PortableGroup/UIPMC_Transport.h"
#include "orbsvcs/PortableGroup/UIPMC_Message_Block_Data_Iterator.h"
#include "orbsvcs/PortableGroup/UIPMC_Wait_Never.h"

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

// Local MIOP Definitions:

// Note: We currently support packet fragmentation on transmit, but
//       do not support reassembly.

// Limit the number of fragments that we can divide a message
// into.
#define MIOP_MAX_FRAGMENTS    (1)
#define MIOP_MAX_HEADER_SIZE  (272) // See MIOP Spec.  Must be a multiple of 8.
#define MIOP_MAX_DGRAM_SIZE   (ACE_MAX_UDP_PACKET_SIZE)

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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

struct MIOP_Packet
{
  iovec iov[ACE_IOV_MAX];
  int iovcnt;
  int length;
};

template<typename CONNECTION_HANDLER>
TAO_UIPMC_Transport<CONNECTION_HANDLER>::TAO_UIPMC_Transport (
  CONNECTION_HANDLER *handler,
  TAO_ORB_Core *orb_core
)
  : TAO_Transport (IOP::TAG_UIPMC,
                   orb_core,
                   MIOP_MAX_DGRAM_SIZE)
  , connection_handler_ (handler)
{
  // Replace the default wait strategy with our own
  // since we don't support waiting on anything.
  delete this->ws_;
  ACE_NEW (this->ws_,
           TAO_UIPMC_Wait_Never (this));
}

template<typename CONNECTION_HANDLER>
TAO_UIPMC_Transport<CONNECTION_HANDLER>::~TAO_UIPMC_Transport (void)
{
}

template<typename CONNECTION_HANDLER>
ACE_Event_Handler *
TAO_UIPMC_Transport<CONNECTION_HANDLER>::event_handler_i (void)
{
  return this->connection_handler_;
}

template<typename CONNECTION_HANDLER>
TAO_Connection_Handler *
TAO_UIPMC_Transport<CONNECTION_HANDLER>::connection_handler_i (void)
{
  return this->connection_handler_;
}

template<typename CONNECTION_HANDLER>
void
TAO_UIPMC_Transport<CONNECTION_HANDLER>::write_unique_id (TAO_OutputCDR &miop_hdr,
                                                          unsigned long unique)
{
  // We currently construct a unique ID for each MIOP message by
  // concatenating the address of the buffer to a counter.  We may
  // also need to use a MAC address or something more unique to
  // fully comply with the MIOP specification.

  static unsigned long counter = 1;  // Don't worry about race conditions on counter,
                                     // since buffer addresses can't be the same if
                                     // this is being called simultaneously.

  CORBA::Octet unique_id[MIOP_ID_DEFAULT_LENGTH];

  unique_id[0] = static_cast<CORBA::Octet> (unique & 0xff);
  unique_id[1] = static_cast<CORBA::Octet> ((unique & 0xff00) >> 8);
  unique_id[2] = static_cast<CORBA::Octet> ((unique & 0xff0000) >> 16);
  unique_id[3] = static_cast<CORBA::Octet> ((unique & 0xff000000) >> 24);

  unique_id[4] = static_cast<CORBA::Octet> (counter & 0xff);
  unique_id[5] = static_cast<CORBA::Octet> ((counter & 0xff00) >> 8);
  unique_id[6] = static_cast<CORBA::Octet> ((counter & 0xff0000) >> 16);
  unique_id[7] = static_cast<CORBA::Octet> ((counter & 0xff000000) >> 24);

  unique_id[8] = 0;
  unique_id[9] = 0;
  unique_id[10] = 0;
  unique_id[11] = 0;

  miop_hdr.write_ulong (MIOP_ID_DEFAULT_LENGTH);
  miop_hdr.write_octet_array (unique_id, MIOP_ID_DEFAULT_LENGTH);
}

template<typename CONNECTION_HANDLER>
ssize_t
TAO_UIPMC_Transport<CONNECTION_HANDLER>::send (iovec *iov, int iovcnt,
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
  MIOP_Packet *current_fragment = 0;
  int num_fragments = 1;

  UIPMC_Message_Block_Data_Iterator mb_iter (iov, iovcnt);

  // Initialize the first fragment
  current_fragment = &fragments[0];
  current_fragment->iovcnt = 1;  // The MIOP Header
  current_fragment->length = 0;

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
          ++num_fragments;

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
                              ACE_TEXT ("\n\nTAO (%P|%t) - ")
                              ACE_TEXT ("UIPMC_Transport::send ")
                              ACE_TEXT ("Message of size %d needs too many MIOP fragments (max is %d).\n")
                              ACE_TEXT ("You may be able to increase ACE_MAX_DGRAM_SIZE.\n"),
                              bytes_to_send,
                              MIOP_MAX_FRAGMENTS));
                }

              // Pretend it is o.k.  See note by bytes_to_send calculation.
              bytes_transferred = bytes_to_send;
              return 1;
            }

          // Otherwise, initialize another fragment.
          ++current_fragment;
          current_fragment->iovcnt = 1;  // The MIOP Header
          current_fragment->length = 0;
        }
    }

  // Build a generic MIOP Header.

  // Allocate space on the stack for the header (add 8 to account for
  // the possibility of adjusting for alignment).
  char header_buffer[MIOP_HEADER_SIZE + 8];
  TAO_OutputCDR miop_hdr (header_buffer, MIOP_HEADER_SIZE + 8);

  miop_hdr.write_octet_array (miop_magic, 4);   // Magic
  miop_hdr.write_octet (0x10);                  // Version
  CORBA::Octet *flags_field = reinterpret_cast<CORBA::Octet *> (miop_hdr.current ()->wr_ptr ());

  // Write flags octet:
  //  Bit        Description
  //   0         Endian
  //   1         Stop message flag (Assigned later)
  //   2 - 7     Set to 0
  miop_hdr.write_octet (TAO_ENCAP_BYTE_ORDER);  // Flags

  // Packet Length
  // NOTE: We can save pointers and write them later without byte swapping since
  //       in CORBA, the sender chooses the endian.
  CORBA::UShort *packet_length = reinterpret_cast<CORBA::UShort *> (miop_hdr.current ()->wr_ptr ());
  miop_hdr.write_short (0);

  // Packet number
  CORBA::ULong *packet_number = reinterpret_cast<CORBA::ULong *> (miop_hdr.current ()->wr_ptr ());
  miop_hdr.write_ulong (0);

  // Number of packets field
  miop_hdr.write_ulong (num_fragments);

  // UniqueId
  ptrdiff_t unique_id = reinterpret_cast<ptrdiff_t> (iov);
  this->write_unique_id (miop_hdr,
                         static_cast<unsigned long> (unique_id));

  // Send the buffers.
  current_fragment = &fragments[0];
  while (num_fragments > 0 &&
         current_fragment->iovcnt > 1)
    {
      // Fill in the packet length header field.
      *packet_length = static_cast<CORBA::UShort> (current_fragment->length);

      // If this is the last fragment, set the stop message flag.
      if (num_fragments == 1)
        {
          *flags_field |= 0x02;
        }

      // Setup the MIOP header in the iov list.
      current_fragment->iov[0].iov_base = miop_hdr.current ()->rd_ptr ();
      current_fragment->iov[0].iov_len = MIOP_HEADER_SIZE;

      // Send the fragment. - Need to check for errors!!
      ssize_t rc = this->connection_handler_->send (current_fragment->iov,
                                                    current_fragment->iovcnt,
                                                    addr);

      if (rc <= 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("\n\nTAO (%P|%t) - ")
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
                      "TAO_UIPMC_Transport::send: sent %d bytes to %s:%d\n",
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

template<typename CONNECTION_HANDLER>
ssize_t
TAO_UIPMC_Transport<CONNECTION_HANDLER>::recv (char *buf,
                                               size_t len,
                                               const ACE_Time_Value * /*max_wait_time*/)
{
  ACE_INET_Addr from_addr;

  ssize_t n = this->connection_handler_->peer ().recv (buf,
                                                       len,
                                                       from_addr);
  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO_UIPMC_Transport::recv: received %d bytes from %s:%d\n",
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
                      "TAO_UIPMC_Transport::recv: packet of size %d is too small from %s:%d\n",
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
                      "TAO_UIPMC_Transport::recv: UIPMC packet didn't contain magic bytes.\n"));
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
      id_length = *reinterpret_cast<ACE_CDR::ULong*> (&buf[MIOP_ID_LENGTH_OFFSET]);
    }
  else
    {
      ACE_CDR::swap_4 (&buf[MIOP_ID_LENGTH_OFFSET],
                       reinterpret_cast<char*> (&id_length));
    }
#else
  id_length = *reinterpret_cast<ACE_CDR::ULong*> (&buf[MIOP_ID_LENGTH_OFFSET]);
#endif /* ACE_DISABLE_SWAP_ON_READ */

  // Make sure that the length field is legal.
  if (id_length > MIOP_MAX_LENGTH_ID ||
      static_cast<ssize_t> (MIOP_ID_CONTENT_OFFSET + id_length) > n)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_UIPMC_Transport::recv: Invalid ID length.\n"));
        }

      return 0;
    }

  // Trim off the header for now.
  ssize_t const miop_header_size = (MIOP_ID_CONTENT_OFFSET + id_length + 7) & ~0x7;
  if (miop_header_size > n)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO_UIPMC_Transport::recv: MIOP packet not large enough for padding.\n"));
        }

      return 0;
    }

  n -= miop_header_size;
  ACE_OS::memmove (buf, buf + miop_header_size, n);

  return n;
}

template<typename CONNECTION_HANDLER>
int
TAO_UIPMC_Transport<CONNECTION_HANDLER>::handle_input (TAO_Resume_Handle &rh,
                                                       ACE_Time_Value *max_wait_time)
{
  // If there are no messages then we can go ahead to read from the
  // handle for further reading..

  // The buffer on the stack which will be used to hold the input
  // messages
  char buf [MIOP_MAX_DGRAM_SIZE];

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
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l) recv returned error on transport %d after fault %p\n"),
                      this->id (),
                      ACE_TEXT ("handle_input ()\n")));
        }

      if (n == -1)
        this->tms_->connection_closed ();

      return n;
    }

  // Set the write pointer in the stack buffer.
  message_block.wr_ptr (n);

  // Make a node of the message block..
  TAO_Queued_Data qd (&message_block);
  size_t mesg_length = 0;

  // Parse the incoming message for validity. The check needs to be
  // performed by the messaging objects.
  if (this->messaging_object ()->parse_next_message (qd, mesg_length) == -1)
    {
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l) handle_input failed on transport %d after fault\n"),
                      this->id () ));
        }

      return -1;
    }

  if (message_block.length () > mesg_length)
    {
      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l) handle_input  failed on transport %d after fault\n"),
                      this->id () ));
        }

      return -1;
    }

  // NOTE: We are not performing any queueing nor any checking for
  // missing data. We are assuming that ALL the data would be got in a
  // single read.

  // Process the message
  return this->process_parsed_messages (&qd, rh);
}

template<typename CONNECTION_HANDLER>
int
TAO_UIPMC_Transport<CONNECTION_HANDLER>::register_handler (void)
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

template<typename CONNECTION_HANDLER>
int
TAO_UIPMC_Transport<CONNECTION_HANDLER>::send_request (TAO_Stub *stub,
                                                       TAO_ORB_Core *orb_core,
                                                       TAO_OutputCDR &stream,
                                                       TAO_Message_Semantics message_semantics,
                                                       ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  message_semantics) == -1)
    return -1;

  if (this->send_message (stream,
                          stub,
                          0,
                          message_semantics,
                          max_wait_time) == -1)

    return -1;

  return 0;
}

template<typename CONNECTION_HANDLER>
int
TAO_UIPMC_Transport<CONNECTION_HANDLER>::send_message (TAO_OutputCDR &stream,
                                                       TAO_Stub *stub,
                                                       TAO_ServerRequest *request,
                                                       TAO_Message_Semantics message_semantics,
                                                       ACE_Time_Value *max_wait_time)
{
  // Format the message in the stream first
  if (this->messaging_object ()->format_message (stream, stub, request) != 0)
    {
      return -1;
    }

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
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO: (%P|%t|%N|%l) closing transport %d after fault %m\n"),
                      this->id (),
                      ACE_TEXT ("send_message ()\n")));
        }

      return -1;
    }

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_UIPMC_TRANSPORT_CPP */
