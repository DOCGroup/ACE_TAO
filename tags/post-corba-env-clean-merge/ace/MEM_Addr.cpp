// $Id$

// Defines the Internet domain address family address format.

#include "ace/MEM_Addr.h"
#include "ace/Log_Msg.h"

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

#if !defined (__ACE_INLINE__)
#include "ace/MEM_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, MEM_Addr, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_MEM_Addr)

// Transform the current address into string format.

ACE_MEM_Addr::ACE_MEM_Addr (void)
  : ACE_Addr (AF_INET, sizeof (ACE_MEM_Addr))
{
  // ACE_TRACE ("ACE_MEM_Addr::ACE_MEM_Addr");
  this->initialize_local (0);
}

ACE_MEM_Addr::ACE_MEM_Addr (const ACE_MEM_Addr &sa)
  : ACE_Addr (AF_INET, sizeof (ACE_MEM_Addr))
{
  ACE_TRACE ("ACE_MEM_Addr::ACE_MEM_Addr");
  this->external_.set (sa.external_);
  this->internal_.set (sa.internal_);
}

ACE_MEM_Addr::ACE_MEM_Addr (const ACE_TCHAR port_number[])
  : ACE_Addr (AF_INET, sizeof (ACE_MEM_Addr))
{
  ACE_TRACE ("ACE_MEM_Addr::ACE_MEM_Addr");
  u_short pn
    = ACE_static_cast (u_short,
                       ACE_OS::strtoul (port_number,
                                        NULL,
                                        10));
  this->initialize_local (pn);
}

ACE_MEM_Addr::ACE_MEM_Addr (u_short port_number)
  : ACE_Addr (AF_INET, sizeof (ACE_MEM_Addr))
{
  ACE_TRACE ("ACE_MEM_Addr::ACE_MEM_Addr");
  this->initialize_local (port_number);
}

int
ACE_MEM_Addr::initialize_local (u_short port_number)
{
  ACE_TCHAR name[MAXHOSTNAMELEN + 1];
  if (ACE_OS::hostname (name, MAXHOSTNAMELEN+1) == -1)
    return -1;

  this->external_.set (port_number, name);
  this->internal_.set (port_number, ACE_LIB_TEXT ("localhost"));
  return 0;
}

int
ACE_MEM_Addr::same_host (const ACE_INET_Addr &sap)
{
  ACE_TRACE ("ACE_MEM_Addr::same_host");

  // Rather than try to obtain and compare low-level binary
  // data, get copies of both addresses, set the ports to 0
  // (don't care about them) then compare for equality.
  ACE_INET_Addr me (this->external_);
  ACE_INET_Addr you (sap);
  me.set_port_number (0, 0);    // No need to encode a 0
  you.set_port_number (0, 0);
  return me == you;
}

int
ACE_MEM_Addr::addr_to_string (ACE_TCHAR s[],
                              size_t size,
                              int ipaddr_format) const
{
  ACE_TRACE ("ACE_MEM_Addr::addr_to_string");

  return this->external_.addr_to_string (s, size, ipaddr_format);
}

// Transform the string into the current addressing format.

int
ACE_MEM_Addr::string_to_addr (const ACE_TCHAR s[])
{
  ACE_TRACE ("ACE_MEM_Addr::string_to_addr");

  u_short pn
    = ACE_static_cast (u_short,
                       ACE_OS::strtoul (s,
                                        NULL,
                                        10));
  return this->set (pn);
}

// Return the address.

void *
ACE_MEM_Addr::get_addr (void) const
{
  ACE_TRACE ("ACE_MEM_Addr::get_addr");
  return this->external_.get_addr ();
}

// Set a pointer to the address.
void
ACE_MEM_Addr::set_addr (void *addr, int len)
{
  ACE_TRACE ("ACE_MEM_Addr::set_addr");

  this->external_.set_addr (addr, len);
  this->internal_.set_port_number (this->external_.get_port_number ());
}

int
ACE_MEM_Addr::get_host_name (ACE_TCHAR hostname[],
                              size_t len) const
{
  ACE_TRACE ("ACE_MEM_Addr::get_host_name");
  return this->external_.get_host_name (hostname, len);
}

// Return the character representation of the hostname.

const char *
ACE_MEM_Addr::get_host_name (void) const
{
  ACE_TRACE ("ACE_MEM_Addr::get_host_name");
  return this->external_.get_host_name ();
}

void
ACE_MEM_Addr::dump (void) const
{
  ACE_TRACE ("ACE_MEM_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->external_.dump ();
  this->internal_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */
