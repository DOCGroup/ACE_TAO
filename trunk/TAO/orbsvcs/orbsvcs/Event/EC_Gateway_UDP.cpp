// $Id$

#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"

#if !defined(__ACE_INLINE__)
#include "EC_Gateway_UDP.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Gateway_UDP, "$Id$")

// ****************************************************************

TAO_ECG_UDP_Sender::TAO_ECG_UDP_Sender (void)
  :  mtu_ (TAO_ECG_UDP_Sender::ECG_DEFAULT_MTU)
{
}

int
TAO_ECG_UDP_Sender::get_local_addr (ACE_INET_Addr& addr)
{
  if (this->endpoint_ == 0)
    return -1;
  return this->dgram ().get_local_addr (addr);
}

void
TAO_ECG_UDP_Sender::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
                          RtecUDPAdmin::AddrServer_ptr addr_server,
                          TAO_ECG_UDP_Out_Endpoint* endpoint,
                          CORBA::Environment &TAO_IN_ENV)
{
  this->lcl_ec_ =
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->addr_server_ =
    RtecUDPAdmin::AddrServer::_duplicate (addr_server);

  this->endpoint_ = endpoint;
}

int
TAO_ECG_UDP_Sender::mtu (CORBA::ULong new_mtu)
{
  if (new_mtu < TAO_ECG_UDP_Sender::ECG_MIN_MTU
      || new_mtu >= TAO_ECG_UDP_Sender::ECG_MAX_MTU)
    return -1;
  this->mtu_ = new_mtu;
  return 0;
}

void
TAO_ECG_UDP_Sender::shutdown (CORBA::Environment& TAO_IN_ENV)
{
  this->close (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;
  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();
}

void
TAO_ECG_UDP_Sender::open (RtecEventChannelAdmin::ConsumerQOS& sub,
                          CORBA::Environment &TAO_IN_ENV)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Open gateway\n"));
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    return;

  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    this->close (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  if (sub.dependencies.length () == 0)
    return;

  //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Supplier "));
  //ACE_SupplierQOS_Factory::debug (pub);

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->lcl_ec_->for_consumers (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  RtecEventComm::PushConsumer_var consumer_ref =
    this->_this (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Consumer "));
  //ACE_ConsumerQOS_Factory::debug (sub);

  this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                sub,
                                                TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;
}

void
TAO_ECG_UDP_Sender::close (CORBA::Environment &env)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (env);
  if (env.exception () != 0) return;
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
TAO_ECG_UDP_Sender::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier-consumer received "
              "disconnect from channel.\n"));
}

void
TAO_ECG_UDP_Sender::push (const RtecEventComm::EventSet &events,
                          CORBA::Environment & TAO_IN_ENV)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG_UDP_Sender::push - \n"));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  // @@ TODO precompute this when the MTU changes...
  CORBA::ULong max_fragment_payload = this->mtu () -
    TAO_ECG_UDP_Sender::ECG_HEADER_SIZE;
  // ACE_ASSERT (max_fragment_payload != 0);

  // ACE_DEBUG ((LM_DEBUG, "%d event(s) - ", events.length ()));

  // Send each event in a separate message.
  // @@ TODO It is interesting to group events destined to the
  // same mcast group in a single message.
  for (u_int i = 0; i < events.length (); ++i)
    {
      // To avoid loops we keep a TTL field on the events and skip the
      // events with TTL <= 0
      if (events[i].header.ttl <= 0)
        continue;

      const RtecEventComm::Event& e = events[i];

      // We need to modify the TTL field, but copying the entire event
      // would be wasteful; instead we create a new header and only
      // modify the header portion.
      RtecEventComm::EventHeader header = e.header;
      header.ttl--;

      // Grab the right mcast group for this event...
      RtecUDPAdmin::UDP_Addr udp_addr;
      this->addr_server_->get_addr (header, udp_addr, TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);

      // Start building the message
      TAO_OutputCDR cdr;

      // Marshal as if it was a sequence of one element, notice how we
      // marshal a modified version of the header, but the payload is
      // marshal without any extra copies.
      cdr.write_ulong (1);
      cdr.encode (RtecEventComm::_tc_EventHeader, &header, 0, TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);

      cdr.encode (RtecEventComm::_tc_EventData, &e.data, 0, TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);

      const int TAO_WRITEV_MAX = IOV_MAX;
      iovec iov[TAO_WRITEV_MAX];

      CORBA::ULong total_length;
      CORBA::ULong fragment_count =
        this->compute_fragment_count (cdr.begin (),
                                      cdr.end (),
                                      TAO_WRITEV_MAX,
                                      max_fragment_payload,
                                      total_length);

      CORBA::ULong request_id = this->endpoint_->next_request_id ();

      // Reserve the first iovec for the header...
      int iovcnt = 1;
      CORBA::ULong fragment_id = 0;
      CORBA::ULong fragment_offset = 0;
      CORBA::ULong fragment_size = 0;
      for (const ACE_Message_Block* b = cdr.begin ();
           b != cdr.end ();
           b = b->cont ())
        {
          CORBA::ULong l = b->length ();

          iov[iovcnt].iov_base = b->rd_ptr ();
          iov[iovcnt].iov_len  = l;
          fragment_size += l;
          iovcnt++;
          while (fragment_size > max_fragment_payload)
            {
              // This fragment is full, we have to send it...

              // First adjust the last iov entry:
              CORBA::ULong last_mb_length =
                max_fragment_payload - (fragment_size - l);
              iov[iovcnt - 1].iov_len = last_mb_length;

              this->send_fragment (udp_addr,
                                   request_id,
                                   total_length,
                                   max_fragment_payload,
                                   fragment_offset,
                                   fragment_id,
                                   fragment_count,
                                   iov,
                                   iovcnt,
                                   TAO_IN_ENV);
              TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);
              fragment_id++;
              fragment_offset += max_fragment_payload;

              // Reset, but don't forget that the last Message_Block
              // may need to be sent in multiple fragments..
              l -= last_mb_length;
              iov[1].iov_base = b->rd_ptr () + last_mb_length;
              iov[1].iov_len = l;
              fragment_size = l;
              iovcnt = 2;
            }
          if (fragment_size == max_fragment_payload)
            {
              // We filled a fragment, but this time it was filled
              // exactly, the treatment is a little different from the
              // loop above...
              this->send_fragment (udp_addr,
                                   request_id,
                                   total_length,
                                   max_fragment_payload,
                                   fragment_offset,
                                   fragment_id,
                                   fragment_count,
                                   iov,
                                   iovcnt,
                                   TAO_IN_ENV);
              TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);
              fragment_id++;
              fragment_offset += max_fragment_payload;

              iovcnt = 1;
              fragment_size = 0;
            }
          if (iovcnt == TAO_WRITEV_MAX)
            {
              // Now we ran out of space in the iovec, we must send a
              // fragment to work around that....
              this->send_fragment (udp_addr,
                                   request_id,
                                   total_length,
                                   fragment_size,
                                   fragment_offset,
                                   fragment_id,
                                   fragment_count,
                                   iov,
                                   iovcnt,
                                   TAO_IN_ENV);
              TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);
              fragment_id++;
              fragment_offset += fragment_size;

              iovcnt = 1;
              fragment_size = 0;
           }
        }
      // There is something left in the iovvec that we must send
      // also...
      if (iovcnt != 1)
        {
          // Now we ran out of space in the iovec, we must send a
          // fragment to work around that....
          this->send_fragment (udp_addr,
                               request_id,
                               total_length,
                               fragment_size,
                               fragment_offset,
                               fragment_id,
                               fragment_count,
                               iov,
                               iovcnt,
                               TAO_IN_ENV);
          TAO_CHECK_ENV_RETURN_VOID(TAO_IN_ENV);
          fragment_id++;
          fragment_offset += fragment_size;

          // reset, not needed here...
          // iovcnt = 1;
          // fragment_size = 0;
        }
      // ACE_ASSERT (total_length == fragment_offset);
      // ACE_ASSERT (fragment_id == fragment_count);

    }
}

