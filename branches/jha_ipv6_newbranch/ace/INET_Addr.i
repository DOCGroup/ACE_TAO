/* -*- C++ -*- */
// $Id$

// INET_Addr.i

// Default dtor.
ACE_INLINE
ACE_INET_Addr::~ACE_INET_Addr (void)
{
}

ACE_INLINE void *
ACE_INET_Addr::ip_addr_pointer(void) const
{
#if defined (ACE_HAS_IPV6)
  if (this->get_type () == PF_INET)
    return (void*)&this->inet_addr_.in4_.sin_addr;
  else
    return (void*)&this->inet_addr_.in6_.sin6_addr;
#else
  return (void*)&this->inet_addr_.in4_.sin_addr;
#endif
}

ACE_INLINE size_t
ACE_INET_Addr::ip_addr_size(void) const
{
#if defined (ACE_HAS_IPV6)
  if (this->inet_addr_.in4_.sin_family == PF_INET)
    return sizeof this->inet_addr_.in4_.sin_addr;
  else
    return sizeof this->inet_addr_.in6_.sin6_addr;
#else
  return sizeof this->inet_addr_.in4_.sin_addr;
#endif /* ACE_HAS_IPV6 */
}

// Return the port number, converting it into host byte order...

ACE_INLINE u_short
ACE_INET_Addr::get_port_number (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_port_number");
#if defined (ACE_HAS_IPV6)
  if (this->get_type () == PF_INET)
    return ntohs (this->inet_addr_.in4_.sin_port);
  else
    return ntohs (this->inet_addr_.in6_.sin6_port);
#else
  return ntohs (this->inet_addr_.in4_.sin_port);
#endif /* ACE_HAS_IPV6 */
}

// Return the address.

ACE_INLINE void *
ACE_INET_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr");
  return (void*)&this->inet_addr_;
}

ACE_INLINE int
ACE_INET_Addr::get_addr_size (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr_size");
#if defined (ACE_HAS_IPV6)
  if (this->inet_addr_.in4_.sin_family == PF_INET)
    return sizeof this->inet_addr_.in4_;
  else
    return sizeof this->inet_addr_.in6_;
#else
  return sizeof this->inet_addr_.in4_;
#endif /* ACE_HAS_IPV6 */
}


ACE_INLINE u_long
ACE_INET_Addr::hash (void) const
{
#if defined (ACE_HAS_IPV6)
  if (this->inet_addr_.in6_.sin6_family == PF_INET6)
    {
      const unsigned int *addr = (const unsigned int*)this->ip_addr_pointer();
      return addr[0] + addr[1] + addr[2] + addr[3] + this->get_port_number();
    }
  else
#endif /* ACE_HAS_IPV6 */
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
