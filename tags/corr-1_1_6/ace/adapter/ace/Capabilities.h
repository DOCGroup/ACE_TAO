// $Id$
#ifndef ACE_ADAPTER_CAPABILITIES_H
#define ACE_ADAPTER_CAPABILITIES_H
#include "ace/pre.h"

#include "../../Capabilities.h"

class ACE_Capabilities_W : public ACE_Capabilities
{
public:
  ACE_Capabilities_W (void) 
  {}

  ~ACE_Capabilities_W (void) 
  {}

  int getval (const wchar_t *ent, ACE_WString &val)
  { 
    ACE_CString cstr;
    int result = ACE_Capabilities::getval (ACE_TEXT_WCHAR_TO_CHAR (ent), cstr); 
    val = ACE_WString (cstr.c_str ());
    return result;
  }

  int getval (const wchar_t *ent, int &val)
  { return ACE_Capabilities::getval (ACE_TEXT_WCHAR_TO_CHAR (ent), val); }

  int getent (const wchar_t *fname, const wchar_t *name)
  { return ACE_Capabilities::getent (ACE_TEXT_WCHAR_TO_CHAR (fname), ACE_TEXT_WCHAR_TO_CHAR (name)); }
};

#include "ace/post.h"
#endif /* __ACE_ADAPTER_CAPABILITIES_H__ */
