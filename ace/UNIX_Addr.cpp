// UNIX_Addr.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/UNIX_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/UNIX_Addr.i"
#endif /* __ACE_INLINE__ */

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
  (void) ACE_OS::memset ((void *) &this->unix_addr_, 0, sizeof this->unix_addr_);
}

// Copy constructor. 

ACE_UNIX_Addr::ACE_UNIX_Addr (const ACE_UNIX_Addr &sa)
  : ACE_Addr (AF_UNIX, sa.get_size ())
{
  size_t size = sa.get_size ();

  // Add one extra byte to account for the NUL at the end of the
  // pathname.
  if (size < sizeof this->unix_addr_)
    size = sa.get_size () + 1;

  this->unix_addr_.sun_family = AF_UNIX;
  ACE_OS::strcpy (this->unix_addr_.sun_path, sa.unix_addr_.sun_path);
}

void
ACE_UNIX_Addr::set (const sockaddr_un *un, int len)
{
  (void) ACE_OS::memset ((void *) &this->unix_addr_, 0, 
		   sizeof this->unix_addr_);
  this->unix_addr_.sun_family = AF_UNIX;
  ACE_OS::strcpy (this->unix_addr_.sun_path, un->sun_path);
  this->base_set (AF_UNIX, len);
}

ACE_UNIX_Addr::ACE_UNIX_Addr (const sockaddr_un *un, int len)
{
  this->set (un, len);
}

void 
ACE_UNIX_Addr::set (const char rendezvous_point[])
{
  (void) ACE_OS::memset ((void *) &this->unix_addr_, 0, 
		   sizeof this->unix_addr_);
  this->unix_addr_.sun_family = AF_UNIX;
  size_t len = ACE_OS::strlen (rendezvous_point);

  if (len >= sizeof this->unix_addr_.sun_path)
    {
      // At this point, things are screwed up, so let's make sure we
      // don't crash.
      (void) ACE_OS::strncpy (this->unix_addr_.sun_path, 
			rendezvous_point, 
			sizeof this->unix_addr_.sun_path);
      len = sizeof this->unix_addr_.sun_path;
      this->unix_addr_.sun_path[len - 1] = '\0';
      // Don't count the NUL byte at the end of the string. 
      len -= 2; 
    }
  else
    (void) ACE_OS::strcpy (this->unix_addr_.sun_path, rendezvous_point);
  this->ACE_Addr::base_set (AF_UNIX, len + sizeof this->unix_addr_.sun_family);
}

// Create a ACE_Addr from a UNIX pathname.

ACE_UNIX_Addr::ACE_UNIX_Addr (const char rendezvous_point[])
{
  this->set (rendezvous_point);
}

#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
