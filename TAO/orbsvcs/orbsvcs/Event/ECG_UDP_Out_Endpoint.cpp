// $Id$

#include "ECG_UDP_Out_Endpoint.h"
#include "ace/INET_Addr.h"
#include "ace/Sock_Connect.h"

#if !defined(__ACE_INLINE__)
#include "ECG_UDP_Out_Endpoint.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, ECG_UDP_Out_Endpoint, "$Id$")

TAO_ECG_UDP_Out_Endpoint::~TAO_ECG_UDP_Out_Endpoint (void)
{
  this->dgram_.close ();

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
      ACE_Sock_Connect::get_ip_interfaces (this->if_count_, this->ifs_);
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>;
template class ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX, CORBA::ULong>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<TAO_SYNCH_MUTEX, CORBA::ULong>
#pragma instantiate ACE_Atomic_Op_Ex<TAO_SYNCH_MUTEX, CORBA::ULong>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
