/* -*- C++ -*- */
// $Id$

// INET_Addr.i

// Default dtor.
ACE_INLINE
ACE_INET_Addr::~ACE_INET_Addr (void)
{
}

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
#if defined (VXWORKS)
  // It would be nice to be able to encapsulate this into
  // ACE_OS::inet_ntoa(), but that would lead to either inefficiencies
  // on vxworks or lack of thread safety.
  //
  // So, we use the way that vxworks suggests.
  inet_ntoa_b (this->inet_addr_.sin_addr, this->buf_);
  return &buf_[0];
#else /* VXWORKS */
  return ACE_OS::inet_ntoa (this->inet_addr_.sin_addr);  
#endif /* VXWORKS */
}

// Return the 4-byte IP address, converting it into host byte order.

ACE_INLINE ACE_UINT32
ACE_INET_Addr::get_ip_address (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_ip_address");
  return ntohl (ACE_UINT32 (this->inet_addr_.sin_addr.s_addr));
}

ACE_INLINE u_long 
ACE_INET_Addr::hash (void) const
{
  return this->get_ip_address () + this->get_port_number ();
}

ACE_INLINE int
ACE_INET_Addr::operator < (const ACE_INET_Addr &rhs) const
{
  return this->get_ip_address () < rhs.get_ip_address ()
    || (this->get_ip_address () == rhs.get_ip_address ()
        && this->get_port_number () < rhs.get_port_number ());
}

#if defined (ACE_HAS_WCHAR)
ACE_INLINE int 
ACE_INET_Addr::set (u_short port_number, 
                    const wchar_t host_name[], 
                    int encode)
{
  return this->set (port_number, 
                    ACE_Wide_To_Ascii (host_name).char_rep (),
                    encode);
}

ACE_INLINE int 
ACE_INET_Addr::set (const wchar_t port_name[],
                    const wchar_t host_name[], 
                    const wchar_t protocol[])
{
  return this->set (ACE_Wide_To_Ascii (port_name).char_rep (), 
                    ACE_Wide_To_Ascii (host_name).char_rep (),
                    ACE_Wide_To_Ascii (protocol).char_rep ());
}

ACE_INLINE int 
ACE_INET_Addr::set (const wchar_t port_name[],
                    ACE_UINT32 ip_addr,
                    const wchar_t protocol[])
{
  return this->set (ACE_Wide_To_Ascii (port_name).char_rep (),
                    ip_addr,
                    ACE_Wide_To_Ascii (protocol).char_rep ());
}

ACE_INLINE int 
ACE_INET_Addr::set (const wchar_t addr[])
{
  return this->set (ACE_Wide_To_Ascii (addr).char_rep ());
}

#endif /* ACE_HAS_WCHAR */
