// $Id$

#ifndef ACE_ADAPTER_FILE_ADDR_H
#define ACE_ADAPTER_FILE_ADDR_H
#include "ace/pre.h"

#include "../../FILE_Addr.h"

class ACE_FILE_Addr_W : public ACE_FILE_Addr
{
public:
  ACE_FILE_Addr_W (void)
  {}

  ACE_FILE_Addr_W (const ACE_FILE_Addr_W &sa)
    : ACE_FILE_Addr (sa) 
  {}

  int set (const ACE_FILE_Addr_W &sa)
  { return ACE_FILE_Addr::set (sa); }

  ACE_EXPLICIT ACE_FILE_Addr_W (const wchar_t *filename)
    : ACE_FILE_Addr (ACE_TEXT_WCHAR_TO_CHAR (filename))
  {}
  
  int set (const wchar_t *filename)
  { return ACE_FILE_Addr::set (ACE_TEXT_WCHAR_TO_CHAR (filename)); }

  ACE_FILE_Addr_W &operator= (const ACE_FILE_Addr_W &SAP)
  { 
    ACE_FILE_Addr::operator= (SAP);
    return *this;
  }

  virtual int addr_to_string (wchar_t *addr, size_t size) const
  {
    char *caddr;
    ACE_NEW_RETURN (caddr, char [size], -1);
   
    int result = ACE_FILE_Addr::addr_to_string (caddr, size);
    ACE_OS::strcpy (addr, ACE_TEXT_CHAR_TO_WCHAR (caddr));
    delete [] caddr;
    return result;
  }

  int operator == (const ACE_FILE_Addr_W &SAP) const
  { return ACE_FILE_Addr::operator== (SAP); }

  int operator != (const ACE_FILE_Addr_W &SAP) const
  { return ACE_FILE_Addr::operator!= (SAP); }

  const wchar_t *get_path_name (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_FILE_Addr::get_path_name ())); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_FILE_ADDR_H */
