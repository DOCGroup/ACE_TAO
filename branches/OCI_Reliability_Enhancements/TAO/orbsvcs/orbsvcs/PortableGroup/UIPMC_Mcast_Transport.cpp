// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Mcast_Transport.h"
#include "orbsvcs/PortableGroup/miopconf.h"
#include "orbsvcs/PortableGroup/UIPMC_Transport_Recv_Packet.h"
#include "orbsvcs/PortableGroup/UIPMC_Mcast_Connection_Handler.h"
#include "orbsvcs/PortableGroup/UIPMC_Wait_Never.h"
#include "orbsvcs/PortableGroup/Fragments_Cleanup_Strategy.h"
#include "orbsvcs/PortableGroup/miop_resource.h"

#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "tao/GIOP_Message_Base.h"
#include "tao/Resume_Handle.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Mcast_Transport::TAO_UIPMC_Mcast_Transport (
  TAO_UIPMC_Mcast_Connection_Handler *handler,
  TAO_ORB_Core *orb_core
)
  : TAO_Transport (IOP::TAG_UIPMC,
                   orb_core)
  , connection_handler_ (handler)
{
  // Replace the default wait strategy with our own
  // since we don't support waiting on anything.
  delete this->ws_;
  ACE_NEW (this->ws_,
           TAO_UIPMC_Wait_Never (this));
}

TAO_UIPMC_Mcast_Transport::~TAO_UIPMC_Mcast_Transport (void)
{
  // Cleanup all packets.
  this->cleanup_packets (false);

  // Remove any unprocessed completed messages
  if (!this->complete_.is_empty ())
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, complete_guard, this->complete_lock_);
      while (!this->complete_.is_empty ())
        {
          TAO_PG::UIPMC_Recv_Packet *packet = 0;
          this->complete_.dequeue_head (packet);
          delete packet;
        }
    }
}

void
TAO_UIPMC_Mcast_Transport::cleanup_packets (bool expired_only)
{
  if (expired_only)
    {
      TAO_MIOP_Resource_Factory *factory =
        ACE_Dynamic_Service<TAO_MIOP_Resource_Factory>::instance (
          this->orb_core_->configuration(),
          ACE_TEXT ("MIOP_Resource_Factory"));

      TAO_PG::Fragments_Cleanup_Strategy *cleanup_strategy =
        factory->fragments_cleanup_strategy ();

      cleanup_strategy->cleanup (this->incomplete_);
    }
  else
    {
      for (Packets_Map::iterator iter = this->incomplete_.begin ();
           !iter.done ();)
        {
          // Move forward iter because what it was pointing to could be
          // unbound at the end of the loop leaving the iterator pointing
          // to removed entry.
          Packets_Map::iterator cur_iter = iter++;

          if (TAO_debug_level >= 8)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                          ACE_TEXT ("cleanup_packets, cleaning %d bytes\n"),
                          this->id (),
                          (*cur_iter).item ()->data_length ()));
            }

          ACE_Auto_Ptr<TAO_PG::UIPMC_Recv_Packet> guard ((*cur_iter).item ());
          this->incomplete_.unbind (cur_iter);
        }
    }
}

ACE_Event_Handler *
TAO_UIPMC_Mcast_Transport::event_handler_i (void)
{
  return this->connection_handler_;
}

TAO_Connection_Handler *
TAO_UIPMC_Mcast_Transport::connection_handler_i (void)
{
  return this->connection_handler_;
}

ssize_t TAO_UIPMC_Mcast_Transport::send (
  iovec *,
  int,
  size_t &,
  ACE_Time_Value const *)
{
  // Write the complete Message_Block chain to the connection.
  // Shouldn't ever be called on the server side.
  ACE_ASSERT (0);
  return -1;
}

ssize_t TAO_UIPMC_Mcast_Transport::recv (
  char *,
  size_t,
  ACE_Time_Value const *)
{
  // Shouldn't ever be called. We use recv_all() with different semantics.
  ACE_ASSERT (0);
  return -1;
}

int TAO_UIPMC_Mcast_Transport::send_request (
  TAO_Stub *,
  TAO_ORB_Core *,
  TAO_OutputCDR &,
  TAO_Message_Semantics,
  ACE_Time_Value *)
{
  // Shouldn't ever be called on the server side.
  ACE_ASSERT (0);
  return -1;
}

int TAO_UIPMC_Mcast_Transport::send_message (
  TAO_OutputCDR &,
  TAO_Stub *,
  TAO_ServerRequest *,
  TAO_Message_Semantics,
  ACE_Time_Value *)
{
  // Shouldn't ever be called on the server side.
  ACE_ASSERT (0);
  return -1;
}

