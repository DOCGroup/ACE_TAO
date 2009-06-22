// -*- C++ -*-
//
// $Id$

#include "ace/OS_NS_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_ECG_UDP_Out_Endpoint::TAO_ECG_UDP_Out_Endpoint (void)
  : request_id_generator_ (static_cast<CORBA::ULong> (ACE_OS::time (0))),
    port_number_ (0),
    if_count_ (0),
    ifs_ (0)
{
}

ACE_INLINE
TAO_ECG_UDP_Out_Endpoint::TAO_ECG_UDP_Out_Endpoint (const TAO_ECG_UDP_Out_Endpoint& rhs)
  : port_number_ (0),
    if_count_ (0),
    ifs_ (0)
{
  *this = rhs;
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

TAO_END_VERSIONED_NAMESPACE_DECL
