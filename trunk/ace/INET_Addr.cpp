// $Id$

// Defines the Internet domain address family address format.

#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
#include "ace/INET_Addr.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (ace,
           INET_Addr,
           "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_INET_Addr)

// Transform the current address into string format.

int
ACE_INET_Addr::addr_to_string (ACE_TCHAR s[],
                               size_t size,
                               int ipaddr_format) const
{
  ACE_TRACE ("ACE_INET_Addr::addr_to_string");

  // XXX Can we (should we) include the scope id for IPv6 addresses?

  size_t total_len = (ipaddr_format == 0 ?
                      ACE_OS::strlen (this->get_host_name ()) :
                      ACE_OS::strlen (this->get_host_addr ()))
    + ACE_OS::strlen ("65536") // Assume the max port number.
    + sizeof (':')
#if defined ACE_HAS_IPV6
    + ACE_OS::strlen ("[]")
#endif
    + sizeof ('\0'); // For trailing '\0'.

  if (size < total_len)
    return -1;
  else
    {
      ACE_OS::sprintf (s,
#if defined ACE_HAS_IPV6
                       ACE_LIB_TEXT ("[%s]:%d"),
#else
                       ACE_LIB_TEXT ("%s:%d"),
#endif
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
  this->addr_to_string(s, ACE_MAX_FULLY_QUALIFIED_NAME_LEN + 16);
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

  if (this->get_type () != sap.get_type () ||
      this->get_size () != sap.get_size ()    )
    return 0;

  return (ACE_OS::memcmp (&this->inet_addr_,
                          &sap.inet_addr_,
                          this->get_size ()) == 0);
}

ACE_INET_Addr::ACE_INET_Addr (void)
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  // ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  (void) ACE_OS::memset ((void *) &this->inet_addr_,
                         0,
                         sizeof this->inet_addr_);
}

int
ACE_INET_Addr::set (const ACE_INET_Addr &sa)
{
  ACE_TRACE ("ACE_INET_Addr::set");

  if (sa.get_type () == AF_ANY)
    // Ugh, this is really a base class, so don't copy it.
    ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
  else
    // It's ok to make the copy.
    ACE_OS::memcpy (&this->inet_addr_, &sa.inet_addr_, sa.get_size ());

  return 0;
}

// Transform the string into the current addressing format.

int
ACE_INET_Addr::string_to_addr (const char s[])
{
  ACE_TRACE ("ACE_INET_Addr::string_to_addr");
  int result;
  char *ip_addr;

  // Need to make a duplicate since we'll be overwriting the string.
  ACE_ALLOCATOR_RETURN (ip_addr,
                        ACE_OS::strdup (s),
                        -1);
  // We use strrchr because of IPv6 addresses.
  char *port_p = ACE_OS::strrchr (ip_addr, ':');

  if (port_p == 0) // Assume it's a port number.
    {
      if (ACE_OS::strspn (ip_addr, "1234567890") == ACE_OS::strlen (ip_addr))
        { // port number
          u_short port = (u_short) ACE_OS::atoi (ip_addr);
          result = this->set (port, ACE_UINT32 (INADDR_ANY));
        }
      else // port name
        result = this->set (ip_addr, ACE_UINT32 (INADDR_ANY));
    }
  else
    {
      *port_p = '\0'; ++port_p; // skip over ':'

      if (ACE_OS::strspn (port_p, "1234567890") == ACE_OS::strlen (port_p))
        {
          u_short port = (u_short) ACE_OS::atoi (port_p);
          result = this->set (port, ip_addr);
        }
      else
        result = this->set (port_p, ip_addr);
    }

  ACE_OS::free (ACE_MALLOC_T (ip_addr));
  return result;
}

int
ACE_INET_Addr::set (const char address[])
{
  ACE_TRACE ("ACE_INET_Addr::set");
  return this->string_to_addr (address);
}

ACE_INET_Addr::ACE_INET_Addr (const char address[])
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
  this->set (address);
}

#if defined (ACE_HAS_WCHAR)
ACE_INET_Addr::ACE_INET_Addr (const wchar_t address[])
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
  this->set (address);
}

#endif /* ACE_HAS_WCHAR */

// Copy constructor.

ACE_INET_Addr::ACE_INET_Addr (const ACE_INET_Addr &sa)
  : ACE_Addr (sa.get_type (), sa.get_size())
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->set (sa);
}

// Initializes a ACE_INET_Addr from a PORT_NUMBER and a 32 bit Internet
// address.

int
ACE_INET_Addr::set (u_short port_number,
                    ACE_UINT32 inet_address,
                    int encode)
{
  ACE_TRACE ("ACE_INET_Addr::set");
  this->set_address (ACE_reinterpret_cast (const char *, &inet_address),
                     sizeof inet_address,
                     encode);
  this->set_port_number (port_number, encode);

  return 0;
}


// Initializes a ACE_INET_Addr from a PORT_NUMBER and the remote
// HOST_NAME.

int
ACE_INET_Addr::set (u_short port_number,
                    const char host_name[],
                    int encode,
                    int address_family)
{
  ACE_TRACE ("ACE_INET_Addr::set");

  // Yow, someone gave us a NULL host_name!
  if (host_name == 0)
    {
      errno = EINVAL;
      return -1;
    }

  ACE_OS_String::memset ((void *) &this->inet_addr_,
                         0,
                         sizeof this->inet_addr_);

#if defined (ACE_HAS_IPV6)
  struct addrinfo hints, *res, *res0;
  int error;
  ACE_OS_String::memset (&hints, 0, sizeof (hints));

  hints.ai_family = address_family;

  error = getaddrinfo (host_name, 0, &hints, &res0);
  if (error)
    return -1;

  int ret = -1;
  for (res = res0; res != 0; res = res->ai_next)
    {
      if (res->ai_family == AF_INET || res->ai_family == AF_INET6)
        {
          this->set_type (res->ai_family);
          this->set_addr (res->ai_addr, res->ai_addrlen);
          this->set_port_number (port_number, encode);
          ret = 0;
          break;
        }
    }
  freeaddrinfo (res0);
  return ret;

#else /* ACE_HAS_IPV6 */

  // IPv6 not supported... insure the family is set to IPv4
  address_family = AF_INET;
  this->set_type (address_family);
  this->inet_addr_.in4_.sin_family = address_family;
  struct in_addr addrv4;
  if (ACE_OS::inet_aton (host_name,
                         &addrv4) == 1)
    return this->set (port_number,
                      encode ? ntohl (addrv4.s_addr) : addrv4.s_addr,
                      encode);
  else
    {
#  if defined (VXWORKS) || defined (CHORUS)
      hostent *hp = ACE_OS::gethostbyname (host_name);
#  else
      hostent hentry;
      ACE_HOSTENT_DATA buf;
      int h_errno;  // Not the same as errno!

      hostent *hp = ACE_OS::gethostbyname_r (host_name, &hentry,
                                             buf, &h_errno);
#  endif /* VXWORKS */

      if (hp == 0)
        {
          return -1;
        }
      else
        {
          (void) ACE_OS_String::memcpy ((void *) &addrv4.s_addr,
                                        hp->h_addr,
                                        hp->h_length);
          return this->set (port_number,
                            encode ? ntohl (addrv4.s_addr) : addrv4.s_addr,
                            encode);
        }
    }
#endif /* ACE_HAS_IPV6 */
}


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

  int address_family = PF_UNSPEC;
#  if defined (ACE_HAS_IPV6)
  if (ACE_OS_String::strcmp (protocol, ACE_LIB_TEXT ("tcp6")) == 0)
    address_family = AF_INET6;
#  endif /* ACE_HAS_IPV6 */

  return this->set (sp->s_port, host_name, 0, address_family);
#endif /* VXWORKS */
}