char *
TAO_UIPMC_Mcast_Transport::recv_packet (
  char *buf,
  size_t len,
  ACE_INET_Addr &from_addr,
  CORBA::UShort &packet_length,
  CORBA::ULong &packet_number,
  bool &stop_packet,
  u_long &id_hash) const
{
  // We read the whole MIOP packet which is not longer than MIOP_MAX_DGRAM_SIZE.
  ssize_t const n =
    this->connection_handler_->peer ().recv (buf,
                                             len,
                                             from_addr);

  // There is nothing left in the socket buffer.
  if (n <= 0)
    return 0;

  // Make sure that we at least have a MIOP header.
  if (static_cast<size_t> (n) < MIOP_MIN_HEADER_SIZE)
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                      ACE_TEXT ("recv_packet, packet of size %d is ")
                      ACE_TEXT ("too small\n"),
                      this->id (),
                      n));
        }

      return 0;
    }

  // Use CDR for reading fields from MIOP packet header.
  // buf must be properly aligned.
  TAO_InputCDR miop_hdr (buf, n);

  // Check for MIOP magic bytes.
  CORBA::Octet miop_magic_recv[sizeof miop_magic];
  miop_hdr.read_octet_array (miop_magic_recv, sizeof miop_magic_recv);
  if (miop_magic_recv[0] != miop_magic [0] ||
      miop_magic_recv[1] != miop_magic [1] ||
      miop_magic_recv[2] != miop_magic [2] ||
      miop_magic_recv[3] != miop_magic [3])
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                      ACE_TEXT ("recv_packet, packet didn't contain ")
                      ACE_TEXT ("magic bytes\n"),
                      this->id ()));
        }

      return 0;
    }

  // Check MIOP version.
  CORBA::Octet miop_version;
  miop_hdr.read_octet (miop_version);
  if (miop_version !=
      ((TAO_DEF_MIOP_MAJOR & 0xf) << 4) + (TAO_DEF_MIOP_MINOR & 0xf))
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                      ACE_TEXT ("recv_packet, packet has wrong version ")
                      ACE_TEXT ("%d.%d\n"),
                      this->id (),
                      (miop_version >> 4) & 0xf,
                      miop_version & 0xf));
        }

      return 0;
    }

  CORBA::Octet miop_flags;
  miop_hdr.read_octet (miop_flags);

  // Retrieve the stop packet marker.
  stop_packet = miop_flags & 0x02;

  // Set the byte order.
  // 0 = Big endian
  // 1 = Little endian
  miop_hdr.reset_byte_order (miop_flags & 0x01);

  miop_hdr.read_ushort (packet_length);

  miop_hdr.read_ulong (packet_number);

  // We don't use number_of_packets since it's optional in the spec.
  CORBA::ULong number_of_packets;
  miop_hdr.read_ulong (number_of_packets);

  CORBA::ULong id_length;
  miop_hdr.read_ulong (id_length);

  if (id_length > static_cast<CORBA::ULong> (MIOP_MAX_ID_LENGTH) ||
      static_cast<CORBA::ULong> (MIOP_ID_CONTENT_OFFSET) +
      id_length + packet_length != static_cast<CORBA::ULong> (n))
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                      ACE_TEXT ("recv_packet, malformed packet\n"),
                      this->id ()));
        }

      return 0;
    }

  ssize_t const miop_header_size =
    (MIOP_ID_CONTENT_OFFSET + id_length + 7) & ~0x7;
  if (miop_header_size > n)
    {
      if (TAO_debug_level)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                      ACE_TEXT ("recv_packet, packet not large enough ")
                      ACE_TEXT ("for padding\n"),
                      this->id ()));
        }

      return 0;
    }

  id_hash = ACE::hash_pjw (&buf[MIOP_ID_CONTENT_OFFSET], id_length);

#if 0
  // Normally we don't need to log this but if someone will want. Here we are.
  if (TAO_debug_level >= 10)
    {
      ACE_HEX_DUMP ((LM_DEBUG,
                     (char const *) buf,
                     miop_header_size,
                     ACE_TEXT ("MIOP header")));
    }
#endif

  // Return a pointer to data at 8 byte boundary.
  return &buf[miop_header_size];
}

