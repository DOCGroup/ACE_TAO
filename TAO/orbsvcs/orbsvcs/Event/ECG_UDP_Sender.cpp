/**
 * @file ECG_UDP_Sender.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 */

#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event/ECG_UDP_Protocol.h"
#include "orbsvcs/Event/EC_Gateway_UDP.h"

#include "ace/Unbounded_Set.h"
#include "ace/Hash_Map_Manager.h"

#if !defined(__ACE_INLINE__)
#include "ECG_UDP_Sender.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, ECG_UDP_Sender, "$Id$")

ACE_TEMPLATE_SPECIALIZATION
class ACE_Equal_To<RtecUDPAdmin::UDP_Addr>
{
public:
  inline int operator () (const RtecUDPAdmin::UDP_Addr &lhs,
                   const RtecUDPAdmin::UDP_Addr &rhs) const
  {
    return lhs.ipaddr == rhs.ipaddr
      && lhs.port == rhs.port;
  }
};

ACE_TEMPLATE_SPECIALIZATION
class ACE_Hash<RtecUDPAdmin::UDP_Addr>
{
public:
  inline u_long operator () (const RtecUDPAdmin::UDP_Addr &x) const
  {
    return (x.ipaddr << 16) | x.port;
  }
};

TAO_ECG_UDP_Sender::TAO_ECG_UDP_Sender (void)
  :  mtu_ (TAO_ECG_UDP_Protocol::ECG_DEFAULT_MTU)
{
}

int
TAO_ECG_UDP_Sender::get_local_addr (ACE_INET_Addr& addr)
{
  if (this->endpoint_ == 0)
    return -1;
  return this->endpoint_->dgram ().get_local_addr (addr);
}

void
TAO_ECG_UDP_Sender::init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
                          RtecUDPAdmin::AddrServer_ptr addr_server,
                          TAO_ECG_UDP_Out_Endpoint* endpoint
                          ACE_ENV_ARG_DECL_NOT_USED)
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
  if (new_mtu < TAO_ECG_UDP_Protocol::ECG_MIN_MTU
      || new_mtu >= TAO_ECG_UDP_Protocol::ECG_MAX_MTU)
    return -1;
  this->mtu_ = new_mtu;
  return 0;
}

void
TAO_ECG_UDP_Sender::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();
}

void
TAO_ECG_UDP_Sender::open (RtecEventChannelAdmin::ConsumerQOS& sub
                          ACE_ENV_ARG_DECL)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Open gateway\n"));
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    return;

  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (sub.dependencies.length () == 0)
    return;

  //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Supplier "));
  //ACE_SupplierQOS_Factory::debug (pub);

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->lcl_ec_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventComm::PushConsumer_var consumer_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Consumer "));
  //ACE_ConsumerQOS_Factory::debug (sub);

  this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                sub
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_ECG_UDP_Sender::close (ACE_ENV_SINGLE_ARG_DECL)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  PortableServer::POA_var poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var id =
    poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_ECG_UDP_Sender::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier-consumer received "
              "disconnect from channel.\n"));
}

