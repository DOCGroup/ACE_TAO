// $Id$

#ifndef ACE_ADAPTER_MEM_ADDR_H
#define ACE_ADAPTER_MEM_ADDR_H
#include "ace/pre.h"

#include "../../MEM_Addr.h"
#include "ace/ACE.h"

class ACE_MEM_Addr_W : public ACE_MEM_Addr
{
public:
  ACE_MEM_Addr_W (void)
  {}

  ACE_MEM_Addr_W (const ACE_MEM_Addr_W &sa)
    : ACE_MEM_Addr (sa)
  {}

  ACE_MEM_Addr_W (u_short port_number)
    : ACE_MEM_Addr (port_number)
  {}

  ACE_EXPLICIT ACE_MEM_Addr_W (const wchar_t port_name[])
    : ACE_MEM_Addr (ACE_TEXT_WCHAR_TO_CHAR (port_name))
  {}

  ~ACE_MEM_Addr_W (void)
  {}

  int set (const wchar_t port_name[])
  { return ACE_MEM_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (port_name)); }

  virtual int addr_to_string (wchar_t buffer[],
                              size_t size,
                              int ipaddr_format = 1) const
  {
    char *cbuf;
    ACE_NEW_RETURN (cbuf, char [size], -1);

    int result = ACE_MEM_Addr::addr_to_string (cbuf, size, ipaddr_format);
    ACE_OS::strcpy (buffer, ACE_TEXT_CHAR_TO_WCHAR (cbuf));
    delete [] cbuf;
    return result;
  }

  virtual int string_to_addr (const wchar_t address[])
  { return ACE_MEM_Addr::string_to_addr (ACE_TEXT_WCHAR_TO_CHAR (address)); }

  int get_host_name (wchar_t hostname[],
                     size_t hostnamelen) const
  {
    char *cbuf;
    ACE_NEW_RETURN (cbuf, char [hostnamelen], -1);

    int result = ACE_MEM_Addr::get_host_name (cbuf, hostnamelen);
    ACE_OS::strcpy (hostname, ACE_TEXT_CHAR_TO_WCHAR (cbuf));
    delete [] cbuf;
    return result;
  }

  const wchar_t *get_host_name (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_MEM_Addr::get_host_name ())); }

  const wchar_t *get_host_addr (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_MEM_Addr::get_host_addr ())); }

  int operator == (const ACE_MEM_Addr_W &SAP) const
  { return ACE_MEM_Addr::operator== (SAP); }
  int operator == (const ACE_INET_Addr &SAP) const
  { return ACE_MEM_Addr::operator== (SAP); }

  int operator != (const ACE_MEM_Addr_W &SAP) const
  { return ACE_MEM_Addr::operator!= (SAP); }
  int operator != (const ACE_INET_Addr &SAP) const
  { return ACE_MEM_Addr::operator!= (SAP); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_MEM_ADDR_H */