void
TAO_ECG_UDP_Sender::send_fragment (const RtecUDPAdmin::UDP_Addr& udp_addr,
                                   CORBA::ULong request_id,
                                   CORBA::ULong request_size,
                                   CORBA::ULong fragment_size,
                                   CORBA::ULong fragment_offset,
                                   CORBA::ULong fragment_id,
                                   CORBA::ULong fragment_count,
                                   iovec iov[],
                                   int iovcnt,
                                   CORBA::Environment& TAO_IN_ENV)
{
  CORBA::ULong header[TAO_ECG_UDP_Sender::ECG_HEADER_SIZE
                     / sizeof(CORBA::ULong)
                     + ACE_CDR::MAX_ALIGNMENT];
  char* buf = ACE_reinterpret_cast(char*,header);
  TAO_OutputCDR cdr (buf, sizeof(header));
  cdr.write_boolean (TAO_ENCAP_BYTE_ORDER);
  cdr.write_ulong (request_id);
  cdr.write_ulong (request_size);
  cdr.write_ulong (fragment_size);
  cdr.write_ulong (fragment_offset);
  cdr.write_ulong (fragment_id);
  cdr.write_ulong (fragment_count);
  CORBA::Octet padding[4];
  cdr.write_octet_array (padding, 4);

  iov[0].iov_base = cdr.begin ()->rd_ptr ();
  iov[0].iov_len  = cdr.begin ()->length ();

  ACE_INET_Addr inet_addr (udp_addr.port,
                           udp_addr.ipaddr);

  //  ACE_DEBUG ((LM_DEBUG,
  //              "ECG_UDP_Sender (%P|%t): msg = %d, fragment %d/%d, "
  //               "dest = (%u:%d)\n",
  //              request_id,
  //              fragment_id, fragment_count,
  //              udp_addr.ipaddr, udp_addr.port));

  ssize_t n = this->dgram ().send (iov,
                                   iovcnt,
                                   inet_addr);
  if (n == -1)
    {
      // @@ TODO Use a Event Channel specific exception
      ACE_DEBUG ((LM_DEBUG,
                  "ECG_UDP (%t) send failed %p\n", ""));
      TAO_THROW(CORBA::COMM_FAILURE ());
    }
  else if (n == 0)
    {
      // @@ TODO Use a Event Channel specific exception
      ACE_DEBUG ((LM_DEBUG,
                  "ECG_UDP (%t) EOF on send \n"));
      TAO_THROW(CORBA::COMM_FAILURE ());
    }
}


CORBA::ULong
TAO_ECG_UDP_Sender::compute_fragment_count (const ACE_Message_Block* begin,
                                            const ACE_Message_Block* end,
                                            int iov_size,
                                            CORBA::ULong max_fragment_payload,
                                            CORBA::ULong& total_length)
{
  CORBA::ULong fragment_count = 0;
  total_length = 0;

  CORBA::ULong fragment_size = 0;
  // Reserve the first iovec for the header...
  int iovcnt = 1;
  for (const ACE_Message_Block* b = begin;
       b != end;
       b = b->cont ())
    {
      CORBA::ULong l = b->length ();
      total_length += l;
      fragment_size += l;
      iovcnt++;
      while (fragment_size > max_fragment_payload)
        {
          // Ran out of space, must create a fragment...
          fragment_count++;

          // The next iovector will contain what remains of this
          // buffer, but also consider
          iovcnt = 2;
          l -= max_fragment_payload - (fragment_size - l);
          fragment_size = l;
        }
      if (fragment_size == max_fragment_payload)
        {
          fragment_count++;
          iovcnt = 1;
          fragment_size = 0;
        }
      if (iovcnt >= iov_size)
        {
          // Ran out of space in the iovector....
          fragment_count++;
          iovcnt = 1;
          fragment_size = 0;
        }
    }
  if (iovcnt != 1)
    {
      // Send the remaining data in another fragment
      fragment_count++;
    }
  return fragment_count;
}

// ****************************************************************

TAO_ECG_UDP_Out_Endpoint::~TAO_ECG_UDP_Out_Endpoint (void)
{
  delete[] this->ifs_;
  this->ifs_ = 0;
}

CORBA::Boolean
TAO_ECG_UDP_Out_Endpoint::is_loopback (const ACE_INET_Addr& from)
{
  if (this->port_number_ == 0)
    {
      // Cache the port number...
      ACE_INET_Addr local_addr;
      if (this->dgram ().get_local_addr (local_addr) == -1)
        return 0;
      this->port_number_ = local_addr.get_port_number ();
    }

  // Most of the time the port number is enough to determine if the
  // message is remote, only when the local port number and the remote
  // port number match we have to look at the local ip addresses.
  if (from.get_port_number () != this->port_number_)
    return 0;

  if (this->ifs_ == 0)
    {
      ACE::get_ip_interfaces (this->if_count_, this->ifs_);
    }

  for (ACE_INET_Addr* i = this->ifs_;
       i != this->ifs_ + this->if_count_;
       ++i)
    {
      if ((*i).get_ip_address () == from.get_ip_address ())
        return 1;
    }
  return 0;
}


// ****************************************************************

#if 0
TAO_ECG_UDP_Request_Entry::TAO_ECG_UDP_Request_Entry (void)
  : request_size_ (0),
    fragment_count_ (0),
    timeout_counter_ (0),
    payload_ (0),
    received_fragments_ (default_received_fragments_),
    own_received_fragments_ (0)
{
}

TAO_ECG_UDP_Request_Entry::
TAO_ECG_UDP_Request_Entry (const TAO_ECG_UDP_Request_Entry& rhs)
  :  byte_order_ (rhs.byte_order_),
     request_id_ (rhs.request_id_),
     request_size_ (rhs.request_size_),
     fragment_count_ (rhs.fragment_count_),
     timeout_counter_ (rhs.timeout_counter_),
     payload_ (ACE_Message_Block::duplicate (rhs.payload_)),
     received_fragments_ (default_received_fragments_),
     own_received_fragments_ (0)
{
}
#endif

TAO_ECG_UDP_Request_Entry::~TAO_ECG_UDP_Request_Entry (void)
{
  if (this->own_received_fragments_)
    {
      this->own_received_fragments_ = 0;
      delete[] this->received_fragments_;
    }
}

TAO_ECG_UDP_Request_Entry::
TAO_ECG_UDP_Request_Entry (CORBA::Boolean byte_order,
                           CORBA::ULong request_id,
                           CORBA::ULong request_size,
                           CORBA::ULong fragment_count)
  :  byte_order_ (byte_order),
     request_id_ (request_id),
     request_size_ (request_size),
     fragment_count_ (fragment_count)
{
  ACE_CDR::grow (&this->payload_, this->request_size_);
  this->payload_.wr_ptr (request_size_);

  this->received_fragments_ = this->default_received_fragments_;
  this->own_received_fragments_ = 0;
  const int bits_per_ulong = sizeof(CORBA::ULong) * CHAR_BIT;
  this->received_fragments_size_ =
    this->fragment_count_ / bits_per_ulong + 1;
  if (this->received_fragments_size_ > ECG_DEFAULT_FRAGMENT_BUFSIZ)
    {
      ACE_NEW (this->received_fragments_,
               CORBA::ULong[this->received_fragments_size_]);
      this->own_received_fragments_ = 1;
    }

  for (CORBA::ULong i = 0; i < this->received_fragments_size_; ++i)
    this->received_fragments_[i] = 0;
  CORBA::ULong idx = this->fragment_count_ / bits_per_ulong;
  CORBA::ULong bit = this->fragment_count_ % bits_per_ulong;
  this->received_fragments_[idx] = (0xFFFFFFFF << bit);
}

int
TAO_ECG_UDP_Request_Entry::validate_fragment (CORBA::Boolean byte_order,
                                              CORBA::ULong request_size,
                                              CORBA::ULong fragment_size,
                                              CORBA::ULong fragment_offset,
                                              CORBA::ULong /* fragment_id */,
                                              CORBA::ULong fragment_count) const
{
  if (byte_order != this->byte_order_
      || request_size != this->request_size_
      || fragment_count != this->fragment_count_)
    return 0;

  if (fragment_offset >= request_size
      || fragment_offset + fragment_size > request_size)
    return 0;

  return 1;
}

int
TAO_ECG_UDP_Request_Entry::test_received (CORBA::ULong fragment_id) const
{
  // Assume out-of-range fragments as received, so they are dropped...
  if (fragment_id > this->fragment_count_)
    return 1;

  const int bits_per_ulong = sizeof(CORBA::ULong) * CHAR_BIT;
  CORBA::ULong idx = fragment_id / bits_per_ulong;
  CORBA::ULong bit = fragment_id % bits_per_ulong;
  return ACE_BIT_ENABLED (this->received_fragments_[idx], 1<<bit);
}

void
TAO_ECG_UDP_Request_Entry::mark_received (CORBA::ULong fragment_id)
{
  // Assume out-of-range fragments as received, so they are dropped...
  if (fragment_id > this->fragment_count_)
    return;

  const int bits_per_ulong = sizeof(CORBA::ULong) * CHAR_BIT;
  CORBA::ULong idx = fragment_id / bits_per_ulong;
  CORBA::ULong bit = fragment_id % bits_per_ulong;
  ACE_SET_BITS (this->received_fragments_[idx], 1<<bit);
}

