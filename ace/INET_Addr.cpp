// INET_Addr.cpp
// $Id$

// Defines the Internet domain address family address format. 

#define ACE_BUILD_DLL
#include "ace/INET_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/INET_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_INET_Addr)

// Transform the current address into string format.

int
ACE_INET_Addr::addr_to_string (char s[], size_t) const
{
  ACE_TRACE ("ACE_INET_Addr::addr_to_string");
  // This should check to make sure len is long enough...
  ACE_OS::sprintf (s, "%s:%d",
		   this->get_host_addr (), 
		   this->get_port_number ()); 
  return 0;
}

void
ACE_INET_Addr::dump (void) const
{
  ACE_TRACE ("ACE_INET_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  char s[MAXNAMELEN];
  ACE_OS::sprintf (s, "%s:%d", this->get_host_addr (), this->get_port_number ()); 
  ACE_DEBUG ((LM_DEBUG, "%s", s));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Compare two addresses for inequality.

int
ACE_INET_Addr::operator != (const ACE_Addr &sap) const
{
  ACE_TRACE ("ACE_INET_Addr::operator !=");
  return !((*this) == sap);
}

// Compare two addresses for equality.

int
ACE_INET_Addr::operator == (const ACE_Addr &sap) const
{
  ACE_TRACE ("ACE_INET_Addr::operator ==");
  const sockaddr_in &inet_sap = ((const ACE_INET_Addr &) sap).inet_addr_;

  return this->inet_addr_.sin_port == inet_sap.sin_port
    && ACE_OS::memcmp ((void *) &this->inet_addr_.sin_addr, 
		       (void *) &inet_sap.sin_addr,
		       sizeof (this->inet_addr_.sin_addr)) == 0;
}

ACE_INET_Addr::ACE_INET_Addr (void)
  : ACE_Addr (AF_INET, sizeof this->inet_addr_)
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  (void) ACE_OS::memset ((void *) &this->inet_addr_, 0, 
			 sizeof this->inet_addr_);  
}

int
ACE_INET_Addr::set (const ACE_INET_Addr &sa)
{
  ACE_TRACE ("ACE_INET_Addr::set");
  this->ACE_Addr::base_set (AF_INET, sizeof this->inet_addr_);
  (void) ACE_OS::memcpy ((void *) &this->inet_addr_, 
			 (void *) &sa.inet_addr_, 
			 sizeof this->inet_addr_);
  return 0;
}

// Transform the string into the current addressing format.

int
ACE_INET_Addr::string_to_addr (const char s[])
{
  ACE_TRACE ("ACE_INET_Addr::string_to_addr");
  // Need to make a duplicate since we'll be overwriting the string.
  char *t = ACE_OS::strdup (s);
  if (t == 0)
    return -1;

  char *ip_addr = ACE_OS::strchr (t, ':');
  int result;

  if (ip_addr == 0) // Assume it's a port number.
    {
      u_short port = ACE_OS::atoi (t);
      result = this->set (port, ACE_UINT32 (INADDR_ANY));
    }
  else
    {
      *ip_addr = '\0';
      u_short port = ACE_OS::atoi (ip_addr + 1); // Skip over ':'
      result = this->set (port, t); 
    }
  ACE_OS::free (ACE_MALLOC_T (t));
  return result;
}

int
ACE_INET_Addr::set (const char address[])
{
  ACE_TRACE ("ACE_INET_Addr::set");
  return this->string_to_addr (address);
}

ACE_INET_Addr::ACE_INET_Addr (const char address[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->set (address);
}

// Copy constructor.

ACE_INET_Addr::ACE_INET_Addr (const ACE_INET_Addr &sa)
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->set (sa);
}

// Initializes a ACE_INET_Addr from a PORT_NUMBER and an Internet
// address. 

int
ACE_INET_Addr::set (u_short port_number, 
		    ACE_UINT32 inet_address, 
		    int encode) 
{
  ACE_TRACE ("ACE_INET_Addr::set");
  this->ACE_Addr::base_set (AF_INET, sizeof this->inet_addr_);
  (void) ACE_OS::memset ((void *) &this->inet_addr_, 
			 0, sizeof this->inet_addr_);
  this->inet_addr_.sin_family = AF_INET;
#if defined (ACE_HAS_SIN_LEN)
  this->inet_addr_.sin_len = sizeof this->inet_addr_;
#endif /* ACE_HAS_SIN_LEN */

  if (encode)
    {
      inet_address = htonl (inet_address);
      this->inet_addr_.sin_port = htons (port_number);
    }
  else
    this->inet_addr_.sin_port = port_number;

  (void) ACE_OS::memcpy ((void *) &this->inet_addr_.sin_addr, 
			 (void *) &inet_address,
			 sizeof this->inet_addr_.sin_addr);
  return 0;
}

// Initializes a ACE_INET_Addr from a PORT_NUMBER and the remote
// HOST_NAME. 

int
ACE_INET_Addr::set (u_short port_number, 
		    const char host_name[], 
		    int encode)
{
  ACE_TRACE ("ACE_INET_Addr::set");
  ACE_UINT32 addr;
  
  this->ACE_Addr::base_set (AF_INET, sizeof this->inet_addr_);
  (void) ACE_OS::memset ((void *) &this->inet_addr_, 0, sizeof
			 this->inet_addr_); 

  // Yow, someone gave us a NULL host_name!
  if (host_name == 0)
    {
      errno = EINVAL;
      return -1;
    }
  else if (ACE_OS::inet_aton (host_name, (struct in_addr *) &addr) == 1)
    return this->set (port_number, encode ? ntohl (addr) : addr, encode);

  else 
    {
#if defined (VXWORKS)
      hostent *hp = ACE_OS::gethostbyname (host_name);
#else
      hostent hentry;
      ACE_HOSTENT_DATA buf;
      int error;

      hostent *hp = ACE_OS::gethostbyname_r (host_name, &hentry,
					     buf, &error);
#endif /* VXWORKS */
  
      if (hp == 0)
	{
	  errno = EINVAL;
	  return -1;
	}
      else
	{
	  (void) ACE_OS::memcpy ((void *) &addr, hp->h_addr, hp->h_length); 
	  return this->set (port_number, encode ? ntohl (addr) : addr, encode);
	}
    }
}
 
// Initializes a ACE_INET_Addr from a <port_name> and the remote
// <host_name>. 

int
ACE_INET_Addr::set (const char port_name[], 
		    const char host_name[])
{
  ACE_TRACE ("ACE_INET_Addr::set");

#if defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#else
  servent sentry;
  ACE_SERVENT_DATA buf;

  servent *sp = ACE_OS::getservbyname_r ((char *) port_name, 
					 "tcp", &sentry, buf);
  if (sp == 0)
    return -1;
  else
    return this->set (sp->s_port, host_name, 0);
#endif /* VXWORKS */
}

// Initializes a ACE_INET_Addr from a <port_name> and an Internet
// address.

int
ACE_INET_Addr::set (const char port_name[], 
		    ACE_UINT32 inet_address)
{
  ACE_TRACE ("ACE_INET_Addr::set");

#if defined (VXWORKS)
  ACE_NOTSUP_RETURN (-1);
#else
  servent sentry;
  ACE_SERVENT_DATA buf;

  servent *sp = ACE_OS::getservbyname_r ((char *) port_name, 
					 "tcp", &sentry, buf);
  if (sp == 0)
    return -1;  
  else
    return this->set (sp->s_port, inet_address, 0);
#endif /* VXWORKS */
}

// Creates a ACE_INET_Addr from a PORT_NUMBER and the remote
// HOST_NAME. 


ACE_INET_Addr::ACE_INET_Addr (u_short port_number, 
			      const char host_name[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_number, host_name) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_INET_Addr::ACE_INET_Addr"));
}

// Creates a ACE_INET_Addr from a sockaddr_in structure.

int
ACE_INET_Addr::set (const sockaddr_in *addr, int len)
{
  ACE_TRACE ("ACE_INET_Addr::set");
  this->ACE_Addr::base_set (AF_INET, len);
  ACE_OS::memcpy ((void *) &this->inet_addr_, 
		  (void *) addr, len);
  return 0;
}

// Set a pointer to the address.
void 
ACE_INET_Addr::set_addr (void *addr, int len)
{
  ACE_TRACE ("ACE_INET_Addr::set_addr");

  this->ACE_Addr::base_set (AF_INET, len);
  ACE_OS::memcpy ((void *) &this->inet_addr_, 
		  (void *) addr, len);
}

// Creates a ACE_INET_Addr from a sockaddr_in structure.


ACE_INET_Addr::ACE_INET_Addr (const sockaddr_in *addr, int len)
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->set (addr, len);
}

// Creates a ACE_INET_Addr from a PORT_NUMBER and an Internet address.


ACE_INET_Addr::ACE_INET_Addr (u_short port_number, 
			      ACE_UINT32 inet_address)
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_number, inet_address) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_INET_Addr::ACE_INET_Addr"));
}

// Creates a ACE_INET_Addr from a PORT_NAME and the remote
// HOST_NAME.

ACE_INET_Addr::ACE_INET_Addr (const char port_name[], 
			      const char host_name[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_name, host_name) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_INET_Addr::ACE_INET_Addr"));
}

// Creates a ACE_INET_Addr from a PORT_NAME and an Internet address.


ACE_INET_Addr::ACE_INET_Addr (const char* port_name, 
			      ACE_UINT32 inet_address)
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_name, inet_address) == -1)
    ACE_ERROR ((LM_ERROR, "ACE_INET_Addr::ACE_INET_Addr"));
}

int
ACE_INET_Addr::get_host_name (char hostname[], size_t len) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_name");
#if defined (VXWORKS)
  char name [MAXHOSTNAMELEN + 1];
  int error = ::hostGetByAddr ((int) this->inet_addr_.sin_addr.s_addr, name);
  if (error == OK) 
    {
      if (ACE_OS::strlen (name) >= len) 
        return -1;
      else 
	{
	  ACE_OS::strcpy (hostname, name);
	  return 0;
	}
    } 
  else 
    {
      errno = error;
      return -1;
    }
