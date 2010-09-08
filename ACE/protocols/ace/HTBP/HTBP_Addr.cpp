// $Id$

#include "HTBP_Addr.h"
#include "ace/OS_NS_string.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor
ACE::HTBP::Addr::Addr ()
{
}

// Creates a ACE_INET_Addr from a PORT_NUMBER and the remote
// HOST_NAME.

ACE::HTBP::Addr::Addr (u_short port_number,
                       const char host_name[],
                       int address_family)
  : ACE_INET_Addr (port_number, host_name, address_family),
    htid_ ()
{
}

ACE::HTBP::Addr::Addr (const char *htid)
  : htid_(htid)
{
}

ACE::HTBP::Addr::Addr (const ACE::HTBP::Addr &other)
  : ACE_INET_Addr (other),
    htid_(other.htid_)
{

}

// Destructor
ACE::HTBP::Addr::~Addr ()
{
}

int
ACE::HTBP::Addr::set (u_short port,
                      const char host[],
                      const char *htid)
{
  if (htid != 0 && ACE_OS::strlen (htid) != 0)
    return this->set_htid (htid);
  return this->ACE_INET_Addr::set(port,host);
}

int
ACE::HTBP::Addr::set_htid (const char *htid)
{
  this->htid_ = htid;
  this->set_port_number (0);
  return 0;
}

const char *
ACE::HTBP::Addr::get_htid (void) const
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
ACE::HTBP::Addr::string_to_addr (const char address[],
                                 int address_family)
{
  //  if (ACE_OS::strchr (address,':'))
    return this->ACE_INET_Addr::string_to_addr(address, address_family);
    //  this->htid_ = address;
    //  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL
