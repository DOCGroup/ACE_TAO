// UNIX_Addr.cpp
// $Id$

#include "ace/IPC/UNIX_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/IPC/UNIX_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, UNIX_Addr, "$Id$")

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

ACE_ALLOC_HOOK_DEFINE(ACE_UNIX_Addr)

// Set a pointer to the address.
void 
ACE_UNIX_Addr::set_addr (void *addr, int len)
{
  ACE_TRACE ("ACE_UNIX_Addr::set_addr");

  this->ACE_Addr::base_set (AF_UNIX, len);
  ACE_OS::memcpy ((void *) &this->unix_addr_,
		  (void *) addr, 
		  len);
}

void
ACE_UNIX_Addr::dump (void) const
{
}

// Do nothing constructor. 

ACE_UNIX_Addr::ACE_UNIX_Addr (void)
  : ACE_Addr (AF_UNIX, sizeof this->unix_addr_)
{
  (void) ACE_OS::memset ((void *) &this->unix_addr_,
                         0,
                         sizeof this->unix_addr_);

  this->unix_addr_.sun_family = AF_UNIX;
}

int
ACE_UNIX_Addr::set (const ACE_UNIX_Addr &sa)
{
  if (sa.get_type () == AF_ANY)
    (void) ACE_OS::memset ((void *) &this->unix_addr_,
                           0,
                           sizeof this->unix_addr_);
  else
    ACE_OS::strcpy (this->unix_addr_.sun_path,
                    sa.unix_addr_.sun_path);

  this->unix_addr_.sun_family = AF_UNIX;
  this->base_set (sa.get_type (), sa.get_size ());

  return 0;
}

// Copy constructor. 

ACE_UNIX_Addr::ACE_UNIX_Addr (const ACE_UNIX_Addr &sa)
  : ACE_Addr (AF_UNIX, sa.get_size ())
{
  this->set (sa);
}

int
ACE_UNIX_Addr::set (const sockaddr_un *un, int len)
{
  (void) ACE_OS::memset ((void *) &this->unix_addr_, 0, 
		   sizeof this->unix_addr_);
  this->unix_addr_.sun_family = AF_UNIX;
  ACE_OS::strcpy (this->unix_addr_.sun_path, un->sun_path);
  this->base_set (AF_UNIX, len);
  return 0;
}

ACE_UNIX_Addr::ACE_UNIX_Addr (const sockaddr_un *un, int len)
{
  this->set (un, len);
}

int
ACE_UNIX_Addr::set (const char rendezvous_point[])
{
  (void) ACE_OS::memset ((void *) &this->unix_addr_, 
			 0,
			 sizeof this->unix_addr_);
  this->unix_addr_.sun_family = AF_UNIX;
  size_t len = ACE_OS::strlen (rendezvous_point);
  size_t maxlen = sizeof this->unix_addr_.sun_path;
 
  (void) ACE_OS::memcpy (this->unix_addr_.sun_path, 
			 rendezvous_point,
                         len >= maxlen ? maxlen - 1 : len);
 
  this->ACE_Addr::base_set (AF_UNIX, 
			    sizeof this->unix_addr_ -
			    sizeof (this->unix_addr_.sun_path) +
			    ACE_OS::strlen (this->unix_addr_.sun_path));
  return 0;
}

// Create a ACE_Addr from a UNIX pathname.

ACE_UNIX_Addr::ACE_UNIX_Addr (const char rendezvous_point[])
{
  this->set (rendezvous_point);
}

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
