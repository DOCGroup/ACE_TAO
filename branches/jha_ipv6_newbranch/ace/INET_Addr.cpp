// $Id$

// Defines the Internet domain address family address format.

#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/INET_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, INET_Addr, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_INET_Addr)

// Transform the current address into string format.

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
          int s = socket(PF_INET6,SOCK_DGRAM,0);
          if(s == -1) {
//	    printf("Setting protocol_family_ to PF_INET\n");
	    ACE_INET_Addr::protocol_family_ = PF_INET;
	  } else {
//	    printf("Setting protocol_family_ to PF_INET6\n");
	    ACE_INET_Addr::protocol_family_ = PF_INET6;
            close(s);
	  }
        }
    }
//    printf("returning protocol_family %d %s from ACE_INET_Addr\n",ACE_INET_Addr::protocol_family_, ACE_INET_Addr::protocol_family_ == PF_INET ? "PF_INET" : "PF_INET6");
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

int
ACE_INET_Addr::addr_to_string (ACE_TCHAR s[],
                               size_t size,
                               int ipaddr_format) const
{
  ACE_TRACE ("ACE_INET_Addr::addr_to_string");

  // XXXXXXXX modify for IPv6 formatting.  Be sure to include
  // the scope identifier in the full address.

  size_t total_len = (ipaddr_format == 0 ?
                      ACE_OS::strlen (this->get_host_name ()) :
                      ACE_OS::strlen (this->get_host_addr ()))
    + ACE_OS::strlen ("65536") // Assume the max port number.
    + sizeof (':')
    + sizeof ('\0'); // For trailing '\0'.

  if (size < total_len)
    return -1;
  else
    {
      ACE_OS::sprintf (s,
                       ACE_LIB_TEXT ("%s:%d"),
                       ACE_TEXT_CHAR_TO_TCHAR (ipaddr_format == 0
                                               ? this->get_host_name ()
                                               : this->get_host_addr ()),
                       this->get_port_number ());
      return 0;
    }
}

void
ACE_INET_Addr::dump (void) const
{
  ACE_TRACE ("ACE_INET_Addr::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));

  ACE_TCHAR s[ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16];
  ACE_OS::sprintf (s, ACE_LIB_TEXT ("%s:%d"),
                   this->get_host_addr (),
                   this->get_port_number ());
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("%s"), s));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Compare two addresses for inequality.

int
ACE_INET_Addr::operator != (const ACE_INET_Addr &sap) const
{
  ACE_TRACE ("ACE_INET_Addr::operator !=");
  return !((*this) == sap);
}

// Compare two addresses for equality.

int
ACE_INET_Addr::operator == (const ACE_INET_Addr &sap) const
{
  ACE_TRACE ("ACE_INET_Addr::operator ==");

  return this->get_port_number() == sap.get_port_number()
      && ACE_OS::memcmp (this->addr_pointer(),
                         sap.addr_pointer(),
                         this->addr_size()) == 0;
}

ACE_INET_Addr::ACE_INET_Addr (void)
  : ACE_Addr (ACE_ADDRESS_FAMILY_INET, this->get_addr_size())
{
  // ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->initialize();
}

int
ACE_INET_Addr::set (const ACE_INET_Addr &sa)
{
  ACE_TRACE ("ACE_INET_Addr::set");

  this->ACE_Addr::base_set (sa.get_type (),
                            sa.get_size ());

  if (sa.get_type () == AF_ANY)
    // Ugh, this is really a base class, so don't copy it.
    this->initialize();
  else
    // It's ok to make the copy.
    (void) ACE_OS::memcpy ((void *) this->get_addr(),
                           (void *) sa.get_addr(),
                           this->get_addr_size());
  return 0;
}

// Transform the string into the current addressing format.

int
ACE_INET_Addr::string_to_addr (const char s[])
{
  ACE_TRACE ("ACE_INET_Addr::string_to_addr");
  int result;
  char *t;

  // Need to make a duplicate since we'll be overwriting the string.
  ACE_ALLOCATOR_RETURN (t,
                        ACE_OS::strdup (s),
                        -1);
  char *ip_addr = ACE_OS::strchr (t, ':');

  if (ip_addr == 0) // Assume it's a port number.
    {
      if (ACE_OS::strspn (t, "1234567890")
          == ACE_OS::strlen (t))
        { // port number
          u_short port = (u_short) ACE_OS::atoi (t);
          result = this->set (port,
                              ACE_UINT32 (INADDR_ANY));
        }
      else // port name
        result = this->set (t,
                            ACE_UINT32 (INADDR_ANY));
    }
  else
    {
      *ip_addr = '\0'; ++ip_addr; // skip over ':'

      if (ACE_OS::strspn (ip_addr, "1234567890") ==
          ACE_OS::strlen (ip_addr))
        {
          u_short port =
            (u_short) ACE_OS::atoi (ip_addr);
          result = this->set (port, t);
        }
      else
        result = this->set (ip_addr, t);
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
  : ACE_Addr (ACE_ADDRESS_FAMILY_INET, this->get_addr_size())
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->set (address);
}

#if defined (ACE_HAS_WCHAR)
ACE_INET_Addr::ACE_INET_Addr (const wchar_t address[])
  : ACE_Addr (ACE_ADDRESS_FAMILY_INET, this->get_addr_size())
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->set (address);
}

#endif /* ACE_HAS_WCHAR */

// Copy constructor.

ACE_INET_Addr::ACE_INET_Addr (const ACE_INET_Addr &sa)
  : ACE_Addr (ACE_ADDRESS_FAMILY_INET, this->get_addr_size())
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
  this->set_address((char*)&inet_address,
                    sizeof inet_address,
                    encode);

  this->set_port_number(port_number,
                        encode);

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

#if defined (ACE_HAS_IPV6)
  return this->set_usinggetaddrinfo (port_number,host_name,PF_UNSPEC,encode);
#else

  ACE_UINT32 addrv4;

  this->ACE_Addr::base_set (ACE_ADDRESS_FAMILY_INET,this->get_addr_size());
  this->initialize();

  // Yow, someone gave us a NULL host_name!
  if (host_name == 0)
    {
      errno = EINVAL;
      return -1;
    }
  else if (ACE_OS::inet_aton (host_name,
                              (struct in_addr *) &addrv4) == 1)
    return this->set (port_number,
                      encode ? ntohl (addrv4) : addrv4,
                      encode);
  else
    {
#if defined (VXWORKS) || defined (CHORUS)
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
          (void) ACE_OS::memcpy ((void *) &addrv4,
                                 hp->h_addr,
                                 hp->h_length);
          return this->set (port_number,
                            encode ? ntohl (addrv4) : addrv4,
                            encode);
        }
    }
#endif /* Top ACE_HAS_IPV6 */
}

#if defined (ACE_HAS_IPV6)
// XXXXX this method should change name to something different.
// Possibly use this as a replacement for the ugly set method
// currently used for IPv4.
int ACE_INET_Addr::set_usinggetaddrinfo (u_short port_number,
                                         const char host_name[],
                                         int address_family,
                                         int encode)
{
  struct addrinfo hints, *res, *res0;
  int error;
  ACE_OS::memset((void*)&hints, 0, sizeof(hints));

  hints.ai_family = address_family;

  error = getaddrinfo(host_name, 0, &hints, &res0);
  if(error) {
    return -1;
  }
  int ret = -1;
  for(res = res0; res != 0; res = res->ai_next) {
    if(res->ai_addrlen == sizeof(sockaddr_in)) {
      sockaddr_in *addr = (sockaddr_in*)res->ai_addr;
      this->set_address((const char*)&addr->sin_addr,sizeof(addr->sin_addr),0);
      this->set_port_number(port_number,encode);
      ret = 0;
      break;
    } else if(res->ai_addrlen == sizeof(sockaddr_in6)) {
      sockaddr_in6 *addr = (sockaddr_in6*)res->ai_addr;
      this->set_addr((void*)&addr->sin6_addr,sizeof(addr->sin6_addr));
      this->set_port_number(port_number,encode);
      ret = 0;
      break;
    }
  }
  freeaddrinfo(res0);
  return ret;
}
#endif

// Initializes a ACE_INET_Addr from a <port_name> and the remote
// <host_name>.

int
ACE_INET_Addr::set (const char port_name[],
                    const char host_name[],
                    const char protocol[])
{
  ACE_TRACE ("ACE_INET_Addr::set");

#if defined (VXWORKS) || defined (CHORUS) || defined (ACE_LACKS_GETSERVBYNAME)
  ACE_UNUSED_ARG (port_name);
  ACE_UNUSED_ARG (host_name);
  ACE_UNUSED_ARG (protocol);
  ACE_NOTSUP_RETURN (-1);
#else
  servent sentry;
  ACE_SERVENT_DATA buf;

  servent *sp = ACE_OS::getservbyname_r (port_name,
                                         protocol,
                                         &sentry,
                                         buf);
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
                    ACE_UINT32 inet_address,
                    const char protocol[])
{
  ACE_TRACE ("ACE_INET_Addr::set");

#if defined (VXWORKS) || defined (CHORUS) || defined (ACE_LACKS_GETSERVBYNAME)
  ACE_UNUSED_ARG (port_name);
  ACE_UNUSED_ARG (inet_address);
  ACE_UNUSED_ARG (protocol);
  ACE_NOTSUP_RETURN (-1);
#else
  servent sentry;
  ACE_SERVENT_DATA buf;

  servent *sp = ACE_OS::getservbyname_r (port_name,
                                         protocol,
                                         &sentry,
                                         buf);
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
  if (this->set (port_number,
                 host_name) == -1)
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
    ACE_ERROR ((LM_ERROR,
                (char *)"ACE_INET_Addr::ACE_INET_Addr: %p\n",
                (((char *) host_name == 0) ?
                 ((char *) "<unknown>") :
                 ((char *) (host_name)))));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_INET_Addr::ACE_INET_Addr: %p\n"),
                ACE_TEXT_CHAR_TO_TCHAR ((host_name == 0) ?
                                        "<unknown>" : host_name)));
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
}

#if defined (ACE_HAS_WCHAR)
ACE_INET_Addr::ACE_INET_Addr (u_short port_number,
                              const wchar_t host_name[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_number,
                 host_name) == -1)
#if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
    ACE_ERROR ((LM_ERROR,
                (char *)"ACE_INET_Addr::ACE_INET_Addr: %p\n",
                (((char *) host_name == 0) ?
                 ((char *) "<unknown>") :
                 ((char *) (host_name)))));
#else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_INET_Addr::ACE_INET_Addr: %p\n"),
                ACE_TEXT_WCHAR_TO_TCHAR ((host_name == 0) ?
                                         ACE_TEXT_WIDE ("<unknown>") :
                                         host_name)));
#endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
}
#endif /* ACE_HAS_WCHAR */

// Creates a ACE_INET_Addr from a sockaddr_in structure.

int
ACE_INET_Addr::set (const sockaddr_in *addr, int len)
{
  ACE_TRACE ("ACE_INET_Addr::set");
  this->ACE_Addr::base_set (ACE_ADDRESS_FAMILY_INET, len);

  this->set_address((char*)&addr->sin_addr,
                    sizeof addr->sin_addr,
                    0);
  this->set_port_number(addr->sin_port,
                        0);

  return 0;
}

// Set a pointer to the address.
void
ACE_INET_Addr::set_addr (void *addr, int len)
{
  ACE_TRACE ("ACE_INET_Addr::set_addr");

  this->ACE_Addr::base_set (ACE_ADDRESS_FAMILY_INET, len);
  ACE_OS::memcpy ((void *) this->addr_pointer(),
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
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_INET_Addr::ACE_INET_Addr")));
}

// Creates a ACE_INET_Addr from a PORT_NAME and the remote
// HOST_NAME.

ACE_INET_Addr::ACE_INET_Addr (const char port_name[],
                              const char host_name[],
                              const char protocol[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_name,
                 host_name,
                 protocol) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_INET_Addr::ACE_INET_Addr")));
}

#if defined (ACE_HAS_WCHAR)
ACE_INET_Addr::ACE_INET_Addr (const wchar_t port_name[],
                              const wchar_t host_name[],
                              const wchar_t protocol[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_name,
                 host_name,
                 protocol) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_INET_Addr::ACE_INET_Addr")));
}
#endif /* ACE_HAS_WCHAR */

// Creates a ACE_INET_Addr from a PORT_NAME and an Internet address.


ACE_INET_Addr::ACE_INET_Addr (const char port_name[],
                              ACE_UINT32 inet_address,
                              const char protocol[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_name,
                 inet_address,
                 protocol) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_INET_Addr::ACE_INET_Addr")));
}

#if defined (ACE_HAS_WCHAR)
ACE_INET_Addr::ACE_INET_Addr (const wchar_t port_name[],
                              ACE_UINT32 inet_address,
                              const wchar_t protocol[])
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  if (this->set (port_name,
                 inet_address,
                 protocol) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("ACE_INET_Addr::ACE_INET_Addr")));
}
#endif /* ACE_HAS_WCHAR */

int
ACE_INET_Addr::get_host_name (char hostname[],
                              size_t len) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_name");

  if (
#if defined (ACE_HAS_IPV6)
      0 == memcmp(this->addr_pointer(),(void*)&in6addr_any,this->addr_size())
#else
      this->inet_addr4_.sin_addr.s_addr == INADDR_ANY
#endif
      )
    {
      if (ACE_OS::hostname (hostname, len) == -1)
        return -1;
      else
        return 0;
    }
  else
    {
#if defined (VXWORKS)
      ACE_UNUSED_ARG (len);
      int error =
        ::hostGetByAddr ((int) this->inet_addr4_.sin_addr.s_addr,
                         hostname);
      if (error == OK)
        return 0;
      else
        {
          errno = error;
          return -1;
        }
#else
      int error = 0;

#if defined (CHORUS)
      hostent *hp = ACE_OS::gethostbyaddr ((char *)this->addr_pointer(),,
                                           this->addr_len(),
                                           this->get_type());
      if (hp == 0)
        error = errno;  // So that the errno gets propagated back; it is
                        // loaded from error below.
#else
      hostent hentry;
      ACE_HOSTENT_DATA buf;
      hostent *hp =
        ACE_OS::gethostbyaddr_r ((char *)this->addr_pointer(),
                                 this->addr_size(),
                                 this->get_type(),
                                 &hentry,
                                 buf,
                                 &error);
#endif /* CHORUS */

      if (hp == 0)
        {
          errno = error;
          return -1;
        }

      if (hp->h_name == 0)
        return -1;

      if (ACE_OS::strlen (hp->h_name) >= len)
        {
          errno = ENOSPC;
          return -1;
        }

      ACE_OS::strcpy (hostname, hp->h_name);
      return 0;
#endif /* VXWORKS */
    }

}

#if defined (ACE_HAS_WCHAR)
int
ACE_INET_Addr::get_host_name (wchar_t hostname[],
                              size_t len) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_name");

  char char_hostname [MAXHOSTNAMELEN + 1];

  // We have a build in limitation of MAXHOSTNAMELEN
  if (len > MAXHOSTNAMELEN + 1)
    len = MAXHOSTNAMELEN + 1;

  // Call the char version
  int result = this->get_host_name (char_hostname, len);

  // And copy it over, if successful
  if (result == 0)
    ACE_OS_String::strcpy (hostname,
                           ACE_Ascii_To_Wide (char_hostname).wchar_rep ());

  return result;
}
#endif /* ACE_HAS_WCHAR */

// Return the character representation of the hostname.

const char *
ACE_INET_Addr::get_host_name (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_name");

  static char name[MAXHOSTNAMELEN + 1];
  if (this->get_host_name (name, MAXHOSTNAMELEN + 1) == -1)
    ACE_OS::strcpy (name, "<unknown>");
  return name;
}

void
ACE_INET_Addr::set_port_number (u_short port_number,
                                int encode)
{
  ACE_TRACE ("ACE_INET_Addr::set_port_number");

  if (encode)
    port_number = htons (port_number);

#if defined (ACE_HAS_IPV6)

#if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    this->inet_addr4_.sin_port = port_number;
  else
#endif
    this->inet_addr6_.sin6_port = port_number;

#else
  this->inet_addr4_.sin_port = port_number;
#endif
}

