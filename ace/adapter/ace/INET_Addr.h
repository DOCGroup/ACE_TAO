// $Id$

#ifndef ACE_ADAPTER_INET_ADDR_H
#define ACE_ADAPTER_INET_ADDR_H
#include "ace/pre.h"

#include "../../INET_Addr.h"

class ACE_INET_Addr_W : public ACE_INET_Addr
{
public:
  ACE_INET_Addr_W (void)
  {}

  ACE_INET_Addr_W (const ACE_INET_Addr &addr)
    : ACE_INET_Addr (addr)
  {}

  ACE_INET_Addr_W (const sockaddr_in *sa, int len)
    : ACE_INET_Addr (sa, len)
  {}

  ACE_INET_Addr_W (u_short port_number,
                   const wchar_t host_name[])
    : ACE_INET_Addr (port_number, ACE_TEXT_WCHAR_TO_CHAR (host_name))
  {}
  
  ACE_EXPLICIT ACE_INET_Addr_W (const wchar_t address[])
    : ACE_INET_Addr (ACE_TEXT_WCHAR_TO_CHAR (address))
  {}

  ACE_INET_Addr_W (u_short port_number,
		   ACE_UINT32 ip_addr = INADDR_ANY)
    : ACE_INET_Addr (port_number, ip_addr)
  {}

  ACE_INET_Addr_W (const wchar_t port_name[],
		   const wchar_t host_name[],
                   const wchar_t protocol[] = L"tcp")
    : ACE_INET_Addr (ACE_TEXT_WCHAR_TO_CHAR (port_name),
                     ACE_TEXT_WCHAR_TO_CHAR (host_name),
                     ACE_TEXT_WCHAR_TO_CHAR (protocol))
  {}

  ACE_INET_Addr_W (const wchar_t port_name[],
		   ACE_UINT32 ip_addr,
                   const wchar_t protocol[] = L"tcp")
    : ACE_INET_Addr (ACE_TEXT_WCHAR_TO_CHAR (port_name),
                     ip_addr,
                     ACE_TEXT_WCHAR_TO_CHAR (protocol))
  {}
  
  ~ACE_INET_Addr_W (void)
  {}

  int set (u_short port_number,
	   const wchar_t host_name[],
           int encode = 1)
  { return ACE_INET_Addr::set (port_number, ACE_TEXT_WCHAR_TO_CHAR (host_name), encode); }

  int set (const wchar_t port_name[],
	   const wchar_t host_name[],
           const wchar_t protocol[] = L"tcp")
  { 
    return ACE_INET_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (port_name), 
                               ACE_TEXT_WCHAR_TO_CHAR (host_name), 
                               ACE_TEXT_WCHAR_TO_CHAR (protocol)); 
  }

  int set (const wchar_t port_name[],
	   ACE_UINT32 ip_addr,
           const wchar_t protocol[] = L"tcp")
  { 
    return ACE_INET_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (port_name), 
                               ip_addr, 
                               ACE_TEXT_WCHAR_TO_CHAR (protocol)); 
  }

  int set (const wchar_t addr[])
  { return ACE_INET_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (addr)); }

  virtual int addr_to_string (wchar_t buffer[],
                              size_t size,
                              int ipaddr_format = 1) const
  { 
    char *cbuf;
    ACE_NEW_RETURN (cbuf, char[size], -1);

    int result = ACE_INET_Addr::addr_to_string (cbuf, size, ipaddr_format);
    ACE_OS::strcpy (buffer, ACE_TEXT_CHAR_TO_WCHAR (cbuf));
    delete [] cbuf;
    return result;
  }

  virtual int string_to_addr (const wchar_t address[])
  { return ACE_INET_Addr::string_to_addr (ACE_TEXT_WCHAR_TO_CHAR (address)); }

  int get_host_name (wchar_t hostname[], size_t hostnamelen) const
  { 
    char *cbuf;
    ACE_NEW_RETURN (cbuf, char[hostnamelen], -1);

    int result = ACE_INET_Addr::get_host_name (cbuf, hostnamelen);
    ACE_OS::strcpy (hostname, ACE_TEXT_CHAR_TO_WCHAR (cbuf));
    delete [] cbuf;
    return result;
  }

  const wchar_t *get_host_name (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_INET_Addr::get_host_name ())); }

  const wchar_t *get_host_addr (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_INET_Addr::get_host_addr ())); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_INET_ADDR_H */
