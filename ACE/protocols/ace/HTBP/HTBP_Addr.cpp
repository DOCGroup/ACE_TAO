#include <cstring>

#include "HTBP_Addr.h"
#include "ace/OS_NS_string.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Creates a ACE_INET_Addr from a PORT_NUMBER and the remote
// HOST_NAME.
ACE::HTBP::Addr::Addr (u_short port_number,
                       char const host_name[],
                       int address_family)
  : ACE_INET_Addr (port_number, host_name, address_family),
    htid_ ()
{
}

ACE::HTBP::Addr::Addr (char const* htid)
  : htid_(htid)
{
}

int
ACE::HTBP::Addr::set (u_short port,
                      char const host[],
                      char const* htid)
{
  if (htid != nullptr && std::strlen (htid) != 0)
    return this->set_htid (htid);
  return this->ACE_INET_Addr::set(port,host);
}

int
ACE::HTBP::Addr::set_htid (char const* htid)
{
  this->htid_ = htid;
  this->set_port_number (0);
  return 0;
}

char const*
ACE::HTBP::Addr::get_htid () const
{
  return this->htid_.c_str();
}

int
ACE::HTBP::Addr::addr_to_string (ACE_TCHAR buffer[],
                                 size_t size,
                                 int ipaddr_format) const
{
  if (this->htid_.length() == 0)
    return this->ACE_INET_Addr::addr_to_string(buffer,size,ipaddr_format);
  if (size < htid_.length())
    return -1;
  ACE_OS::strncpy (buffer,
                   ACE_TEXT_CHAR_TO_TCHAR(htid_.c_str()),
                   size);
  return 0;
}

int
ACE::HTBP::Addr::string_to_addr (char const address[],
                                 int address_family)
{
  //  if (ACE_OS::strchr (address,':'))
    return this->ACE_INET_Addr::string_to_addr(address, address_family);
    //  this->htid_ = address;
    //  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