// Initializes a ACE_INET_Addr from a <port_name> and a 32 bit Internet
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
                              const char host_name[],
                              int address_family)
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
  if (this->set (port_number,
                 host_name,
                 1,
                 address_family) == -1)
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
                              const wchar_t host_name[],
                              int address_family)
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
  if (this->set (port_number,
                 host_name,
                 1,
                 address_family) == -1)
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

  if (addr->sin_family == AF_INET)
    {
      ACE_OS::memcpy (&this->inet_addr_.in4_, addr, len);
      this->base_set (AF_INET, len);
      return 0;
    }
#if defined (ACE_HAS_IPV6)
  else if (addr->sin_family == AF_INET6)
    {
      ACE_OS::memcpy (&this->inet_addr_.in6_, addr, len);
      this->base_set (AF_INET6, len);
      return 0;
    }
#endif /* ACE_HAS_IPV6 */

  errno = EAFNOSUPPORT;
  return -1;
}

// Set a pointer to the address.
void
ACE_INET_Addr::set_addr (void *addr, int len)
{
  ACE_TRACE ("ACE_INET_Addr::set_addr");
  ACE_UNUSED_ARG (len);

  struct sockaddr_in *getfamily = ACE_static_cast (struct sockaddr_in *, addr);

  if (getfamily->sin_family == AF_INET)
    {
      this->set_address (ACE_reinterpret_cast (const char*, &getfamily->sin_addr),
                         sizeof (getfamily->sin_addr),
                         0);
    }
#if defined ACE_HAS_IPV6
  else if (getfamily->sin_family == AF_INET6)
    {
      struct sockaddr_in6 *in6 = ACE_static_cast (struct sockaddr_in6*, addr);
      this->set_address (ACE_reinterpret_cast (const char*, &in6->sin6_addr),
                         sizeof (in6->sin6_addr),
                         0);
    }
#endif // ACE_HAS_IPV6
}

// Creates a ACE_INET_Addr from a sockaddr_in structure.


ACE_INET_Addr::ACE_INET_Addr (const sockaddr_in *addr, int len)
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  this->set (addr, len);
}

// Creates a ACE_INET_Addr from a PORT_NUMBER and an Internet address.


ACE_INET_Addr::ACE_INET_Addr (u_short port_number,
                              ACE_UINT32 inet_address)
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
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
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
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
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
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
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
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
  : ACE_Addr (this->determine_type(), sizeof (inet_addr_))
{
  ACE_TRACE ("ACE_INET_Addr::ACE_INET_Addr");
  ACE_OS::memset (&this->inet_addr_, 0, sizeof (this->inet_addr_));
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

  int result;
  if (len > 1)
    {
      result = get_host_name_i(hostname,len);
      if (result < 0)
        {
          if (result == -2)
            {
              result = -1;
              // We know that hostname is nul-terminated
            }
          else
            {
              //result == -1;
              // This could be worse than hostname[len -1] = '\0'?
              hostname[0] = '\0';
            }
        }
    }
  else
    {
      if (len == 1)
        {
          hostname[0] = '\0';
        }
      result = -1;
    }

  return result;
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
  if (this->get_type () == AF_INET6)
    this->inet_addr_.in6_.sin6_port = port_number;
  else
#endif /* ACE_HAS_IPV6 */
  this->inet_addr_.in4_.sin_port = port_number;
}

// returns -2 when the hostname is truncated
int
ACE_INET_Addr::get_host_name_i (char hostname[], size_t len) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_name_i");

#if defined (ACE_HAS_IPV6)
  if ((this->get_type () == PF_INET6 &&
       0 == ACE_OS_String::memcmp (&this->inet_addr_.in6_.sin6_addr,
                                   &in6addr_any,
                                   sizeof (this->inet_addr_.in6_.sin6_addr)))
      ||
      (this->get_type () == PF_INET &&
       this->inet_addr_.in4_.sin_addr.s_addr == INADDR_ANY))
#else
  if (this->inet_addr_.in4_.sin_addr.s_addr == INADDR_ANY)
#endif /* ACE_HAS_IPV6 */
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
        ::hostGetByAddr ((int) this->inet_addr_.in4_.sin_addr.s_addr,
                         hostname);
      if (error == OK)
        return 0;
      else
        {
          errno = error;
          return -1;
        }
