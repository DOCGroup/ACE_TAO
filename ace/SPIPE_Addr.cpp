// SPIPE_Addr.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/SPIPE_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/SPIPE_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, SPIPE_Addr, "$Id$")

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
  (void) ACE_OS::memset ((void *) &this->SPIPE_addr_, 
                         0, 
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

int
ACE_SPIPE_Addr::set (const ACE_SPIPE_Addr &sa)
{
  this->base_set (sa.get_type (), sa.get_size ());

  if (sa.get_type () == AF_ANY)
    (void) ACE_OS::memset ((void *) &this->SPIPE_addr_,
                           0,
                           sizeof this->SPIPE_addr_);
  else
    (void) ACE_OS::memcpy ((void *) &this->SPIPE_addr_, (void *)
                           &sa.SPIPE_addr_,
                           sa.get_size ()); 
  return 0;
}

// Copy constructor.

ACE_SPIPE_Addr::ACE_SPIPE_Addr (const ACE_SPIPE_Addr &sa)
{
  this->set (sa);
}

int
ACE_SPIPE_Addr::set (LPCTSTR addr,
		     gid_t gid, 
		     uid_t uid)
{
  int len = sizeof (this->SPIPE_addr_.uid_);
  len += sizeof (this->SPIPE_addr_.gid_);

#if defined (ACE_WIN32)
  const TCHAR *colonp = ACE_OS::strchr (addr, ':');
  TCHAR temp[BUFSIZ];

  if (colonp == 0) // Assume it's a local name.
    {
      ACE_OS::strcpy (temp, ACE_TEXT ( "\\\\.\\pipe\\"));
      ACE_OS::strcat (temp, addr);
    }
  else
    {
      
      if (ACE_OS::strncmp (addr,
                           ACE_TEXT ("localhost"),
                           ACE_OS::strlen ("localhost")) == 0)
        // change "localhost" to "."
        ACE_OS::strcpy (temp, ACE_TEXT ("\\\\."));
      else
        {
          ACE_OS::strcpy (temp, ACE_TEXT ("\\\\"));

          TCHAR *t;
          
          // We need to allocate a duplicate so that we can write a
          // NUL character into it.
          ACE_ALLOCATOR_RETURN (t, ACE_OS::strdup (addr), -1);

          t[colonp - addr] = ACE_TEXT ('\0');
          ACE_OS::strcpy (temp, t);

          ACE_OS::free (t);
        }

      ACE_OS::strcat (temp, ACE_TEXT ("\\pipe\\"));
      ACE_OS::strcat (temp, colonp + 1);
    }
  this->ACE_Addr::base_set (AF_SPIPE, 
			    ACE_OS::strlen (temp) + len);

  ACE_OS::strcpy (this->SPIPE_addr_.rendezvous_, temp);
#else
  this->ACE_Addr::base_set (AF_SPIPE,
                            ACE_OS::strlen (addr) + len);
  ACE_OS::strncpy (this->SPIPE_addr_.rendezvous_,
                   addr,
		   sizeof this->SPIPE_addr_.rendezvous_);
#endif /* ACE_WIN32 */
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

