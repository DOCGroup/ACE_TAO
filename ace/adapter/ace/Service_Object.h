// $Id$

#ifndef ACE_ADAPTER_SERVICE_OBJECT_H
#define ACE_ADAPTER_SERVICE_OBJECT_H
#include "ace/pre.h"

#include "../../Service_Object.h"

class ACE_Service_Type_W : public ACE_Service_Type
{
public:
  ACE_Service_Type_W (const wchar_t *n,
                      ACE_Service_Type_Impl *o,
                      const ACE_SHLIB_HANDLE handle,
                      int active)
    : ACE_Service_Type (ACE_TEXT_WCHAR_TO_CHAR (n), o, handle, active)
  {}

  ~ACE_Service_Type_W (void)
  {}

  const wchar_t *name (void) const
  { return ACE::strnew (ACE_TEXT_CHAR_TO_WCHAR (ACE_Service_Type::name ())); }

  void name (const wchar_t *name)
  { ACE_Service_Type::name (ACE_TEXT_WCHAR_TO_CHAR (name)); }
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_SERVICE_OBJECT_H */