#else
#  if defined (CHORUS) || (defined (DIGITAL_UNIX) && defined (__GNUC__))
      hostent *hp = ACE_OS::gethostbyaddr ((char *)this->ip_addr_pointer (),
                                           this->ip_addr_size (),
                                           this->get_type ());
#  else
      int h_errno;  // Not the same as errno!
      hostent hentry;
      ACE_HOSTENT_DATA buf;
      hostent *hp =
        ACE_OS::gethostbyaddr_r ((char *)this->ip_addr_pointer (),
                                 this->ip_addr_size (),
                                 this->get_type (),
                                 &hentry,
                                 buf,
                                 &h_errno);
#  endif /* CHORUS */

      if (hp == 0 || hp->h_name == 0)
        return -1;

      if (ACE_OS::strlen (hp->h_name) >= len)
        {
          // We know the length, so use memcpy
          if (len > 0)
            {
              ACE_OS::memcpy (hostname, hp->h_name, len - 1);
              hostname[len-1]= '\0';
            }
          errno = ENOSPC;
          return -2;  // -2 Means that we have a good string
          // Using errno looks ok, but ENOSPC could be set on
          // other places.
        }

      ACE_OS::strcpy (hostname, hp->h_name);
      return 0;
#endif /* VXWORKS */
    }
}

int ACE_INET_Addr::set_address (const char *ip_addr,
                                int len,
                                int encode /* = 1 */)
{
  ACE_TRACE ("ACE_INET_Addr::set_address");
  // This is really intended for IPv4. If the object is IPv4, or the type
  // hasn't been set but it's a 4-byte address, go ahead. If this is an
  // IPv6 object and <encode> is requested, refuse.
  if (encode && len != 4)
    {
      errno = EAFNOSUPPORT;
      return -1;
    }

  if (len == 4)
    {
      ACE_UINT32 ip4 = *ACE_reinterpret_cast (const ACE_UINT32 *, ip_addr);
      if (encode)
        ip4 = ACE_HTONL (ip4);


      if (this->get_type () == AF_INET) {
        this->base_set (AF_INET, sizeof (this->inet_addr_.in4_));
        this->inet_addr_.in4_.sin_family = AF_INET;
        this->set_size (sizeof (this->inet_addr_.in4_));
        ACE_OS_String::memcpy (&this->inet_addr_.in4_.sin_addr,
                               &ip4,
                               len);
      }
#if defined (ACE_HAS_IPV6)
      // If given an IPv4 address to copy to an IPv6 object, map it to
      // an IPv4-mapped IPv6 address.
      else
        {
          this->base_set (AF_INET6, sizeof (this->inet_addr_.in6_));
          this->inet_addr_.in6_.sin6_family = AF_INET6;
          this->set_size (sizeof (this->inet_addr_.in6_));
          if (ip4 == INADDR_ANY)
            {
              in6_addr ip6 = in6addr_any;
              ACE_OS_String::memcpy (&this->inet_addr_.in6_.sin6_addr,
                                     &ip6,
                                     sizeof (ip6));
              return 0;
            }
          if (ip4 == INADDR_LOOPBACK)
            {
              in6_addr ip6 = in6addr_loopback;
              ACE_OS_String::memcpy (&this->inet_addr_.in6_.sin6_addr,
                                     &ip6,
                                     sizeof (ip6));
              return 0;
            }

          // Build up a 128 bit address.  An IPv4-mapped IPv6 address
          // is defined as 0:0:0:0:0:ffff:IPv4_address.  This is defined
          // in RFC 1884 */
          struct {
            ACE_UINT16 prefix[5];
            ACE_UINT16 ffff;
            ACE_UINT32 addr;
          } newaddress = {
            { 0,0,0,0,0 },
            0xffff,
            ip4
          };

          ACE_OS_String::memcpy (&this->inet_addr_.in6_.sin6_addr,
                                 &newaddress,
                                 sizeof (newaddress));
        }
#endif /* ACE_HAS_IPV6 */

      return 0;
    }   /* end if (len == 4) */
#if defined (ACE_HAS_IPV6)
  else if (len == 16)
    {
      if (this->get_type () != PF_INET6)
        {
          errno = EAFNOSUPPORT;
          return -1;
        }
      // We protect ourselves up above so IPv6 must be possible here.
      this->base_set (AF_INET6, sizeof (this->inet_addr_.in6_));
      this->inet_addr_.in6_.sin6_family = AF_INET6;
      ACE_OS_String::memcpy (&this->inet_addr_.in6_.sin6_addr, ip_addr, len);

      return 0;
    } /* end len == 16 */
  else
    {
      /* unknown or unsupported address length */
      errno = EAFNOSUPPORT;
      return -1;
    }

#endif /* ACE_HAS_IPV6 */

  // Here with an unrecognized length.
  errno = EAFNOSUPPORT;
  return -1;

}