int
TAO_ECG_UDP_Request_Entry::complete (void) const
{
  for (CORBA::ULong i = 0;
       i < this->received_fragments_size_;
       ++i)
    {
      if (this->received_fragments_[i] != 0xFFFFFFFF)
        return 0;
    }
  return 1;
}

char*
TAO_ECG_UDP_Request_Entry::fragment_buffer (CORBA::ULong fragment_offset)
{
  return this->payload_.rd_ptr () + fragment_offset;
}

void
TAO_ECG_UDP_Request_Entry::decode (RtecEventComm::EventSet& event,
                                   CORBA::Environment& TAO_IN_ENV)
{
  TAO_InputCDR cdr (&this->payload_,
                    ACE_static_cast(int,this->byte_order_));
  cdr.decode (RtecEventComm::_tc_EventSet, &event, 0, TAO_IN_ENV);
}

// ****************************************************************

TAO_ECG_UDP_TH::TAO_ECG_UDP_TH (TAO_ECG_UDP_Receiver* r)
  :  receiver_ (r)
{
}

int
TAO_ECG_UDP_TH::handle_timeout (const ACE_Time_Value& tv,
                                const void* act)
{
  return this->receiver_->handle_timeout (tv, act);
}

// ****************************************************************

TAO_ECG_UDP_Receiver::TAO_ECG_UDP_Receiver (void)
  : timeout_handler_ (this),
    reactor_ (0)
{
}

void
TAO_ECG_UDP_Receiver::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
                            TAO_ECG_UDP_Out_Endpoint* ignore_from,
                            RtecUDPAdmin::AddrServer_ptr addr_server,
                            ACE_Reactor *reactor,
                            const ACE_Time_Value &expire_interval,
                            int max_timeout,
                            CORBA::Environment &TAO_IN_ENV)
{
  this->ignore_from_ = ignore_from;

  this->lcl_ec_ =
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->addr_server_ =
    RtecUDPAdmin::AddrServer::_duplicate (addr_server);

  this->reactor_ = reactor;
  this->max_timeout_ = max_timeout;
  // @@ TODO throw an exception....
  if (this->reactor_ == 0
      || this->reactor_->schedule_timer (&this->timeout_handler_, 0,
                                         expire_interval,
                                         expire_interval) == -1)
    ACE_ERROR ((LM_ERROR,
                "TAO_ECG_UDP_Receiver::init - "
                "cannot schedule timer\n"));
}

void
TAO_ECG_UDP_Receiver::open (RtecEventChannelAdmin::SupplierQOS& pub,
                            CORBA::Environment &TAO_IN_ENV)
{
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    return;

  if (!CORBA::is_nil (this->consumer_proxy_.in ()))
    this->close (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  if (pub.publications.length () == 0)
    return;

  // = Connect as a supplier to the local EC
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->lcl_ec_->for_suppliers (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  RtecEventComm::PushSupplier_var supplier_ref =
    this->_this (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  // ACE_DEBUG ((LM_DEBUG, "ECG_UDP_Receiver (%t) Gateway/Supplier "));
  // ACE_SupplierQOS_Factory::debug (pub);

  this->consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                pub,
                                                TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;
}

void
TAO_ECG_UDP_Receiver::close (CORBA::Environment &env)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (env);
  if (env.exception () != 0) return;
  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

}

void
TAO_ECG_UDP_Receiver::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier received "
              "disconnect from channel.\n"));
}

void
TAO_ECG_UDP_Receiver::shutdown (CORBA::Environment& TAO_IN_ENV)
{
  this->close (TAO_IN_ENV);
  if (TAO_IN_ENV.exception () != 0) return;

  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();

  this->reactor_->cancel_timer (&this->timeout_handler_);
}

