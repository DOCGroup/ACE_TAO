//
// $Id$
//

ACE_INLINE
TAO_ECG_UDP_Out_Endpoint::TAO_ECG_UDP_Out_Endpoint (void)
  : port_number_ (0),
    if_count_ (0),
    ifs_ (0)
{
}

ACE_INLINE ACE_SOCK_Dgram&
TAO_ECG_UDP_Out_Endpoint::dgram (void)
{
  return this->dgram_;
}

ACE_INLINE CORBA::ULong
TAO_ECG_UDP_Out_Endpoint::next_request_id (void)
{
  return this->request_id_generator_++;
}

// ****************************************************************

ACE_INLINE ACE_SOCK_Dgram&
TAO_ECG_UDP_Sender::dgram (void)
{
  return this->endpoint_->dgram ();
}

ACE_INLINE CORBA::ULong
TAO_ECG_UDP_Sender::mtu (void) const
{
  return this->mtu_;
}

// ****************************************************************

ACE_INLINE
TAO_ECG_UDP_Request_Index::TAO_ECG_UDP_Request_Index (void)
  : request_id (0)
{
}

ACE_INLINE
TAO_ECG_UDP_Request_Index::TAO_ECG_UDP_Request_Index (const ACE_INET_Addr& from_,
                                                      CORBA::ULong request_id_)
  :  from (from_),
     request_id (request_id_)
{
}

ACE_INLINE u_long
TAO_ECG_UDP_Request_Index::hash (void) const
{
  return ((from.get_ip_address() << 24)
          | (from.get_port_number () << 8)
          | (request_id & 0x000000ff));
}

ACE_INLINE int
TAO_ECG_UDP_Request_Index::operator== (const TAO_ECG_UDP_Request_Index& rhs) const
{
  return (this->from == rhs.from &&
          this->request_id == rhs.request_id);
}

ACE_INLINE int
TAO_ECG_UDP_Request_Index::operator!= (const TAO_ECG_UDP_Request_Index& rhs) const
{
  return !(*this == rhs);
}

// ****************************************************************

void
TAO_ECG_UDP_Request_Entry::inc_timeout (void)
{
  this->timeout_counter_++;
}

int
TAO_ECG_UDP_Request_Entry::get_timeout (void) const
{
  return this->timeout_counter_;
}
