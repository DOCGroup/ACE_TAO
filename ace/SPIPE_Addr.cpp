// SPIPE_Addr.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SPIPE_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/SPIPE_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_SPIPE_Addr)

void
ACE_SPIPE_Addr::dump (void) const
{
}

// Set a pointer to the address.
void 
ACE_SPIPE_Addr::set_addr (void *addr, int len)
{
  ACE_TRACE ("ACE_SPIPE_Addr::set_addr");

  this->ACE_Addr::base_set (AF_SPIPE, len);
  ACE_OS::memcpy ((void *) &this->SPIPE_addr_,
		  (void *) addr, 
		  len);
}

// Do nothing constructor. 

ACE_SPIPE_Addr::ACE_SPIPE_Addr (void)
  : ACE_Addr (AF_SPIPE, sizeof this->SPIPE_addr_)
{
  (void) ACE_OS::memset ((void *) &this->SPIPE_addr_, 0, 
			 sizeof this->SPIPE_addr_);
}

// Transform the string into the current addressing format.

#if defined (UNICODE)
int
ACE_SPIPE_Addr::string_to_addr (const wchar_t *addr)
{
  return this->set (addr);
}
#endif /* UNICODE */

int
ACE_SPIPE_Addr::string_to_addr (const char *addr)
{
  return this->set (ACE_WIDE_STRING (addr));
}

/* Copy constructor. */

ACE_SPIPE_Addr::ACE_SPIPE_Addr (const ACE_SPIPE_Addr &sa)
  : ACE_Addr (AF_SPIPE,
	      sizeof this->SPIPE_addr_.gid_ + sizeof this->SPIPE_addr_.uid_
	      + ACE_OS::strlen (this->SPIPE_addr_.rendezvous_) + 1)
{
  (void) ACE_OS::memcpy ((void *) &this->SPIPE_addr_, (void *)
			 &sa.SPIPE_addr_, sa.get_size ()); 
}

int
ACE_SPIPE_Addr::set (LPCTSTR addr,
		     gid_t gid, 
		     uid_t uid)
{
  int len = sizeof (this->SPIPE_addr_.uid_);
  len += sizeof(this->SPIPE_addr_.gid_);

#if defined (ACE_WIN32)
  TCHAR *colonp = ACE_OS::strchr (addr, ':');
  TCHAR temp[BUFSIZ];

  if (colonp == 0) // Assume it's a port number.
    {
      ACE_OS::strcpy(temp, __TEXT ( "\\\\.\\pipe\\"));
      ACE_OS::strcat(temp, addr);
    }
  else
    {
      ACE_OS::strcpy(temp, __TEXT ("\\\\"));
      *colonp = __TEXT ('\0');
      if (ACE_OS::strcmp(addr, __TEXT ("localhost")) == 0)
	ACE_OS::strcat(temp, __TEXT (".")); // change localhost to .
      else
	ACE_OS::strcat(temp, addr);
      ACE_OS::strcat(temp, __TEXT ("\\pipe\\"));
      ACE_OS::strcat(temp, colonp+1);
    }
  this->ACE_Addr::base_set (AF_SPIPE, 
			    ACE_OS::strlen (temp) + len);

  ACE_OS::strcpy(this->SPIPE_addr_.rendezvous_, temp) ;

#else
  this->ACE_Addr::base_set (AF_SPIPE, ACE_OS::strlen (addr) + len);
  ACE_OS::strncpy (this->SPIPE_addr_.rendezvous_, addr,
		   sizeof this->SPIPE_addr_.rendezvous_);
#endif

  this->SPIPE_addr_.gid_ = gid == 0 ? ACE_OS::getgid () : gid;
  this->SPIPE_addr_.uid_ = uid == 0 ? ACE_OS::getuid () : uid;
  return 0;
}

// Create a ACE_Addr from a ACE_SPIPE pathname. 

ACE_SPIPE_Addr::ACE_SPIPE_Addr (LPCTSTR addr,
				gid_t gid, 
				uid_t uid)
{
  this->set (addr, gid, uid);
}