#else
  int a_len = sizeof this->inet_addr_.sin_addr.s_addr;

  hostent hentry;
  int error;
  ACE_HOSTENT_DATA buf;
  hostent *hp;

  hp = ACE_OS::gethostbyaddr_r ((char *) &this->inet_addr_.sin_addr,
				a_len, this->addr_type_,
				&hentry, buf, &error);
  if (hp == 0)
    {
      errno = error;
      return -1;
    }
  else
    {
      if (ACE_OS::strlen (hp->h_name) >= len)
        return -1;
      else
	{
	  ACE_OS::strcpy (hostname, hp->h_name);
	  return 0;
	}
    }
#endif /* VXWORKS */
}

// Return the character representation of the hostname.

const char *
ACE_INET_Addr::get_host_name (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_name");

#if defined (VXWORKS)
  static char buf[MAXHOSTNAMELEN + 1];

  ::gethostname (buf, MAXHOSTNAMELEN + 1);
  return buf;
#else
  int a_len = sizeof this->inet_addr_.sin_addr.s_addr;

  hostent *hp = ACE_OS::gethostbyaddr ((char *) &this->inet_addr_.sin_addr,
				       a_len, this->addr_type_);

  if (hp == 0)
    return 0;
  else
    return hp->h_name;
#endif /* VXWORKS */
}

void
ACE_INET_Addr::set_port_number (u_short port_number, 
				int encode)
{
  ACE_TRACE ("ACE_INET_Addr::set_port_number");

  if (encode)
    port_number = htons (port_number);

  this->inet_addr_.sin_port = port_number;
}