void
TAO_ECG_UDP_Sender::push (const RtecEventComm::EventSet &events
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // ACE_DEBUG ((LM_DEBUG, "ECG_UDP_Sender::push - \n"));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  // ACE_DEBUG ((LM_DEBUG, "%d event(s) - ", events.length ()));

  if (events.length () == 1)
    {
      // Optimize for the common case: no sense in trying to build
      // complex data structures to discover all the multicast groups
      // in this case.  Because it is also the most common case we
      // treat it specially...
      const RtecEventComm::Event& e = events[0];

      RtecEventComm::EventHeader header = e.header;
      // To avoid loops we keep a TTL field on the events and skip the
      // events with TTL <= 0
      if (header.ttl <= 0)
        return;

      RtecUDPAdmin::UDP_Addr udp_addr;
      this->addr_server_->get_addr (header, udp_addr
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->send_event (udp_addr, e
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // That's it, no need to continue...
      return;
    }

  typedef ACE_Unbounded_Set<CORBA::ULong> Index_Set;
  typedef ACE_Hash_Map_Manager<RtecUDPAdmin::UDP_Addr,Index_Set,ACE_Null_Mutex> Destination_Map;

  Destination_Map destination_map;
  for (CORBA::ULong i = 0; i != events.length (); ++i)
    {
      // To avoid loops we keep a TTL field on the events and skip the
      // events with TTL <= 0
      if (events[i].header.ttl <= 0)
        continue;

      const RtecEventComm::Event& e = events[i];

      // Grab the right mcast group for this event...
      RtecUDPAdmin::UDP_Addr udp_addr;
      this->addr_server_->get_addr (e.header, udp_addr
                                    ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      Destination_Map::ENTRY *entry;
      if (destination_map.find (udp_addr, entry) == -1)
        {
          Index_Set empty;
          (void) destination_map.bind (udp_addr, empty);
          (void) destination_map.find (udp_addr, entry);
        }
      entry->int_id_.insert (i);
    }


  for (Destination_Map::ITERATOR j = destination_map.begin ();
       j != destination_map.end ();
       ++j)
    {
      ACE_TRY
        {
          Index_Set &index_set = (*j).int_id_;
          CORBA::ULong length = ACE_static_cast (CORBA::ULong,
                                                 index_set.size ());

          TAO_OutputCDR cdr;
          cdr.write_ulong (length);
          for (Index_Set::iterator k = index_set.begin ();
               k != index_set.end ();
               ++k)
            {
              this->marshal_one_event (events[(*k)], cdr
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          this->send_cdr_stream ((*j).ext_id_, cdr
                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore exceptions raised while trying on one address,
          // gives the other addresses a chance to succeed
          // @@ TODO Log the error....
        }
      ACE_ENDTRY;
    }
}

void
TAO_ECG_UDP_Sender::marshal_one_event (const RtecEventComm::Event& e,
                                       TAO_OutputCDR &cdr
                                       ACE_ENV_ARG_DECL)
{
  // We need to modify the TTL field, but copying the entire event
  // would be wasteful; instead we create a new header and only
  // modify the header portion.
  RtecEventComm::EventHeader header = e.header;
  header.ttl--;

  if (!(cdr << header)
      || !(cdr << e.data))
    ACE_THROW (CORBA::MARSHAL ());
}

void
TAO_ECG_UDP_Sender::send_event (const RtecUDPAdmin::UDP_Addr& udp_addr,
                                const RtecEventComm::Event& e
                                ACE_ENV_ARG_DECL)
{
  // This is the CDR stream used to marshal the event...
  TAO_OutputCDR cdr;

  // Marshal as if it was a sequence of one element, notice how we
  // marshal a modified version of the header, but the payload is
  // marshal without any extra copies.
  cdr.write_ulong (1);

  this->marshal_one_event (e, cdr ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->send_cdr_stream (udp_addr, cdr
                         ACE_ENV_ARG_PARAMETER);
}

void
TAO_ECG_UDP_Sender::send_cdr_stream (const RtecUDPAdmin::UDP_Addr& udp_addr,
                                     TAO_OutputCDR &cdr
                                     ACE_ENV_ARG_DECL)
{
  // @@ TODO precompute this when the MTU changes...
  CORBA::ULong max_fragment_payload = this->mtu () -
    TAO_ECG_UDP_Protocol::ECG_HEADER_SIZE;
  // ACE_ASSERT (max_fragment_payload != 0);

#if defined (ACE_HAS_BROKEN_DGRAM_SENDV)
  const int TAO_WRITEV_MAX = ACE_IOV_MAX - 1;
#else
  const int TAO_WRITEV_MAX = ACE_IOV_MAX;
#endif /* ACE_HAS_BROKEN_DGRAM_SENDV */
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
      CORBA::ULong l = ACE_static_cast (CORBA::ULong, b->length ());

      char* rd_ptr = b->rd_ptr ();

      iov[iovcnt].iov_base = rd_ptr;
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
                               iovcnt
                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          fragment_id++;
          fragment_offset += max_fragment_payload;

          // Reset, but don't forget that the last Message_Block
          // may need to be sent in multiple fragments..
          l -= last_mb_length;
          rd_ptr += last_mb_length;
          iov[1].iov_base = rd_ptr;
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
                               iovcnt
                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
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
                               iovcnt
                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
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
                           iovcnt
                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      fragment_id++;
      fragment_offset += fragment_size;

      // reset, not needed here...
      // iovcnt = 1;
      // fragment_size = 0;
    }
  // ACE_ASSERT (total_length == fragment_offset);
  // ACE_ASSERT (fragment_id == fragment_count);
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
                                   int iovcnt
                                   ACE_ENV_ARG_DECL)
{
  CORBA::ULong header[TAO_ECG_UDP_Protocol::ECG_HEADER_SIZE
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
  iov[0].iov_len  = ACE_static_cast (u_long, cdr.begin ()->length ());

  ACE_INET_Addr inet_addr (udp_addr.port,
                           udp_addr.ipaddr);

  //  ACE_DEBUG ((LM_DEBUG,
  //              "ECG_UDP_Sender (%P|%t): msg = %d, fragment %d/%d, "
  //               "dest = (%u:%d)\n",
  //              request_id,
  //              fragment_id, fragment_count,
  //              udp_addr.ipaddr, udp_addr.port));

  ssize_t n = this->endpoint_->dgram ().send (iov,
                                              iovcnt,
                                              inet_addr);
  if (n == -1)
    {
      // @@ TODO Use a Event Channel specific exception
      ACE_DEBUG ((LM_DEBUG,
                  "ECG_UDP (%t) send failed %p\n", ""));
      ACE_THROW (CORBA::COMM_FAILURE ());
    }
  else if (n == 0)
    {
      // @@ TODO Use a Event Channel specific exception
      ACE_DEBUG ((LM_DEBUG,
                  "ECG_UDP (%t) EOF on send \n"));
      ACE_THROW (CORBA::COMM_FAILURE ());
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
      CORBA::ULong l = ACE_static_cast (CORBA::ULong, b->length ());
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

typedef ACE_Unbounded_Set<CORBA::ULong> Index_Set;
template class ACE_Node<CORBA::ULong>;
template class ACE_Unbounded_Set<CORBA::ULong>;
template class ACE_Unbounded_Set_Iterator<CORBA::ULong>;
template class ACE_Hash_Map_Entry<RtecUDPAdmin::UDP_Addr,Index_Set>;
template class ACE_Hash_Map_Manager<RtecUDPAdmin::UDP_Addr,Index_Set,ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<RtecUDPAdmin::UDP_Addr, Index_Set, ACE_Hash<RtecUDPAdmin::UDP_Addr>, ACE_Equal_To<RtecUDPAdmin::UDP_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<RtecUDPAdmin::UDP_Addr, Index_Set, ACE_Hash<RtecUDPAdmin::UDP_Addr>, ACE_Equal_To<RtecUDPAdmin::UDP_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<RtecUDPAdmin::UDP_Addr, Index_Set, ACE_Hash<RtecUDPAdmin::UDP_Addr>, ACE_Equal_To<RtecUDPAdmin::UDP_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<RtecUDPAdmin::UDP_Addr, Index_Set, ACE_Hash<RtecUDPAdmin::UDP_Addr>, ACE_Equal_To<RtecUDPAdmin::UDP_Addr>, ACE_Null_Mutex>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