const char *
ACE_INET_Addr::get_host_addr (char *dst, int size) const
{
#if defined (ACE_HAS_IPV6)
  if (this->get_type () == AF_INET6)
    {
      if (IN6_IS_ADDR_V4MAPPED (&this->inet_addr_.in6_.sin6_addr))
        {
          ACE_UINT32 addr;
          addr = this->get_ip_address();
          addr = ACE_HTONL (addr);
          return ACE_OS::inet_ntop (AF_INET, &addr, dst, size);
        }

      const char *ch = ACE_OS::inet_ntop (AF_INET6,
                                          &this->inet_addr_.in6_.sin6_addr,
                                          dst,
                                          size);
      return ch;
    }
#endif /* ACE_HAS_IPV6 */

#if defined (VXWORKS)
  // It would be nice to be able to encapsulate this into
  // ACE_OS::inet_ntoa(), but that would lead to either inefficiencies
  // on vxworks or lack of thread safety.
  //
  // So, we use the way that vxworks suggests.
  ACE_INET_Addr *ncthis = ACE_const_cast (ACE_INET_Addr *, this);
  inet_ntoa_b (this->inet_addr_.in4_.sin_addr, ncthis->buf_);
  return &buf_[0];
#else /* VXWORKS */
  char *ch = ACE_OS::inet_ntoa (this->inet_addr_.in4_.sin_addr);
  ACE_OS::strncpy (dst, ch, size);
  return ch;
#endif
}

// Return the dotted Internet address.
const char *
ACE_INET_Addr::get_host_addr (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_host_addr");
#if defined (ACE_HAS_IPV6)
  static char buf[INET6_ADDRSTRLEN];
  return this->get_host_addr (buf, INET6_ADDRSTRLEN);
#else
  return ACE_OS::inet_ntoa (this->inet_addr_.in4_.sin_addr);
#endif
}

// Return the 4-byte IP address, converting it into host byte order.

ACE_UINT32
ACE_INET_Addr::get_ip_address (void) const
{
  ACE_TRACE ("ACE_INET_Addr::get_ip_address");
#if defined (ACE_HAS_IPV6)
  if (this->get_type () == AF_INET6)
    {
      if (IN6_IS_ADDR_V4MAPPED (&this->inet_addr_.in6_.sin6_addr) ||
          IN6_IS_ADDR_V4COMPAT (&this->inet_addr_.in6_.sin6_addr)    )
        {
          ACE_UINT32 addr;
          // Return the last 32 bits of the address
          char *thisaddrptr = (char*)this->ip_addr_pointer ();
          thisaddrptr += 128/8 - 32/8;
          ACE_OS_String::memcpy (&addr, thisaddrptr, sizeof (addr));
          return ACE_NTOHL (addr);
        }

      ACE_ERROR ((LM_ERROR,
                  ACE_LIB_TEXT ("ACE_INET_Addr::get_ip_address: address is a IPv6 address not IPv4\n")));
      errno = EAFNOSUPPORT;
      return 0;
    }
#endif /* ACE_HAS_IPV6 */
  return ntohl (ACE_UINT32 (this->inet_addr_.in4_.sin_addr.s_addr));
}
