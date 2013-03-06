// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Transport.h"
#include "orbsvcs/PortableGroup/miopconf.h"
#include "orbsvcs/PortableGroup/UIPMC_Connection_Handler.h"
#include "orbsvcs/PortableGroup/UIPMC_Message_Block_Data_Iterator.h"
#include "orbsvcs/PortableGroup/UIPMC_Wait_Never.h"
#include "orbsvcs/PortableGroup/miop_resource.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"

#include "ace/UUID.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Transport::TAO_UIPMC_Transport (
  TAO_UIPMC_Connection_Handler *handler,
  TAO_ORB_Core *orb_core
)
  : TAO_Transport (IOP::TAG_UIPMC, orb_core)
  , connection_handler_ (handler)
  , total_bytes_outstanding_ (0u)
  , time_last_sent_ (ACE_Time_Value::zero)
{
  // Replace the default wait strategy with our own
  // since we don't support waiting on anything.
  delete this->ws_;
  ACE_NEW (this->ws_, TAO_UIPMC_Wait_Never (this));

  ACE_Utils::UUID uuid;
  ACE_Utils::UUID_GENERATOR::instance ()->generate_UUID (uuid);

  // This ID is "globally" unique.
  this->uuid_hash_ = uuid.to_string ()->hash ();
}

TAO_UIPMC_Transport::~TAO_UIPMC_Transport (void)
{
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

bool
TAO_UIPMC_Transport::write_unique_id (
  TAO_OutputCDR &miop_hdr) const
{
  // This is unique within single machine.
  unsigned long pid = static_cast<unsigned long> (ACE_OS::getpid ());

  // This is unique within single process.
  static ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> cnt = 0;
  unsigned long const id = ++cnt;

  CORBA::Octet unique_id[MIOP_DEFAULT_ID_LENGTH];

#define UIPMC_OCTET(value) static_cast<CORBA::Octet> ((value) & 0xff)

  // Use the lowest 4 bytes. In case of IPv6 it gives more diversity.
  unique_id[0]  = UIPMC_OCTET (this->uuid_hash_);
  unique_id[1]  = UIPMC_OCTET (this->uuid_hash_ >> 8);
  unique_id[2]  = UIPMC_OCTET (this->uuid_hash_ >> 16);
  unique_id[3]  = UIPMC_OCTET (this->uuid_hash_ >> 24);

  unique_id[4]  = UIPMC_OCTET (pid);
  unique_id[5]  = UIPMC_OCTET (pid >> 8);
  unique_id[6]  = UIPMC_OCTET (pid >> 16);
  unique_id[7]  = UIPMC_OCTET (pid >> 24);

  unique_id[8]  = UIPMC_OCTET (id);
  unique_id[9]  = UIPMC_OCTET (id >> 8);
  unique_id[10] = UIPMC_OCTET (id >> 16);
  unique_id[11] = UIPMC_OCTET (id >> 24);

  miop_hdr.write_ulong (MIOP_DEFAULT_ID_LENGTH);
  miop_hdr.write_octet_array (unique_id, MIOP_DEFAULT_ID_LENGTH);

  return miop_hdr.good_bit ();
}

void
TAO_UIPMC_Transport::throttle_send_rate (
  ACE_UINT64 const max_fragment_rate,
  u_long const max_fragment_size,
  u_long const this_fragment_size)
{
  ACE_Time_Value const now = ACE_OS::gettimeofday ();
  if (this->total_bytes_outstanding_)
    {
      // How much of the previously sent data have we had time to send?
      ACE_Time_Value const how_long = now - this->time_last_sent_;
      ACE_UINT64 how_long_in_micro_seconds = 0u;
      how_long.to_usec (how_long_in_micro_seconds);

      ACE_UINT64 const octets_processed =
        how_long_in_micro_seconds * max_fragment_size / max_fragment_rate;
      if (this->total_bytes_outstanding_ <= octets_processed)
        {
          if (2 <= TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::")
                        ACE_TEXT ("throttle_send_rate, Previous data ")
                        ACE_TEXT ("(%u bytes) has cleared ")
                        ACE_TEXT ("(could have sent %Q bytes over ")
                        ACE_TEXT ("the last %Q uSecs)\n"),
                        this->id (),
                        this->total_bytes_outstanding_,
                        octets_processed,
                        how_long_in_micro_seconds));
          this->total_bytes_outstanding_ = 0u;
        }
      else
        {
          if (2 <= TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::")
                        ACE_TEXT ("throttle_send_rate, Previous data ")
                        ACE_TEXT ("(%u bytes) has reduced by %Q bytes ")
                        ACE_TEXT ("over the last %Q uSecs\n"),
                        this->id (),
                        this->total_bytes_outstanding_,
                        octets_processed,
                        how_long_in_micro_seconds));
          this->total_bytes_outstanding_ -=
            static_cast<u_long> (octets_processed);
        }
    }
  this->time_last_sent_ = now;

  if (this->total_bytes_outstanding_)
    {
      // Is this fragment enough to exceed the capacity of the client
      // and/or server system socket buffers?
      u_long const
        new_total_send = this->total_bytes_outstanding_ + this_fragment_size,
        hwm = this->connection_handler_->send_hi_water_mark ();
      if (new_total_send > hwm)
        {
          // Definatly need to throttle back and delay before sending
          // this new packet. How much extra data are we trying to fit
          // into the various socket buffers?
          u_long bytes_over_by = new_total_send - hwm;

          // We need to wait until existing "bytes_over_by" data has been
          // transmitted/processed; however since we only have
          // "total_bytes_outstanding" actualy in progress at the moment,
          // this is the maximum amount we can wait for. (This can happen
          // if the High Water Mark is set less than the "max_fragment_size"
          // and means we can initialy exceed the HWM; will simply wait
          // longer before the next fragment, after this current one.)
          if (bytes_over_by > this->total_bytes_outstanding_)
            bytes_over_by = this->total_bytes_outstanding_;

          // How much time does it take for "bytes_over_by" data to be
          // processed?
          ACE_UINT64 const delay_in_micro_seconds=
            bytes_over_by * max_fragment_rate / max_fragment_size;
          ACE_Time_Value const delay (
            static_cast <time_t>      (delay_in_micro_seconds / ACE_ONE_SECOND_IN_USECS),
            static_cast <suseconds_t> (delay_in_micro_seconds % ACE_ONE_SECOND_IN_USECS));

          if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::")
                        ACE_TEXT ("throttle_send_rate, SendHighWaterMark ")
                        ACE_TEXT ("(%u) exceeded by %u bytes, delaying ")
                        ACE_TEXT ("for %Q uSecs\n"),
                        this->id (),
                        hwm,
                        bytes_over_by,
                        delay_in_micro_seconds));

          ACE_OS::sleep (delay);
        }
    }
}