int ACE_INET_Addr::set_address (const char *ip_addr,
                                int len,
                                int encode /* = 1 */)
{
  ACE_TRACE ("ACE_INET_Addr::set");

  this->ACE_Addr::base_set (ACE_ADDRESS_FAMILY_INET, this->get_addr_size());
#if defined (ACE_HAS_IPV6)

#if defined (ACE_USES_IPV4_IPV6_MIGRATION) 
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    this->inet_addr4_.sin_family = ACE_ADDRESS_FAMILY_INET;
  else
#endif
    this->inet_addr6_.sin6_family = ACE_ADDRESS_FAMILY_INET;

#else
  this->inet_addr4_.sin_family = ACE_ADDRESS_FAMILY_INET;
#endif

  if(len == 4)
    {
      ACE_UINT32 ip4 = *ACE_reinterpret_cast (const ACE_UINT32 *, ip_addr);
      if(encode) {
        ip4 = htonl(ip4);
      }
#if defined (ACE_HAS_IPV6)
      if(ip4 == INADDR_ANY) {
        in6_addr ip6 = in6addr_any;
        memcpy(this->addr_pointer(),(void*)&ip6,this->addr_size());
        return 0;
      }

      /* Build up a 128 bit address.  An IPv4-mapped IPv6 address is
         defined as 0:0:0:0:0:ffff:IPv4_address.  This id defined in RFC 1884 */
      struct {
        ACE_UINT16 prefix[5];
        ACE_UINT16 ffff;
        ACE_UINT32 addr;
      } newaddress = {
        { 0,0,0,0,0 },
          0xffff,
            ip4
            };

      memcpy(this->addr_pointer(),(void*)&newaddress,this->addr_size());
#else /* ACE_HAS_IPV6 */
      memcpy(this->addr_pointer(),(void*)&ip4,this->addr_size());
#endif

      return 0;
    }
#if defined (ACE_HAS_IPV6)
  else
    {
      if(encode)
        printf("It doesn't make sense to encode for IPv6 addresses.\n");
      memcpy(this->addr_pointer(),(void*)ip_addr,this->addr_size());
      return 0;
    }
#endif /* ACE_HAS_IPV6 */

  return -1;
}
const char *
ACE_INET_Addr::get_host_addr (char *dst, int size) const
{
#if defined (ACE_HAS_IPV6)
// XXXX This won't work with the ACE_USES_IPV4_IPV6_MIGRATION turned on
  if(IN6_IS_ADDR_V4MAPPED(&this->inet_addr6_.sin6_addr)) {
    ACE_UINT32 addr;
    addr = this->get_ip_address();
    addr = htonl(addr);
    return ACE_OS::inet_ntop (AF_INET, (const void*)&addr,dst,size);
  }
  const char *ch = ACE_OS::inet_ntop (AF_INET6, (const void*)&this->inet_addr6_.sin6_addr,dst,size);
  return dst;

#elif defined (VXWORKS)
  // It would be nice to be able to encapsulate this into
  // ACE_OS::inet_ntoa(), but that would lead to either inefficiencies
  // on vxworks or lack of thread safety.
  //
  // So, we use the way that vxworks suggests.
  ACE_INET_Addr *ncthis = ACE_const_cast (ACE_INET_Addr *, this);
  inet_ntoa_b (this->inet_addr4_.sin_addr, ncthis->buf_);
  return &buf_[0];
#else /* VXWORKS */
  return ACE_OS::inet_ntoa (this->inet_addr4_.sin_addr);
#endif
}

// Return the dotted Internet address.
const char *
ACE_INET_Addr::get_host_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_addr");
#if defined (ACE_HAS_IPV6)
  // XXX check this with IPV4_IPV6_MIGRATION
  static char buf[INET6_ADDRSTRLEN];
  return this->get_host_addr(buf,INET6_ADDRSTRLEN);
#else
  return ACE_OS::inet_ntoa (this->inet_addr4_.sin_addr);
#endif
}

// Return the 4-byte IP address, converting it into host byte order.

ACE_UINT32
ACE_INET_Addr::get_ip_address (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_ip_address");
#if defined (ACE_HAS_IPV6)
  // XXX check with IPV4_IPV6_MIGRATION
  if(IN6_IS_ADDR_V4MAPPED (&this->inet_addr6_.sin6_addr) ||
     IN6_IS_ADDR_V4COMPAT (&this->inet_addr6_.sin6_addr)    )
    {
      ACE_UINT32 addr;
      // Return the last 32 bits of the address
      char *thisaddrptr = (char*)this->addr_pointer ();
      thisaddrptr += 128/8 - 32/8;
      memcpy ((void*)&addr, (void*)(thisaddrptr), sizeof(addr));
      return ntohl (addr);
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
		  ACE_LIB_TEXT ("ACE_INET_Addr::get_ip_address: address is a IPv6 address not IPv4\n")));
      errno = EAFNOSUPPORT;
      return 0;
    }
#else
  return ntohl (ACE_UINT32 (this->inet_addr4_.sin_addr.s_addr));
#endif
}

// For now keep this in INET_Addr.cpp for easy compiling.  Move them
// into INET_Addr.i for inlining later.
void *ACE_INET_Addr::addr_pointer(void) const
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

size_t ACE_INET_Addr::addr_size(void) const
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

void ACE_INET_Addr::initialize(void)
{
  (void) ACE_OS::memset ((void *) this->get_addr(),
                         0, this->get_addr_size());
#if defined (ACE_HAS_IPV6)

#if defined (ACE_USES_IPV4_IPV6_MIGRATION)
  if(ACE_INET_Addr::protocol_family() == PF_INET)
    this->inet_addr4_.sin_family = ACE_ADDRESS_FAMILY_INET;
  else
#endif
  this->inet_addr6_.sin6_family = ACE_ADDRESS_FAMILY_INET;

#else
  this->inet_addr4_.sin_family = ACE_ADDRESS_FAMILY_INET;
#endif
}
