// $Id$

#ifndef ACE_ADAPTER_SPIPE_ADDR_H
#define ACE_ADAPTER_SPIPE_ADDR_H
#include "ace/pre.h"

#include "../../SPIPE_Addr.h"

class ACE_SPIPE_Addr_W : public ACE_SPIPE_Addr
{
public:
  ACE_SPIPE_Addr_W (void)
  {}
  
  ACE_SPIPE_Addr_W (const ACE_SPIPE_Addr_W &sa)
    : ACE_SPIPE_Addr (sa)
  {}

  ACE_SPIPE_Addr_W (const wchar_t *rendezvous_point, gid_t g = 0, uid_t u = 0)
    : ACE_SPIPE_Addr (ACE_TEXT_WCHAR_TO_CHAR (rendezvous_point), g, u)
  {}

  int set (const ACE_SPIPE_Addr_W &sa)
  { return ACE_SPIPE_Addr::set (sa); }

  int set (const wchar_t *rendezvous_point, gid_t g = 0, uid_t u = 0)
  { return ACE_SPIPE_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (rendezvous_point), g, u); }

  virtual int addr_to_string (wchar_t *addr, size_t size) const
  { 
    char *caddr;
    ACE_NEW_RETURN (caddr, char [size], -1);

    int result = ACE_SPIPE_Addr::addr_to_string (caddr, size);
    
    ACE_OS::strcpy (addr, ACE_TEXT_CHAR_TO_WCHAR (caddr));
    delete [] caddr;
    return result;
  }

  virtual int string_to_addr (const wchar_t *addr)
  { return ACE_SPIPE_Addr::string_to_addr (ACE_TEXT_WCHAR_TO_CHAR (addr)); }

  // = Equality/inequality tests
  int operator == (const ACE_SPIPE_Addr_W &SAP) const
  { return ACE_SPIPE_Addr::operator== (SAP); }

  int operator != (const ACE_SPIPE_Addr_W &SAP) const
  { return ACE_SPIPE_Addr::operator!= (SAP); }

  const wchar_t *get_path_name (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_SPIPE_Addr::get_path_name ())); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_SPIPE_ADDR_H */
