// $Id$

#include "fe_utils.h"

#include "utl_scoped_name.h"
#include "utl_strlist.h"
#include "utl_namelist.h"
#include "utl_identifier.h"

FE_Utils::T_Param_Info::T_Param_Info (void)
  : const_type_ (AST_Expression::EV_none),
    enum_const_type_decl_ (0)
{
}

bool
FE_Utils::duplicate_param_id (T_PARAMLIST_INFO *params)
{
  size_t cur_pos = 0UL;
  size_t size = params->size ();

  for (FE_Utils::T_PARAMLIST_INFO::CONST_ITERATOR i (*params);
       !i.done ();
       i.advance (), ++cur_pos)
    {
      FE_Utils::T_Param_Info *this_one = 0;
      FE_Utils::T_Param_Info *that_one = 0;

      i.next (this_one);

      for (size_t j = cur_pos + 1; j < size; ++j)
        {
          params->get (that_one, j);

          if (this_one->name_ == that_one->name_)
            {
              return true;
            }
        }
    }

  return false;
}

void
FE_Utils::T_ARGLIST::destroy (void)
{
  AST_Decl **d = 0;
  AST_Decl *tmp = 0;

  for (T_ARGLIST::ITERATOR i (this->begin ());
       !i.done ();
       i.advance ())
    {
      i.next (d);
      tmp = *d;

      // These were created on the fly and not part of any scope
      // so we manage their lifetime here.
      if (tmp->node_type () == AST_Decl::NT_const)
        {
          tmp->destroy ();
          delete tmp;
          tmp = 0;
        }
    }
}

