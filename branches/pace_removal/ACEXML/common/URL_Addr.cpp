// $Id$

#include "ace/Log_Msg.h"
#include "ace/Auto_Ptr.h"
#include "ACEXML/common/URL_Addr.h"


ACE_RCSID(common, ACEXML_URL_Addr, "$Id$")

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
  size_t total_len =
    ACE_OS::strlen (ipaddr_format == 0 ?
                    this->get_host_name () :
                    this->get_host_addr ())
    + ACE_OS::strlen ("65536") // Assume the max port number.
    + ACE_OS::strlen (this->get_path_name ())
    + sizeof (':')
    + sizeof ('/')
    + sizeof ('\0'); // For trailing '\0'.

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
ACEXML_URL_Addr::addr_to_string (int ipaddr_format) const
{
  ACEXML_URL_Addr *this_ptr = ACE_const_cast (ACEXML_URL_Addr *,
                                           this);

  size_t size =
    ACE_OS::strlen (ipaddr_format == 0 ?
                    this->get_host_name () :
                    this->get_host_addr ())
    + ACE_OS::strlen ("65536") // Assume the max port number.
    + ACE_OS::strlen (this->get_path_name ())
    + sizeof (':')
    + sizeof ('/')
    + sizeof ('\0'); // For trailing '\0'.

  if (size > this->addr_string_len_)
    {
      ACE_ALLOCATOR_RETURN (this_ptr->addr_string_,
                            (ACEXML_Char *) ACE_OS::realloc ((void *) this->addr_string_,
                                                           size),
                            0);
      this_ptr->addr_string_len_ = size;
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

int
ACEXML_URL_Addr::string_to_addr (const char *s)
{
  if (s == 0)
    return -1;

  const char* http = "http://";
  int http_len = ACE_OS::strlen (http);

  // Check validity of URL
  if (ACE_OS::strncmp (http, s, http_len) != 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Invalid URL %s\n", s), -1);

  const char* url = 0;
  // Get the host name
  for (url = s + http_len; *url != '\0' && *url != ':' && *url != '/'; ++url)
    ;

  int host_len = url - s;
  host_len -= http_len;

  char* host_name = 0;
  ACE_NEW_RETURN (host_name, char[host_len + 1], -1);
  ACE_OS::strncpy (host_name, s + http_len, host_len);
  host_name[host_len] = '\0';
  ACE_Auto_Basic_Array_Ptr<char> cleanup_host_name (host_name);

  // Get the port number (if any)
  unsigned short port = ACE_DEFAULT_HTTP_PORT;
  if (*url == ':')
      port = (unsigned short) ACE_OS::atoi (++url); // Skip over ':'

  // Set the addr
  int result = this->ACE_INET_Addr::set (port, host_name);

  if (result == -1)
    return -1;

  // Get the path name
  const char* path_name = 0;
  if (*url == '\0')
    path_name = "/";
  else
    path_name = url;

  ACE_ALLOCATOR_RETURN (this->path_name_,
                        ACE_TEXT_CHAR_TO_TCHAR (ACE_OS::strdup (path_name)),
                        -1);
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
  ACE_OS::free (ACE_reinterpret_cast (void *,
                                      ACE_const_cast (ACEXML_Char *,
                                                      this->path_name_)));
  ACE_OS::free (ACE_reinterpret_cast (void *,
                                      ACE_const_cast (ACEXML_Char *,
                                                      this->addr_string_)));
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

void
ACEXML_URL_Addr::operator= (const ACEXML_URL_Addr &addr)
{
  if (this->set (addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACEXML_URL_Addr::ACEXML_URL_Addr")));
}

u_long
ACEXML_URL_Addr::hash (void) const
{
  u_long result = this->ACE_INET_Addr::hash ()
    + ACE::hash_pjw (this->get_path_name ());

  return result;
}

int
ACEXML_URL_Addr::operator== (const ACEXML_URL_Addr &addr) const
{
  return ACE_OS::strcmp (addr.get_path_name (),
                         this->get_path_name ()) == 0
    && addr.get_port_number () == this->get_port_number ()
    && addr.get_ip_address () == this->get_ip_address ();
}

int
ACEXML_URL_Addr::operator!= (const ACEXML_URL_Addr &addr) const
{
  return !(*this == addr);
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

const ACEXML_Char *
ACEXML_URL_Addr::get_path_name (void) const
{
  return this->path_name_;
}

ACEXML_URL_Addr::~ACEXML_URL_Addr (void)
{
  ACE_OS::free (ACE_reinterpret_cast (void *,
                                      ACE_const_cast (ACEXML_Char *,
                                                      this->path_name_)));
  ACE_OS::free (ACE_reinterpret_cast (void *,
                                      ACE_const_cast (ACEXML_Char *,
                                                      this->addr_string_)));
  this->path_name_ = 0;
}

int
ACEXML_URL_Addr::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}
