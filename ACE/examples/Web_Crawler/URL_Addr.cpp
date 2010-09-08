// $Id$

#include "URL_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_Memory.h"

ACE_URL_Addr::ACE_URL_Addr (void)
  : path_name_ (0),
    addr_string_ (0),
    addr_string_len_ (0)
{
}

int
ACE_URL_Addr::addr_to_string (ACE_TCHAR *s,
                              size_t size,
                              int ipaddr_format) const
{
  const size_t total_len =
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

const ACE_TCHAR *
ACE_URL_Addr::addr_to_string (int ipaddr_format) const
{
  ACE_URL_Addr *this_ptr = const_cast<ACE_URL_Addr *> (this);

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
                            (ACE_TCHAR *) ACE_OS::realloc ((void *) this->addr_string_,
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
ACE_URL_Addr::string_to_addr (const ACE_TCHAR *s,
                              int /* address_family */)
{
  int result;
  ACE_TCHAR *t;

  // Need to make a duplicate since we'll be overwriting the string.
  ACE_ALLOCATOR_RETURN (t,
                        ACE_OS::strdup (s),
                        -1);


  // First split off the path_name.

  ACE_TCHAR *path_name = ACE_OS::strchr (t, ACE_TEXT ('/'));
  const ACE_TCHAR *name = ACE_TEXT ("index.html");
  if (path_name != 0)
    {
      if (ACE_OS::strlen (path_name + 1) > 0)
        name = path_name + 1;

      *path_name = '\0';
    }

  ACE_ALLOCATOR_RETURN (this->path_name_,
                        // Skip over '/'
                        ACE_OS::strdup (name),
                        -1);

  // Now handle the host address and port number.
  ACE_TCHAR *port_number = ACE_OS::strchr (t, ':');

  if (port_number == 0)
    {
      // Assume it's an ip-address or ip-number.
      result = this->ACE_INET_Addr::set (ACE_DEFAULT_HTTP_PORT,
                                         t);
    }
  else
    {
      *port_number = '\0';
      u_short port = (u_short) ACE_OS::atoi (port_number + 1); // Skip over ':'
      result = this->ACE_INET_Addr::set (port, t);
    }

  ACE_OS::free (ACE_MALLOC_T (t));
  return result;
}

ACE_URL_Addr::ACE_URL_Addr (const ACE_URL_Addr &addr)
  : ACE_INET_Addr (),
    path_name_ (0),
    addr_string_ (0),
    addr_string_len_ (0)
{
  if (this->set (addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_URL_Addr::ACE_URL_Addr")));
}

int
ACE_URL_Addr::set (const ACE_URL_Addr &addr)
{
  ACE_OS::free (reinterpret_cast<void *> (const_cast<ACE_TCHAR *>
                                                      (this->path_name_)));
  ACE_OS::free (reinterpret_cast<void *> (const_cast<ACE_TCHAR *>
                                                      (this->addr_string_)));
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
      this->addr_string_len_ =
        addr.addr_string_len_;
      return 0;
    }
}

void
ACE_URL_Addr::operator= (const ACE_URL_Addr &addr)
{
  if (this->set (addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_URL_Addr::ACE_URL_Addr")));
}

u_long
ACE_URL_Addr::hash (void) const
{
  u_long result = this->ACE_INET_Addr::hash ()
    + ACE::hash_pjw (this->get_path_name ());

  return result;
}

bool
ACE_URL_Addr::operator== (const ACE_URL_Addr &addr) const
{
  return ACE_OS::strcmp (addr.get_path_name (),
                         this->get_path_name ()) == 0
    && addr.get_port_number () == this->get_port_number ()
    && addr.get_ip_address () == this->get_ip_address ();
}

bool
ACE_URL_Addr::operator!= (const ACE_URL_Addr &addr) const
{
  return !(*this == addr);
}

ACE_URL_Addr::ACE_URL_Addr (const ACE_TCHAR *host_name,
                            const ACE_TCHAR *path_name,
                            u_short port)
  : ACE_INET_Addr (port, host_name),
    path_name_ (ACE_OS::strdup (path_name)),
    addr_string_ (0),
    addr_string_len_ (0)
{
}

const ACE_TCHAR *
ACE_URL_Addr::get_path_name (void) const
{
  return this->path_name_;
}

ACE_URL_Addr::~ACE_URL_Addr (void)
{
  ACE_OS::free (reinterpret_cast<void *> (const_cast<ACE_TCHAR *>
                                                      (this->path_name_)));
  ACE_OS::free (reinterpret_cast<void *> (const_cast<ACE_TCHAR *>
                                                      (this->addr_string_)));
  this->path_name_ = 0;
}

int
ACE_URL_Addr::destroy (void)
{
  // Commit suicide.
  delete this;
  return 0;
}