TAO_PG::UIPMC_Recv_Packet *
TAO_UIPMC_Mcast_Transport::recv_all (TAO_Resume_Handle &rh)
{
  const TAO_MIOP_Resource_Factory *const factory =
    ACE_Dynamic_Service<TAO_MIOP_Resource_Factory>::instance (
      this->orb_core_->configuration(),
      ACE_TEXT ("MIOP_Resource_Factory"));
  const bool eager_dequeue= factory->enable_eager_dequeue ();

  // Only one thread will do recv at the same time.
  // FUZZ: disable check_for_ACE_Guard
  ACE_Guard<TAO_SYNCH_MUTEX> recv_guard (this->recv_lock_, 0); // tryacquire
  // FUZZ: enable check_for_ACE_Guard
  if (recv_guard.locked ())
    {
      // The buffer on the stack which will be used to hold the input
      // messages.
      char buf [MIOP_MAX_DGRAM_SIZE + ACE_CDR::MAX_ALIGNMENT];
      char *aligned_buf = ACE_ptr_align_binary (buf, ACE_CDR::MAX_ALIGNMENT);

#if defined (ACE_INITIALIZE_MEMORY_BEFORE_USE)
      (void) ACE_OS::memset (buf, '\0', sizeof buf);
#endif /* ACE_INITIALIZE_MEMORY_BEFORE_USE */

      while (true)
        {
          // This guard will cleanup expired packets each iteration.
          TAO_PG::UIPMC_Recv_Packet_Cleanup_Guard guard (this);

          ACE_INET_Addr from_addr;
          CORBA::UShort packet_length;
          CORBA::ULong packet_number;
          bool stop_packet;
          u_long id_hash;

          char *start_data =
            this->recv_packet (aligned_buf, MIOP_MAX_DGRAM_SIZE, from_addr,
                               packet_length, packet_number, stop_packet, id_hash);

          // The socket buffer is empty. Try to do other useful things.
          if (start_data == 0)
            {
              if (errno != EWOULDBLOCK && errno != EAGAIN)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                              ACE_TEXT ("recv_all, unexpected failure of recv_packet '%m'\n"),
                              this->id ()));
                }
              break;
            }

          if (TAO_debug_level >= 9)
            {
              char tmp[INET6_ADDRSTRLEN];
              from_addr.get_host_addr (tmp, sizeof tmp);
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                          ACE_TEXT ("recv, received %d bytes from <%C:%u> ")
                          ACE_TEXT ("(hash %d)\n"),
                          this->id (),
                          packet_length,
                          tmp,
                          from_addr.get_port_number (),
                          id_hash));
            }

          TAO_PG::UIPMC_Recv_Packet *packet = 0;
          if (this->incomplete_.find (id_hash, packet) == -1)
            {
              ACE_NEW_THROW_EX (packet,
                                TAO_PG::UIPMC_Recv_Packet,
                                CORBA::NO_MEMORY (
                                  CORBA::SystemException::_tao_minor_code (
                                    TAO::VMCID,
                                    ENOMEM),
                                  CORBA::COMPLETED_NO));

              if (this->incomplete_.bind (id_hash, packet) != 0)
                {
                  // Cleanup the packet.
                  delete packet;
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                              ACE_TEXT ("recv_all, could not queue fragment\n"),
                              this->id ()));
                  continue;
                }
            }

          // We have incomplete packet so add a new data to it.
          // add_fragment returns 1 iff the packet is complete.
          if (1 == packet->add_fragment (start_data, packet_length,
                                         packet_number, stop_packet))
            {
              // Remove this packet from incomplete packets.
              this->incomplete_.unbind (id_hash);

              // If there are no completed message ahead of us AND
              // we only want a single message, just return it.
              if (this->complete_.is_empty () && !eager_dequeue)
                {
                  if (TAO_debug_level >= 9)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                                  ACE_TEXT ("recv_all, completed MIOP message %@\n"),
                                  this->id (), static_cast<void *> (packet)));
                    }

                  return packet;
                }
              ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                                guard,
                                this->complete_lock_,
                                packet);
              if (this->complete_.is_empty () && !eager_dequeue)
                {
                  // Another thread dequeued the waiting MIOP message before we got
                  // the lock, simply return our single message, don't bother queueing
                  // it after all.
                  if (TAO_debug_level >= 9)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                                  ACE_TEXT ("recv_all, completed MIOP message %@\n"),
                                  this->id (), static_cast<void *> (packet)));
                    }

                  return packet;
                }

              if (TAO_debug_level >= 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                              ACE_TEXT ("recv_all, completed MIOP message %@ (QUEUED)\n"),
                              this->id (), static_cast<void *> (packet)));
                }

              // Add it to the complete queue.
              this->complete_.enqueue_tail (packet);

              // Stop attempting to queue more messages if we are not in eager mode.
              if (!eager_dequeue)
                break;
            }
        }
      recv_guard.release ();
    }

  // Ok we have received as many packets as we could, now if we have
  // any completed packets queued up, return the first to the caller.
  if (this->complete_.is_empty ())
    return 0;
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, complete_guard, this->complete_lock_, 0);
  if (this->complete_.is_empty ())
    return 0; // Another thread got here first, not a problem.

  TAO_PG::UIPMC_Recv_Packet *packet = 0;
  if (this->complete_.dequeue_head (packet) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - TAO_UIPMC_Mcast_Transport[%d]::recv_all, ")
                  ACE_TEXT ("unable to dequeue completed message\n"),
                  this->id ()));
      return 0;
    }

  if (TAO_debug_level >= 9)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                  ACE_TEXT ("recv_all, completed MIOP message %@ (DEQUEUED)\n"),
                  this->id (), static_cast<void *> (packet)));
    }

  // If there is another message waiting to be processed (in addition
  // to the one we have just taken off), notify another thread (if
  // available) so this can also be processed in parrellel.
  if (!this->complete_.is_empty ())
    {
      int const retval = this->notify_reactor_now ();
      if (retval == 1)
        {
          // Now we have handed off to another thread, let the class
          // know that it doesn't need to resume with OUR handle
          // after we have processed our message.
          rh.set_flag (TAO_Resume_Handle::TAO_HANDLE_LEAVE_SUSPENDED);
        }
      else if (retval < 0 && TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - TAO_UIPMC_Mcast_Transport[%d]::recv_all, ")
                      ACE_TEXT ("notify to the reactor failed.\n"),
                      this->id ()));
        }
    }

  return packet;
}

