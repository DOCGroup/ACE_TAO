/* -*- C++ -*- */
// $Id$

// INET_Addr.i

// Default dtor.
ACE_INLINE
ACE_INET_Addr::~ACE_INET_Addr (void)
{
}

#if defined (ACE_USES_IPV4_IPV6_MIGRATION)

// Process-wide Protocol Family
#include "ace/Synch.h"
#include "ace/Object_Manager.h"
int ACE_INET_Addr::protocol_family_ = -1;

int
ACE_INET_Addr::protocol_family (void)
{
  ACE_TRACE ("ACE_INET_Addr::protocol_family");

  if (ACE_INET_Addr::protocol_family_ == -1)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (ACE_INET_Addr::protocol_family_ == -1)
        {
          // Determine if the kernel has IPv6 support by attempting to
          // create a PF_INET6 socket and see if it fails.
          int s = socket(PF_INET6,SOCK_DGRAM,0);
          if(s == -1) {
            ACE_INET_Addr::protocol_family_ = PF_INET;
          } else {
            ACE_INET_Addr::protocol_family_ = PF_INET6;
            close(s);
          }
        }
    }

  return ACE_INET_Addr::protocol_family_;
}

int
ACE_INET_Addr::address_family (void)
{
  if(ACE_INET_Addr::protocol_family() == PF_INET6)
    return AF_INET6;
  else
    return AF_INET;
}

#endif /* ACE_USES_IPV4_IPV6_MIGRATION */

void *ACE_INET_Addr::ip_addr_pointer(void) const
{
#if defined (ACE_HAS_IPV6)
#if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    return (void*)&this->inet_addr4_.sin_addr;
  else
#endif
    return (void*)&this->inet_addr6_.sin6_addr;
#else
  return (void*)&this->inet_addr4_.sin_addr;
#endif
}

size_t ACE_INET_Addr::ip_addr_size(void) const
{
#if defined (ACE_HAS_IPV6)

#if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    return sizeof this->inet_addr4_.sin_addr;
  else
#endif
    return sizeof this->inet_addr6_.sin6_addr;

#else
  return sizeof this->inet_addr4_.sin_addr;
#endif
}

// Return the port number, converting it into host byte order...

ACE_INLINE u_short
ACE_INET_Addr::get_port_number (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_port_number");
#if defined (ACE_HAS_IPV6)

#if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    return ntohs (this->inet_addr4_.sin_port);
   else
#endif
    return ntohs (this->inet_addr6_.sin6_port);
#else
  return ntohs (this->inet_addr6_.sin_port);
#endif
}

// Return the address.

ACE_INLINE void *
ACE_INET_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr");
#if defined (ACE_HAS_IPV6)

#if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    return (void*)&this->inet_addr4_;
  else
#endif
    return (void*)&this->inet_addr6_;
#else
  return (void *) &this->inet_addr4_;
#endif
}

ACE_INLINE int
ACE_INET_Addr::get_addr_size (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_addr_size");
#if defined (ACE_HAS_IPV6)

#if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    return sizeof this->inet_addr4_;
  else
#endif
    return sizeof this->inet_addr6_;
#else
  return sizeof this->inet_addr4_;
#endif
}


ACE_INLINE u_long
ACE_INET_Addr::hash (void) const
{
#if defined (ACE_HAS_IPV6)
  // XXX do migration code
  const unsigned int *addr = (const unsigned int*)this->ip_addr_pointer();
  return addr[0] + addr[1] + addr[2] + addr[3] + this->get_port_number();
#else
  return this->get_ip_address () + this->get_port_number ();
#endif
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
