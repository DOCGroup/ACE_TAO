/* -*- C++ -*- */
// $Id$

// INET_Addr.i

// Default dtor.
ACE_INLINE
ACE_INET_Addr::~ACE_INET_Addr (void)
{
}

ACE_INLINE void
ACE_INET_Addr::reset (void)
{
  ACE_OS::memset(&this->inet_addr_, 0, sizeof(this->inet_addr_));
  if (this->get_type() == AF_INET) {
    this->inet_addr_.in4_.sin_family = AF_INET;
  }
#if defined (ACE_HAS_IPV6)
  else if (this->get_type() == AF_INET6) {
    this->inet_addr_.in6_.sin6_family = AF_INET6;
  }
#endif  /* ACE_HAS_IPV6 */
}

ACE_INLINE int
ACE_INET_Addr::determine_type (void) const
{
#if defined (ACE_HAS_IPV6)
#  if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  return ACE_Sock_Connect::ipv6_enabled () ? AF_INET6 : AF_INET;
#  else
  return AF_INET6;
#  endif /* ACE_USES_IPV4_IPV6_MIGRATION */
#endif /* ACE_HAS_IPV6 */
  return AF_INET;
}

ACE_INLINE void *
ACE_INET_Addr::ip_addr_pointer (void) const
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

ACE_INLINE int
ACE_INET_Addr::ip_addr_size (void) const
{
  // Since this size value is used to pass to other host db-type
  // functions (gethostbyaddr, etc.) the length is of int type.
  // Thus, cast all these sizes back to int. They're all well
  // within the range of an int anyway.
#if defined (ACE_HAS_IPV6)
  if (this->get_type () == PF_INET)
    return ACE_static_cast (int, sizeof this->inet_addr_.in4_.sin_addr);
  else
    return ACE_static_cast (int, sizeof this->inet_addr_.in6_.sin6_addr);
#else
  // These _UNICOS changes were picked up from pre-IPv6 code in
  // get_host_name_i... the IPv6 section above may need something
  // similar, so keep an eye out for it.
#  if !defined(_UNICOS)
  return ACE_static_cast (int, sizeof this->inet_addr_.in4_.sin_addr.s_addr);
#  else /* _UNICOS */
  return ACE_static_cast (int, sizeof this->inet_addr_.in4_.sin_addr);
#  endif /* ! _UNICOS */
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
  if (this->get_type () == PF_INET)
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
  if (this->get_type () == PF_INET6)
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
                    int encode,
                    int address_family)
{
  return this->set (port_number,
                    ACE_Wide_To_Ascii (host_name).char_rep (),
                    encode,
                    address_family);
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