int
TAO_UIPMC_Mcast_Transport::handle_input (
  TAO_Resume_Handle &rh,
  ACE_Time_Value *)
{
  // Note: We should not ever return -1 from this function. This will close
  // server connection which we don't want to happen here.

  if (TAO_debug_level >= 8)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                  ACE_TEXT ("handle_input, started\n"),
                  this->id ()));
    }

  // Grab the next completed MIOP message to process from the FIFO Queue.
  ACE_Auto_Ptr<TAO_PG::UIPMC_Recv_Packet> complete_owner (this->recv_all (rh));
  if (TAO_PG::UIPMC_Recv_Packet *complete = complete_owner.get ())
    {
      if (TAO_debug_level >= 9)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                      ACE_TEXT ("handle_input, processing MIOP message %@ (%d bytes)\n"),
                      this->id (), static_cast<void *> (complete), complete->data_length ()));
        }

      // Create a data block from our dequeued completed message.
      char *buffer= 0;
      ACE_NEW_THROW_EX (buffer,
                        char[complete->data_length () + ACE_CDR::MAX_ALIGNMENT],
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_Auto_Array_Ptr<char> owner_buffer (buffer);
      ACE_Data_Block db (complete->data_length () + ACE_CDR::MAX_ALIGNMENT,
                         ACE_Message_Block::MB_DATA,
                         buffer,
                         this->orb_core_->input_cdr_buffer_allocator (),
                         this->orb_core_->locking_strategy (),
                         ACE_Message_Block::DONT_DELETE,
                         this->orb_core_->input_cdr_dblock_allocator ());

      // Create a message block
      ACE_Message_Block message_block (
        &db,
        ACE_Message_Block::DONT_DELETE,
        this->orb_core_->input_cdr_msgblock_allocator ());

      // Align the message block.
      ACE_CDR::mb_align (&message_block);

      complete->copy_data (message_block.wr_ptr ());

      // Set the write pointer in the stack buffer.
      message_block.wr_ptr (complete->data_length ());

      // Make a node of the message block.
      TAO_Queued_Data qd (&message_block);
      size_t mesg_length = 0;

      // Parse the incoming message for validity. The check needs to be
      // performed by the messaging objects.
      if (this->messaging_object ()->parse_next_message (qd, mesg_length) == -1)
        {
          if (TAO_debug_level)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                          ACE_TEXT ("handle_input, failed to parse input\n"),
                          this->id ()));
            }
        }
      else if (qd.missing_data () == TAO_MISSING_DATA_UNDEFINED)
        {
          // Parse/marshal error happened.
          if (TAO_debug_level)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                          ACE_TEXT ("handle_input, got missing data\n"),
                          this->id ()));
            }
        }
      else if (message_block.length () > mesg_length)
        {
          // We read too much data.
          if (TAO_debug_level)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - UIPMC_Mcast_Transport[%d]::")
                          ACE_TEXT ("handle_input, read %d but expected %d\n"),
                          this->id (),
                          message_block.length (),
                          mesg_length));
            }
        }
      else // Process the message.
        (void) this->process_parsed_messages (&qd, rh);
    }

  return 0;
}

int
TAO_UIPMC_Mcast_Transport::register_handler (void)
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

TAO_END_VERSIONED_NAMESPACE_DECL
