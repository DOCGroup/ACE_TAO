#include "URL_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include <new>
#include <utility>

ACE_URL_Addr::ACE_URL_Addr ()
  : path_name_ {},
    addr_string_ {}
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
    {
      return -1;
    }
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
  size_t const size =
    ACE_OS::strlen (ipaddr_format == 0 ?
                    this->get_host_name () :
                    this->get_host_addr ())
    + ACE_OS::strlen ("65536") // Assume the max port number.
    + ACE_OS::strlen (this->get_path_name ())
    + sizeof (':')
    + sizeof ('/')
    + sizeof ('\0'); // For trailing '\0'.

  try
    {
      this->addr_string_.resize (size);
      ACE_OS::sprintf (this->addr_string_.data (),
                       ACE_TEXT ("%s:%d/%s"),
                       ACE_TEXT_CHAR_TO_TCHAR (ipaddr_format == 0
                                               ? this->get_host_name ()
                                               : this->get_host_addr ()),
                       this->get_port_number (),
                       this->get_path_name ());
      this->addr_string_.resize (size - 1);
    }
  catch (const std::bad_alloc &)
    {
      return nullptr;
    }
  return this->addr_string_.c_str ();
}

int
ACE_URL_Addr::string_to_addr (const ACE_TCHAR *s,
                              int /* address_family */)
{
  if (s == nullptr)
  {
    return -1;
  }

  try
    {
      std::basic_string<ACE_TCHAR> parsed_address (s);
      size_t const path_start = parsed_address.find (ACE_TEXT ('/'));
      std::basic_string<ACE_TCHAR> path_name (ACE_TEXT ("index.html"));
      if (path_start != std::basic_string<ACE_TCHAR>::npos)
        {
          if (path_start + 1 < parsed_address.size ())
          {
            path_name = parsed_address.substr (path_start + 1);
          }
          parsed_address.resize (path_start);
        }

      // Now handle the host address and port number.
      size_t const port_start = parsed_address.find (ACE_TEXT (':'));
      int result {};
      if (port_start == std::basic_string<ACE_TCHAR>::npos)
        {
          // Assume it's an ip-address or ip-number.
          result = this->ACE_INET_Addr::set (ACE_DEFAULT_HTTP_PORT,
                                             parsed_address.c_str ());
        }
      else
        {
          u_short const port = static_cast<u_short> (
            ACE_OS::atoi (parsed_address.c_str () + port_start + 1));
          parsed_address.resize (port_start);
          result = this->ACE_INET_Addr::set (port,
                                             parsed_address.c_str ());
        }

      if (result == 0)
      {
        this->path_name_ = std::move (path_name);
      }

      return result;
    }
  catch (const std::bad_alloc &)
    {
      return -1;
    }
}

ACE_URL_Addr::ACE_URL_Addr (const ACE_URL_Addr &addr)
  : ACE_INET_Addr (),
    path_name_ {},
    addr_string_ {}
{
  if (this->set (addr) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("ACE_URL_Addr::ACE_URL_Addr")));
}

int
ACE_URL_Addr::set (const ACE_URL_Addr &addr)
{
  try
    {
      std::basic_string<ACE_TCHAR> path_name (addr.path_name_);
      std::basic_string<ACE_TCHAR> addr_string (addr.addr_string_);
      if (this->ACE_INET_Addr::set (addr) == -1)
      {
        return -1;
      }
      this->path_name_ = std::move (path_name);
      this->addr_string_ = std::move (addr_string);
      return 0;
    }
  catch (const std::bad_alloc &)
    {
      return -1;
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
ACE_URL_Addr::hash () const
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
    path_name_ (path_name == nullptr ? ACE_TEXT ("") : path_name),
    addr_string_ {}
{
}

const ACE_TCHAR *
ACE_URL_Addr::get_path_name () const
{
  return this->path_name_.c_str ();
}

int
ACE_URL_Addr::destroy ()
{
  // Commit suicide.
  delete this;
  return 0;
}
