// $Id$

#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event/ECG_UDP_Protocol.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"
#include "ace/Reactor.h"

#if !defined(__ACE_INLINE__)
#include "EC_Gateway_UDP.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Event, EC_Gateway_UDP, "$Id$")

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

// @@ todo:This code is commented out. So no problem. But when this
// code is enabled we need to make sure that
// ACE_Message_Block::duplicate () should be called only if the data
// block in it is NOT from stack. Else we need to copy the datablock
// and then duplicate () it. See $TAO_ROOT/tao/Sequence.{cpp,i} on how
// this is done.

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
     fragment_count_ (fragment_count),
     timeout_counter_ (0)
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
TAO_ECG_UDP_Request_Entry::decode (RtecEventComm::EventSet& event
                                   ACE_ENV_ARG_DECL)
{
  TAO_InputCDR cdr (&this->payload_,
                    ACE_static_cast(int,this->byte_order_));
  if (!(cdr >> event))
    ACE_THROW (CORBA::MARSHAL ());
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
                            int max_timeout
                            ACE_ENV_ARG_DECL_NOT_USED)
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
TAO_ECG_UDP_Receiver::open (RtecEventChannelAdmin::SupplierQOS& pub
                            ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (this->lcl_ec_.in ()))
    return;

  if (!CORBA::is_nil (this->consumer_proxy_.in ()))
    this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (pub.publications.length () == 0)
    return;

  // = Connect as a supplier to the local EC
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->lcl_ec_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_proxy_ =
    supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventComm::PushSupplier_var supplier_ref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ACE_DEBUG ((LM_DEBUG, "ECG_UDP_Receiver (%t) Gateway/Supplier "));
  // ACE_SupplierQOS_Factory::debug (pub);

  this->consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                pub
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_ECG_UDP_Receiver::close (ACE_ENV_SINGLE_ARG_DECL)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();

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
TAO_ECG_UDP_Receiver::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier received "
              "disconnect from channel.\n"));
}

void
TAO_ECG_UDP_Receiver::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->lcl_ec_ = RtecEventChannelAdmin::EventChannel::_nil ();

  this->reactor_->cancel_timer (&this->timeout_handler_);
}

int
TAO_ECG_UDP_Receiver::handle_input (ACE_SOCK_Dgram& dgram)
{
  // Use ULong so the alignment is right.
  CORBA::ULong header[TAO_ECG_UDP_Protocol::ECG_HEADER_SIZE
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


  TAO_ECG_UDP_Request_Index map_index (from, request_id);
  Request_Map_Entry* entry;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    tao_mon,
                    this->request_map_protector_,
	            -1);

  if (this->request_map_.find (map_index, entry) == -1)
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
          || this->request_map_.bind (map_index,
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
  char drop_header[TAO_ECG_UDP_Protocol::ECG_HEADER_SIZE];
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

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      RtecEventComm::EventSet event;
      entry->int_id_->decode (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->consumer_proxy_->push (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //      ACE_DEBUG ((LM_DEBUG,
      //                  "TAO_ECG_UDP_Received (%P|%t): push %d "
      //                  "from = (%u:%d)\n",
      //                  request_id,
      //                  from.get_ip_address (), from.get_port_number ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "ECG_UDP_Receive_EH::handle_input");
    }
  ACE_ENDTRY;
  return 0;
}

void
TAO_ECG_UDP_Receiver::get_addr (const RtecEventComm::EventHeader& header,
                                RtecUDPAdmin::UDP_Addr_out addr
                                ACE_ENV_ARG_DECL)
{
  this->addr_server_->get_addr (header, addr ACE_ENV_ARG_PARAMETER);
}

int
TAO_ECG_UDP_Receiver::handle_timeout (const ACE_Time_Value& /* tv */,
                                      const void* /* act */)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    tao_mon,
                    this->request_map_protector_,
                    -1);

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
TAO_ECG_UDP_EH::open (const ACE_INET_Addr& ipaddr,
                      int reuse_addr)
{
  if (this->dgram_.open (ipaddr, PF_INET, 0, reuse_addr) == -1)
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>;
template class ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX, CORBA::ULong>;
template class ACE_Hash_Map_Manager<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Entry<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*>;
template class ACE_Hash<TAO_ECG_UDP_Request_Index>;
template class ACE_Equal_To<TAO_ECG_UDP_Request_Index>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,TAO_SYNCH_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>
#pragma instantiate ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX, CORBA::ULong>
#pragma instantiate ACE_Hash_Map_Manager<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*>
#pragma instantiate ACE_Hash<TAO_ECG_UDP_Request_Index>
#pragma instantiate ACE_Equal_To<TAO_ECG_UDP_Request_Index>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_ECG_UDP_Request_Index,TAO_ECG_UDP_Request_Entry*,TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ECG_UDP_Request_Index, TAO_ECG_UDP_Request_Entry*, ACE_Hash<TAO_ECG_UDP_Request_Index>, ACE_Equal_To<TAO_ECG_UDP_Request_Index>, TAO_SYNCH_MUTEX>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
