// $Id$

#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.h"
#include "ace/INET_Addr.h"
#include "ace/Sock_Connect.h"
#include "ace/OS_NS_string.h"

#if !defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.inl"
#endif /* __ACE_INLINE__ */



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ECG_UDP_Out_Endpoint::~TAO_ECG_UDP_Out_Endpoint (void)
{
  this->dgram_.close ();

  delete [] this->ifs_;
}

CORBA::Boolean
TAO_ECG_UDP_Out_Endpoint::is_loopback (const ACE_INET_Addr& from)
{
  if (this->port_number_ == 0)
    {
      // Cache the port number...
      ACE_INET_Addr local_addr;
      if (this->dgram ().get_local_addr (local_addr) == -1)
        return false;
      this->port_number_ = local_addr.get_port_number ();
    }

  // Most of the time the port number is enough to determine if the
  // message is remote, only when the local port number and the remote
  // port number match we have to look at the local ip addresses.
  if (from.get_port_number () != this->port_number_)
    return false;

  if (this->ifs_ == 0)
    {
      ACE::get_ip_interfaces (this->if_count_, this->ifs_);
    }

  for (ACE_INET_Addr* i = this->ifs_;
       i != this->ifs_ + this->if_count_;
       ++i)
    {
      if (i->is_ip_equal(from))
        return true;
    }
  return false;
}

TAO_ECG_UDP_Out_Endpoint&
TAO_ECG_UDP_Out_Endpoint::operator= (const TAO_ECG_UDP_Out_Endpoint& rhs)
{
  if (this != &rhs)
    {
      this->request_id_generator_ = rhs.request_id_generator_;
      this->dgram_ = rhs.dgram_;
      this->port_number_ = rhs.port_number_;
      this->if_count_ = rhs.if_count_;

      delete [] this->ifs_;
      this->ifs_ = 0;

      if (this->if_count_ != 0)
        {
          ACE_NEW_RETURN (this->ifs_,
                          ACE_INET_Addr [this->if_count_],
                          *this);
          for (size_t i = 0; i < this->if_count_; ++i)
            this->ifs_[i] = rhs.ifs_[i];
        }
    }

  return *this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
