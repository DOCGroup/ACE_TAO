// $Id$

#ifndef ACE_ADAPTER_DEV_ADDR_H
#define ACE_ADAPTER_DEV_ADDR_H
#include "ace/pre.h"

#include "../../DEV_Addr.h"

class ACE_DEV_Addr_W : public ACE_DEV_Addr
{
public:
  ACE_DEV_Addr_W (void)
  {}

  ACE_DEV_Addr_W (const ACE_DEV_Addr &sa)
    : ACE_DEV_Addr (sa)
  {}

  ACE_EXPLICIT ACE_DEV_Addr_W (const wchar_t *devname)
    : ACE_DEV_Addr (ACE_TEXT_WCHAR_TO_CHAR (devname))
  {}

  void set (const wchar_t *devname)
  { ACE_DEV_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (devname)); }

  ACE_DEV_Addr &operator= (const ACE_DEV_Addr &sa)
  {
    ACE_DEV_Addr::operator= (sa);
    return *this;
  }

  virtual int addr_to_string (wchar_t *addr, size_t size) const
  { 
    char *caddr;
    ACE_NEW_RETURN (caddr, char [size], -1);

    int result = ACE_DEV_Addr::addr_to_string (caddr, size);
    
    ACE_OS::strcpy (addr, ACE_TEXT_CHAR_TO_WCHAR (caddr));
    delete [] caddr;
    return result;
  }

  int operator == (const ACE_DEV_Addr &SAP) const
  { return ACE_DEV_Addr::operator== (SAP); }

  int operator != (const ACE_DEV_Addr &SAP) const
  { return ACE_DEV_Addr::operator!= (SAP); }

  const wchar_t *get_path_name (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_DEV_Addr::get_path_name ())); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_DEV_ADDR_H */
