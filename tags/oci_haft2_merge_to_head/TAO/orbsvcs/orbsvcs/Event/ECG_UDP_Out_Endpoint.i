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