int
TAO_ECG_UDP_Receiver::handle_input (ACE_SOCK_Dgram& dgram)
{
  // Use ULong so the alignment is right.
  CORBA::ULong header[TAO_ECG_UDP_Sender::ECG_HEADER_SIZE
                     / sizeof(CORBA::ULong)
                     + ACE_CDR::MAX_ALIGNMENT];
  ACE_INET_Addr from;
  ssize_t n = dgram.recv (header, sizeof(header), from, MSG_PEEK);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ECG_UDP_Receive_EH::handle_input - peek\n"), -1);
  else if (n == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ECG_UDP_Receive_EH::handle_input - peek 0\n"),
                      0);

  // This is to avoid receiving the events we send; notice that we
  // must read the message to drop it...
  if (this->ignore_from_ != 0
      && this->ignore_from_->is_loopback (from))
    {
      n = dgram.recv (header, sizeof(header), from);
      // ACE_DEBUG ((LM_DEBUG,
      //            "ECG_UDP_Receiver (%P|%t): cycle dropped\n"));
      return 0;
    }

  char* buf = ACE_reinterpret_cast(char*,header);
  int byte_order = buf[0];
  TAO_InputCDR header_cdr (buf, sizeof(header), byte_order);
  CORBA::Boolean unused;
  CORBA::ULong request_id;
  CORBA::ULong request_size;
  CORBA::ULong fragment_size;
  CORBA::ULong fragment_offset;
  CORBA::ULong fragment_id;
  CORBA::ULong fragment_count;
  header_cdr.read_boolean (unused);
  header_cdr.read_ulong (request_id);
  header_cdr.read_ulong (request_size);
  header_cdr.read_ulong (fragment_size);
  header_cdr.read_ulong (fragment_offset);
  header_cdr.read_ulong (fragment_id);
  header_cdr.read_ulong (fragment_count);

  if (request_size < fragment_size
      || fragment_offset >= request_size
      || fragment_id >= fragment_count)
    {
      // Drop the packet...
      n = dgram.recv (header, sizeof(header), from);
      return 0;
      // ACE_DEBUG ((LM_DEBUG,
      //             "ECG_UDP_Receiver (%P|%t): invalid fragment dropped"
      //             ", from = (%u:%d)\n",
      //             from.get_ip_address (), from.get_port_number ()));
    }

  //  ACE_DEBUG ((LM_DEBUG,
  //              "ECG_UDP_Receiver (%P|%t): msg = %d, from = (%u:%d) "
  //              "fragment = %d/%d\n",
  //              request_id,
  //              from.get_ip_address (), from.get_port_number (),
  //              fragment_id, fragment_count));

  TAO_ECG_UDP_Request_Index index (from, request_id);
  Request_Map_Entry* entry;

  if (this->request_map_.find (index, entry) == -1)
    {
      //      ACE_DEBUG ((LM_DEBUG,
      //                  "ECG_UDP_Receiver (%P|%t): new entry\n"
      //                  "    byte_order = %d\n"
      //                  "    request_id = %d\n"
      //                  "    request_size = %d\n"
      //                  "    fragment_size = %d\n"
      //                  "    fragment_offset = %d\n"
      //                  "    fragment_id = %d\n"
      //                  "    fragment_count = %d\n",
      //                  byte_order,
      //                  request_id,
      //                  request_size,
      //                  fragment_size,
      //                  fragment_offset,
      //                  fragment_id,
      //                  fragment_count));

      // Create an entry and insert it....
      TAO_ECG_UDP_Request_Entry* request_entry =
        new TAO_ECG_UDP_Request_Entry(byte_order,
                                      request_id,
                                      request_size,
                                      fragment_count);
      if (request_entry == 0
          || this->request_map_.bind (index,
                                      request_entry,
                                      entry) == -1)
        {
          // Drop the packet...
          n = dgram.recv (header, sizeof(header), from);
          return 0;
        }
    }

  // Validate the message...
  if (entry->int_id_->validate_fragment (byte_order,
                                         request_size,
                                         fragment_size,
                                         fragment_offset,
                                          fragment_id,
                                         fragment_count) == 0)
    {
      //      ACE_DEBUG ((LM_DEBUG,
      //                  "ECG_UDP_Receiver (%P|%t): fragment rejected:"
      //                  "    byte_order = %d\n"
      //                  "    request_id = %d\n"
      //                  "    request_size = %d\n"
      //                  "    fragment_size = %d\n"
      //                  "    fragment_offset = %d\n"
      //                  "    fragment_id = %d\n"
      //                  "    fragment_count = %d\n",
      //                  byte_order,
      //                  request_id,
      //                  request_size,
      //                  fragment_size,
      //                  fragment_offset,
      //                  fragment_id,
      //                  fragment_count));

      // Drop the fragment if it is invalid...
      n = dgram.recv (header, sizeof(header), from);
      return 0;
    }

  // Was this fragment received already?
  if (entry->int_id_->test_received (fragment_id) == 1)
    {
      //      ACE_DEBUG ((LM_DEBUG,
      //                  "ECG_UDP_Receiver (%P|%t): fragment duplicate\n"));

      // Drop the fragment...
      n = dgram.recv (header, sizeof(header), from);
      return 0;
    }

  // Now we should read the fragment, use an iovec to drop the header
  // into the bit bucket and the payload into the entry's buffer:
  const int iovcnt = 2;
  iovec iov[iovcnt];
  char drop_header[TAO_ECG_UDP_Sender::ECG_HEADER_SIZE];
  iov[0].iov_base = drop_header;
  iov[0].iov_len  = sizeof(drop_header);
  iov[1].iov_base = entry->int_id_->fragment_buffer (fragment_offset);
  iov[1].iov_len  = fragment_size;

  n = dgram.recv (iov, iovcnt, from);

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ECG_UDP_Receive_EH::handle_input - read\n"), -1);
  else if (n == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ECG_UDP_Receive_EH::handle_input - read 0\n"),
                      0);

  entry->int_id_->mark_received (fragment_id);
  // If the message is not complete we must return...
  if (!entry->int_id_->complete ())
    {
      //      ACE_DEBUG ((LM_DEBUG,
      //                  "ECG_UDP_Receiver (%P|%t): incomplete message\n"));

      return 0;
    }

  TAO_TRY
    {
      RtecEventComm::EventSet event;
      entry->int_id_->decode (event, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_->push (event, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      //      ACE_DEBUG ((LM_DEBUG,
      //                  "TAO_ECG_UDP_Received (%P|%t): push %d "
      //                  "from = (%u:%d)\n",
      //                  request_id,
      //                  from.get_ip_address (), from.get_port_number ()));
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("ECG_UDP_Receive_EH::handle_input");
    }
  TAO_ENDTRY;
  return 0;
}

void
TAO_ECG_UDP_Receiver::get_addr (const RtecEventComm::EventHeader& header,
                                RtecUDPAdmin::UDP_Addr_out addr,
                                CORBA::Environment& env)
{
  this->addr_server_->get_addr (header, addr, env);
}

int
TAO_ECG_UDP_Receiver::handle_timeout (const ACE_Time_Value& /* tv */,
                                      const void* /* act */)
{
  Request_Map::iterator begin = this->request_map_.begin ();
  Request_Map::iterator end = this->request_map_.end ();
  {
    for (Request_Map::iterator i = begin;
         i != end;
         ++i)
      {
        (*i).int_id_->inc_timeout ();
      }
  }
  for (Request_Map::iterator j = begin;
       j != end;
       )
    {
      if ((*j).int_id_->get_timeout () > this->max_timeout_)
        {
          Request_Map_Entry& entry = *j;
          ++j;
          {
            // ACE_DEBUG ((LM_DEBUG,
            //            "TAO_ECG_UDP_Receiver::handle_timeout (%P|%t) "
            //            "msg = %d\n",
            //            entry.ext_id_.request_id));
            delete entry.int_id_;
            this->request_map_.unbind (&entry);
          }
        }
      else
        {
          ++j;
        }
    }
  return 0;
}

// ****************************************************************

TAO_ECG_UDP_EH::TAO_ECG_UDP_EH (TAO_ECG_UDP_Receiver *recv)
  :  receiver_ (recv)
{
}

int
TAO_ECG_UDP_EH::open (const ACE_INET_Addr& ipaddr)
{
  if (this->dgram_.open (ipaddr) == -1)
    return -1;
  return this->reactor ()->register_handler (this,
                                             ACE_Event_Handler::READ_MASK);
}

int
TAO_ECG_UDP_EH::close (void)
{
  if (this->reactor ()->remove_handler (this,
                                        ACE_Event_Handler::READ_MASK) == -1)
    return -1;

  return this->dgram_.close ();
}

int
TAO_ECG_UDP_EH::handle_input (ACE_HANDLE)
{
  return this->receiver_->handle_input (this->dgram_);
}

ACE_HANDLE
TAO_ECG_UDP_EH::get_handle (void) const
{
  return this->dgram_.get_handle ();
}


// ****************************************************************

TAO_ECG_Mcast_EH::TAO_ECG_Mcast_EH (TAO_ECG_UDP_Receiver *recv)
  :  receiver_ (recv),
     observer_ (this)
{
}

int
TAO_ECG_Mcast_EH::open (RtecEventChannelAdmin::EventChannel_ptr ec,
                        CORBA::Environment& TAO_IN_ENV)
{
  this->ec_ = RtecEventChannelAdmin::EventChannel::_duplicate (ec);
  RtecEventChannelAdmin::Observer_var obs =
    this->observer_._this (TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  this->handle_ = this->ec_->append_observer (obs.in (), TAO_IN_ENV);
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  return 0;
}

int
TAO_ECG_Mcast_EH::close (CORBA::Environment& TAO_IN_ENV)
{
  if (this->reactor ()->remove_handler (this,
                                        ACE_Event_Handler::READ_MASK) == -1)
    return -1;

  if (this->dgram_.unsubscribe () == -1)
    return -1;

  this->ec_->remove_observer (this->handle_, TAO_IN_ENV);
  this->handle_ = 0;
  TAO_CHECK_ENV_RETURN (TAO_IN_ENV, -1);

  return 0;
}

int
TAO_ECG_Mcast_EH::handle_input (ACE_HANDLE)
{
  return this->receiver_->handle_input (this->dgram_);
}

ACE_HANDLE
TAO_ECG_Mcast_EH::get_handle (void) const
{
  return this->dgram_.get_handle ();
}

int
TAO_ECG_Mcast_EH::subscribe (const ACE_INET_Addr &mcast_addr)
{
  return this->dgram_.subscribe (mcast_addr);
}

int
TAO_ECG_Mcast_EH::unsubscribe (const ACE_INET_Addr &mcast_addr)
{
  return this->dgram_.unsubscribe (mcast_addr);
}

void
TAO_ECG_Mcast_EH::update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub,
                                   CORBA::Environment& TAO_IN_ENV)
{
  // ACE_DEBUG ((LM_DEBUG,
  //          "ECG_Mcast_EH (%t) updating consumer\n"));

  // @@ TODO: If we are more careful we may simply subscribe for the
  // new event types and unsubscribe from the old ones...
  this->reactor ()->remove_handler (this,
                                    ACE_Event_Handler::READ_MASK);
  this->dgram_.close ();

  int must_register = 0;
  for (CORBA::ULong i = 0; i < sub.dependencies.length (); ++i)
    {
      const RtecEventComm::EventHeader& header =
        sub.dependencies[i].event.header;

      if (0 <= header.type && header.type <= ACE_ES_EVENT_UNDEFINED)
        {
          // ACE_DEBUG ((LM_DEBUG,
          //          "ECG_Mcast_EH (%t) type = %d skipped\n",
          //          header.type));
          continue;
        }
      must_register = 1;
      RtecUDPAdmin::UDP_Addr addr;

      this->receiver_->get_addr (header, addr, TAO_IN_ENV);
      TAO_CHECK_ENV_RETURN_VOID (TAO_IN_ENV);

      ACE_INET_Addr inet_addr (addr.port, addr.ipaddr);
      if (this->subscribe (inet_addr) == -1)
        ACE_ERROR ((LM_ERROR,
                    "cannot subscribe to %s:%d\n",
                    inet_addr.get_host_addr (),
                    inet_addr.get_port_number ()));
      // ACE_DEBUG ((LM_DEBUG,
      //          "ECG_Mcast_EH (%t) subscribed to %s:%d\n",
      //          inet_addr.get_host_addr (),
      //          inet_addr.get_port_number ()));
    }

  if (must_register)
    this->reactor ()->register_handler (this,
                                        ACE_Event_Handler::READ_MASK);
}

void
TAO_ECG_Mcast_EH::update_supplier (const RtecEventChannelAdmin::SupplierQOS&,
                                   CORBA::Environment&)
{
  // Do nothing
}

// ****************************************************************

TAO_ECG_Mcast_EH::Observer::Observer (TAO_ECG_Mcast_EH* eh)
  :  eh_ (eh)
{
}

void
TAO_ECG_Mcast_EH::Observer::update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub,
                                             CORBA::Environment& TAO_IN_ENV)
{
  this->eh_->update_consumer (sub, TAO_IN_ENV);
}

void
TAO_ECG_Mcast_EH::Observer::update_supplier (const
                                             RtecEventChannelAdmin::SupplierQOS& pub,
                                             CORBA::Environment& TAO_IN_ENV)
{
  this->eh_->update_supplier (pub, TAO_IN_ENV);
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Manager<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*>;
template class ACE_Hash<TAO_ECG_UDP_Request_Index>;
template class ACE_Equal_To<TAO_ECG_UDP_Request_Index>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,ACE_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*>
#pragma instantiate ACE_Hash<TAO_ECG_UDP_Request_Index>
#pragma instantiate ACE_Equal_To<TAO_ECG_UDP_Request_Index>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, ACE_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