ssize_t
TAO_UIPMC_Transport::send (
  iovec *iov,
  int iovcnt,
  size_t &bytes_transferred,
  const ACE_Time_Value *)
{
  bytes_transferred = 0u;

  // Calculate the total number of bytes being sent.
  u_long bytes_to_send = 0u;
  for (int i = 0; i < iovcnt; ++i)
    bytes_to_send += iov[i].iov_len;
  if (!bytes_to_send)
    return 0; // Nothing to send, we are done.

  // Determine the number of MIOP packets that we have to send (add one
  // if "bytes_to_send" is not a multiple of "max_fragment_payload").
  TAO_MIOP_Resource_Factory *const factory =
    ACE_Dynamic_Service<TAO_MIOP_Resource_Factory>::instance (
      this->orb_core_->configuration(),
      ACE_TEXT ("MIOP_Resource_Factory"));
  u_long const
    max_fragment_size= factory->max_fragment_size (),
    max_fragment_payload= max_fragment_size - MIOP_DEFAULT_HEADER_SIZE,
    number_of_packets_required =
         bytes_to_send / max_fragment_payload +
      !!(bytes_to_send % max_fragment_payload);
  if (factory->max_fragments () && // Any value except Zero (Unlimited)
      factory->max_fragments () < number_of_packets_required)
    {
      // Pretend it was sent ok, but we are dropping this message,
      // it is too large to send.
      bytes_transferred = bytes_to_send;

      if (TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::send, ")
                      ACE_TEXT ("Did not send MIOP message of size %u ")
                      ACE_TEXT ("(it was too large, needing %u fragments).\n")
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::send, ")
                      ACE_TEXT ("You maybe able to increase ")
                      ACE_TEXT ("MIOP_Resource_Factory -ORBMaxFragments %u"),
                      this->id (),
                      bytes_to_send,
                      number_of_packets_required,
                      this->id (),
                      factory->max_fragments ()));

          if (max_fragment_size < MIOP_MAX_DGRAM_SIZE)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT (" or -ORBMaxFragmentSize %u\n"),
                        max_fragment_size));
          else
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
        }

      return bytes_transferred;
    }

  // Build a generic MIOP Header on the stack for this
  // "number_of_packets_required". NOTE: We can save
  // pointers and write to them later without byte swapping
  // since in CORBA, the sender chooses the endianess.
  char header_buffer[MIOP_DEFAULT_HEADER_SIZE + ACE_CDR::MAX_ALIGNMENT];
  TAO_OutputCDR miop_hdr (header_buffer, sizeof header_buffer);
  miop_hdr.write_octet_array (miop_magic, sizeof miop_magic);
  // MIOP (Not GIOP) Version
  miop_hdr.write_octet (((TAO_DEF_MIOP_MAJOR & 0xf) << 4) +
                         (TAO_DEF_MIOP_MINOR & 0xf));
  // Write flags octet:
  // Bit  Description
  //  0   Endian
  //  1   Stop message flag (Assigned later)
  // 2-7  Set to 0
  CORBA::Octet *flags_field =
    reinterpret_cast<CORBA::Octet *> (miop_hdr.current ()->wr_ptr ());
  miop_hdr.write_octet (TAO_ENCAP_BYTE_ORDER);
  CORBA::UShort *packet_length =
    reinterpret_cast<CORBA::UShort *> (miop_hdr.current ()->wr_ptr ());
  miop_hdr.write_short (0);
  CORBA::ULong *packet_number =
    reinterpret_cast<CORBA::ULong *> (miop_hdr.current ()->wr_ptr ());
  miop_hdr.write_ulong (0u);
  miop_hdr.write_ulong (number_of_packets_required);
  if (!this->write_unique_id (miop_hdr))
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::send, ")
                  ACE_TEXT ("error creating fragment MIOP header.\n"),
                  this->id ()));
      return -1;
    }

  // Attempt to partition up the payload data sending each of the MIOP fragments
  iovec this_fragment_iov[ACE_IOV_MAX];
  UIPMC_Message_Block_Data_Iterator mb_iter (iov, iovcnt);
  ACE_INET_Addr const &addr = this->connection_handler_->addr ();
  for (*packet_number= 0u;
       *packet_number < number_of_packets_required;
       ++*packet_number)
    {
      // The first iov for each packet points at the static MIOP header.
      this_fragment_iov[0].iov_base = miop_hdr.current ()->rd_ptr ();
      this_fragment_iov[0].iov_len  = MIOP_DEFAULT_HEADER_SIZE;
      int this_fragment_iovcnt = 1;   // Just the MIOP Header so far!
      u_long this_fragment_size= 0uL; // Just the payload data length (for now)

      // Obtain the next fragment's payload data.
      while (mb_iter.next_block (max_fragment_payload - this_fragment_size,
                                 this_fragment_iov[this_fragment_iovcnt]))
        {
          // Increment the fragments length and iovcnt.
          this_fragment_size +=
            this_fragment_iov[this_fragment_iovcnt++].iov_len;

          // Check if we have maxed out this fragment's payload.
          if (this_fragment_size == max_fragment_payload)
            break;

          // Just a safety check for building iovec.
          if (this_fragment_iovcnt >= ACE_IOV_MAX)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::send, ")
                          ACE_TEXT ("Too many iovec to create fragment.\n"),
                          this->id ()));
              return -1;
            }
        } // While fragment is not complete

      // Now we have the payload length for this fragment, update the MIOP header
      *packet_length = static_cast<CORBA::UShort> (this_fragment_size);
      if (*packet_number == number_of_packets_required-1uL)
        *flags_field |= 0x02;

      ssize_t already_sent = 0; // No data sent yet!
      iovec *current_iov= this_fragment_iov;
      for (this_fragment_size+= MIOP_DEFAULT_HEADER_SIZE; // Now includes MIOP header
           this_fragment_size; // Still any data to send
           this_fragment_size-= static_cast<u_long> (already_sent))
        {
          // Make sure we don't send our fragments too quickly
          if (factory->enable_throttling ())
            this->throttle_send_rate (
              factory->max_fragment_rate (),
              max_fragment_size,
              this_fragment_size);

          // Haven't sent some of the data yet, we need to adjust the fragments iov's
          // to skip the data we have actually manage to send so far.
          while (already_sent)
            if (static_cast<u_long> (current_iov->iov_len) <= static_cast<u_long> (already_sent))
              {
                // This whole iov has been sent, simply skip over it
                already_sent-= current_iov->iov_len;
                --this_fragment_iovcnt;
                ++current_iov;
              }
            else
              {
                // This iov has been partially sent, adjust it's data
                // to skip over those bytes already transmitted.
                current_iov->iov_len -= static_cast<u_long> (already_sent);
                current_iov->iov_base =
                  &static_cast<char *> (current_iov->iov_base)[already_sent];
                break; // already_sent= 0;
              }

          // Ok now we attempt to actually send the fragment.
          already_sent =
            this->connection_handler_->peer ().send (
              current_iov,
              this_fragment_iovcnt,
              addr);
          if (already_sent < 0)
            {
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::send, ")
                          ACE_TEXT ("error sending data (Errno: '%m')\n"),
                          this->id ()));
              return -1;
            }
          else if (TAO_debug_level &&
                   static_cast<u_long> (already_sent) != this_fragment_size)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::send, ")
                          ACE_TEXT ("Partial fragment (%B/%u bytes), ")
                          ACE_TEXT ("reattempting remainder.\n"),
                          this->id (),
                          already_sent,
                          this_fragment_size));
            }

          // Keep a note of the number of bytes we have just buffered
          if (factory->enable_throttling ())
            this->total_bytes_outstanding_+= static_cast<u_long> (already_sent);
        } // Keep sending the rest of the fragment

      // Increment the number of bytes of payload transferred.
      bytes_transferred += *packet_length;

      if (9 <= TAO_debug_level)
        {
          char tmp[INET6_ADDRSTRLEN];
          addr.get_host_addr (tmp, sizeof tmp);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Transport[%d]::send, ")
                      ACE_TEXT ("Sent %u bytes payload (fragment %u/%u) to <%C:%u>\n"),
                      this->id (),
                      *packet_length,
                      *packet_number + 1uL,
                      number_of_packets_required,
                      tmp,
                      addr.get_port_number ()));
        }
    } // Send next fragment

  // Return total bytes transferred.
  return bytes_transferred;
}

