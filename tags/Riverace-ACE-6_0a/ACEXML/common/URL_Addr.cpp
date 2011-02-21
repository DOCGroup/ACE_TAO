// $Id$

#include "ACEXML/common/URL_Addr.h"



#if !defined (__ACEXML_INLINE__)
#include "ACEXML/common/URL_Addr.inl"
#endif /* __ACEXML_INLINE__ */

#include "ace/Log_Msg.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"

ACEXML_URL_Addr::ACEXML_URL_Addr (void)
  : path_name_ (0),
    addr_string_ (0),
    addr_string_len_ (0)
{
}

int
ACEXML_URL_Addr::addr_to_string (ACEXML_Char *s,
                                 size_t size,
                                 int ipaddr_format) const
{
  size_t total_len = this->calculate_length (ipaddr_format);
  if (size < total_len)
    return -1;
  else
    {
      ACE_OS::sprintf (s, ACE_TEXT ("%s:%d/%s"),
                       ACE_TEXT_CHAR_TO_TCHAR (ipaddr_format == 0
                                               ? this->get_host_name ()
                                               : this->get_host_addr ()),
                       this->get_port_number (),
                       this->get_path_name ());
      return 0;
    }
}

const ACEXML_Char *
ACEXML_URL_Addr::addr_to_string (int ipaddr_format)
{
  size_t size = this->calculate_length (ipaddr_format);
  if (size > this->addr_string_len_)
    {
      ACE_ALLOCATOR_RETURN (this->addr_string_,
                            (ACEXML_Char *) ACE_OS::realloc(this->addr_string_,
                                                            size), 0);
      this->addr_string_len_ = size;
    }
  ACE_OS::sprintf (this->addr_string_,
                   ACE_TEXT ("%s:%d/%s"),
                   ACE_TEXT_CHAR_TO_TCHAR (ipaddr_format == 0
                                           ? this->get_host_name ()
                                          : this->get_host_addr ()),
                   this->get_port_number (),
                   this->get_path_name ());
  return this->addr_string_;
}

#if defined (ACE_USES_WCHAR)
int
ACEXML_URL_Addr::string_to_addr (const char* s,
                                 int address_family)
{
  return this->string_to_addr (ACE_TEXT_CHAR_TO_TCHAR (s), address_family);
}
#endif /* ACE_USES_WCHAR */

int
ACEXML_URL_Addr::string_to_addr (const ACEXML_Char* s,
                                 int /*address_family */)
{
  if (s == 0)
    return -1;

  const ACEXML_Char* http = ACE_TEXT ("http://");
  size_t http_len = ACE_OS::strlen (http);

  // Check validity of URL
  if (ACE_OS::strncmp (http, s, http_len) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Invalid URL %s\n"), s), -1);

  const ACEXML_Char* url = 0;
  // Get the host name
  for (url = s + http_len; *url != '\0' && *url != ':' && *url != '/'; ++url)
    ;

  size_t host_len = url - s;
  host_len -= http_len;

  ACEXML_Char* host_name = 0;
  ACE_NEW_RETURN (host_name, ACEXML_Char[host_len + 1], -1);
  ACE_OS::strncpy (host_name, s + http_len, host_len);
  host_name[host_len] = '\0';
  ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_host_name (host_name);

  // Get the port number (if any)
  unsigned short port = ACE_DEFAULT_HTTP_PORT;
  if (*url == ':')
    {
      port = (unsigned short) ACE_OS::strtol (++url, 0, 10); // Skip over ':'
      while ( *url != '\0' && *url != '/' )
        ++url;
    }

  // Set the addr
  int result = this->ACE_INET_Addr::set (port, host_name);

  if (result == -1)
    return -1;

  // Get the path name
  const ACEXML_Char* path_name = 0;
  if (*url == '\0')
    path_name = ACE_TEXT ("/");
  else
    path_name = url;

  ACE_ALLOCATOR_RETURN (this->path_name_, ACE_OS::strdup (path_name), -1);
  return result;
}

ACEXML_URL_Addr::ACEXML_URL_Addr (const ACEXML_URL_Addr &addr)
  : ACE_INET_Addr (),
    path_name_ (0),
    addr_string_ (0),
    addr_string_len_ (0)
{
  if (this->set (addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACEXML_URL_Addr::ACEXML_URL_Addr")));
}

int
ACEXML_URL_Addr::set (const ACEXML_URL_Addr &addr)
{
  ACE_OS::free (this->path_name_);
  ACE_OS::free (this->addr_string_);
  if (this->ACE_INET_Addr::set (addr) == -1)
    return -1;
  else
    {
      if (addr.path_name_)
        ACE_ALLOCATOR_RETURN (this->path_name_,
                              ACE_OS::strdup (addr.path_name_),
                              -1);
      if (addr.addr_string_)
        ACE_ALLOCATOR_RETURN (this->addr_string_,
                              ACE_OS::strdup (addr.addr_string_),
                              -1);
      this->addr_string_len_ = addr.addr_string_len_;
      return 0;
    }
}

ACEXML_URL_Addr::ACEXML_URL_Addr (const ACEXML_Char *host_name,
                            const ACEXML_Char *path_name,
                            unsigned short port)
  : ACE_INET_Addr (port, host_name),
    path_name_ (ACE_OS::strdup (path_name)),
    addr_string_ (0),
    addr_string_len_ (0)
{
}

ACEXML_URL_Addr::~ACEXML_URL_Addr (void)
{
  ACE_OS::free (this->path_name_);
  ACE_OS::free (this->addr_string_);
  this->path_name_ = 0;
}
