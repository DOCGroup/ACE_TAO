// $Id$

#ifndef ACE_ADAPTER_ATM_ADDR_H
#define ACE_ADAPTER_ATM_ADDR_H
#include "ace/pre.h"

#include "../../ATM_Addr.h"

class ACE_ATM_Addr_W : public ACE_ATM_Addr
{
public:
  ACE_ATM_Addr_W (unsigned char selector = DEFAULT_SELECTOR)
    : ACE_ATM_Addr (selector)
  {}

  ACE_ATM_Addr_W (const ACE_ATM_Addr_W &addr,
                  unsigned char selector = DEFAULT_SELECTOR)
    : ACE_ATM_Addr (addr, selector)
  {}

  ACE_ATM_Addr_W (const ATM_Addr *addr,
                  unsigned char selector = DEFAULT_SELECTOR)
    : ACE_ATM_Addr (addr, selector)
  {}

  ACE_ATM_Addr_W (const wchar_t sap[],
                  unsigned char selector = DEFAULT_SELECTOR)
    : ACE_ATM_Addr (ACE_TEXT_WCHAR_TO_CHAR (sap), selector)
  {}

  ~ACE_ATM_Addr_W (void)
  {}

  int set (const ACE_ATM_Addr_W &addr,
           unsigned char selector = DEFAULT_SELECTOR)
  { return ACE_ATM_Addr::set (addr, selector); }

  int set (const wchar_t sap[],
           unsigned char selector = DEFAULT_SELECTOR)
  { return ACE_ATM_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (sap), selector); }

  virtual int string_to_addr (const wchar_t sap[])
  { return ACE_ATM_Addr::string_to_addr (ACE_TEXT_WCHAR_TO_CHAR (sap)); }

  virtual int addr_to_string (wchar_t addr[], 
                              size_t addrlen) const
  {
    char *caddr;
    ACE_NEW_RETURN (caddr, char [addrlen], -1);
    
    int result = ACE_ATM_Addr::addr_to_string (caddr, addrlen);
    ACE_OS::strcpy (addr, ACE_TEXT_CHAR_TO_WCHAR (caddr));
    delete [] caddr;
    return result;
  }

  const wchar_t *addr_to_string (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_ATM_Addr::addr_to_string ())); }

  int operator == (const ACE_ATM_Addr_W &SAP) const
  { return ACE_ATM_Addr::operator== (SAP); }

  int operator != (const ACE_ATM_Addr_W &SAP) const
  { return ACE_ATM_Addr::operator!= (SAP); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_ATM_ADDR_H */

