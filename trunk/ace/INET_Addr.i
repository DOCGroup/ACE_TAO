/* -*- C++ -*- */
// $Id$

// INET_Addr.i

#include "ace/Log_Msg.h"

// Return the port number, converting it into host byte order...

ACE_INLINE u_short
ACE_INET_Addr::get_port_number (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_port_number");
  return ntohs (this->inet_addr_.sin_port);
}

// Return the address.

ACE_INLINE void *
ACE_INET_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr");
  return (void *) &this->inet_addr_;
}

// Return the dotted Internet address.

ACE_INLINE const char *
ACE_INET_Addr::get_host_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_addr");
  return ACE_OS::inet_ntoa (this->inet_addr_.sin_addr);  
}

// Return the 4-byte IP address, converting it into host byte order.

ACE_INLINE ACE_UINT32
ACE_INET_Addr::get_ip_address (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_ip_address");
  return ntohl (ACE_UINT32 (this->inet_addr_.sin_addr.s_addr));
}