ssize_t
TAO_UIPMC_Transport::recv (
  char *,
  size_t,
  const ACE_Time_Value *)
{
  // Shouldn't ever be called on the client side.
  ACE_ASSERT (0);
  return -1;
}

int
TAO_UIPMC_Transport::register_handler (void)
{
  // We never register the handler with the reactor
  // as we never need to be informed about any incoming data,
  // assuming we only use one-ways.
  // If we would register and ICMP Messages would arrive, e.g
  // due to a not reachable server, we would get informed - as this
  // disturbs the general MIOP assumptions of not being
  // interested in any network failures, we ignore ICMP messages.
  return 0;
}

int
TAO_UIPMC_Transport::send_request (
  TAO_Stub *stub,
  TAO_ORB_Core *orb_core,
  TAO_OutputCDR &stream,
  TAO_Message_Semantics message_semantics,
  ACE_Time_Value *max_wait_time)
{
  if (this->ws_->sending_request (orb_core,
                                  message_semantics) == -1 ||
      this->send_message (stream,
                          stub,
                          0,
                          message_semantics,
                          max_wait_time) == -1)
    {
      return -1;
    }

  this->first_request_sent();
  return 0;
}

int
TAO_UIPMC_Transport::send_message (
  TAO_OutputCDR &stream,
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
  ssize_t const n = this->send_message_shared (stub,
                                               message_semantics,
                                               stream.begin (),
                                               max_wait_time);

  if (n == -1)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_ERROR,
                    ACE_TEXT ("TAO: (%P|%t) - UIPMC_Transport[%d]::")
                    ACE_TEXT ("send_message, closing transport %d after ")
                    ACE_TEXT ("fault (Errno: '%m')\n"),
                    this->id ()));
      return -1;
    }

  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
