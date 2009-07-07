// $Id$

#include "fe_utils.h"

#include "utl_scoped_name.h"
#include "utl_strlist.h"

FE_Utils::T_Ref_Info::T_Ref_Info (void)
  : name (0),
    params (0)
{
}

FE_Utils::T_Ref_Info::T_Ref_Info (UTL_ScopedName *n,
                                  UTL_StrList *strs)
  : name (n),
    params (strs)
{
}

FE_Utils::T_Ref_Info::~T_Ref_Info (void)
{
  if (this->name != 0)
    {
      this->name->destroy ();
      delete this->name;
      this->name = 0;
    }

  if (this->params != 0)
    {
      this->params->destroy ();
      delete this->params;
      this->params = 0;
    }
}
