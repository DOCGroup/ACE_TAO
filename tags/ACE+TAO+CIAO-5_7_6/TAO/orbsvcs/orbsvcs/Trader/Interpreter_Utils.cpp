// $Id$

#include "orbsvcs/Trader/Interpreter_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO_Sequence_Extracter_Base

CORBA::TCKind
TAO_Sequence_Extracter_Base::
sequence_type (CORBA::TypeCode* type_code)
{
  // @@ Seth, why do you use a comma to separate the two statements?
  CORBA::TCKind return_value = CORBA::tk_void,
  type_kind = type_code->kind ();

  if (type_kind == CORBA::tk_alias
      || type_kind == CORBA::tk_sequence)
    {
      CORBA::TypeCode_var base = CORBA::TypeCode::_duplicate (type_code);

      for (;;)
        {
          CORBA::TCKind base_kind = base->kind ();

          if (base_kind == CORBA::tk_alias)
            {
              base = base->content_type ();
            }
          else
            break;
        }

      CORBA::TCKind base_kind = base->kind ();

      if (base_kind == CORBA::tk_sequence)
        {
          base = base->content_type ();

          base_kind = base->kind ();
          return_value = base_kind;
        }
    }

  return return_value;
}

TAO_END_VERSIONED_NAMESPACE_DECL
