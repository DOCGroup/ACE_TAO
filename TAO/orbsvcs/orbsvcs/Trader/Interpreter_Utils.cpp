// $Id$

#include "orbsvcs/Trader/Interpreter_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// TAO_Sequence_Extracter_Base

CORBA::TCKind
TAO_Sequence_Extracter_Base::
sequence_type (CORBA::TypeCode* type_code
               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Seth, why do you use a comma to separate the two statements?
  CORBA::TCKind return_value = CORBA::tk_void,
  type_kind = type_code->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (return_value);

  if (type_kind == CORBA::tk_alias
      || type_kind == CORBA::tk_sequence)
    {
      CORBA::TypeCode_var base = CORBA::TypeCode::_duplicate (type_code);

      for (;;)
        {
          CORBA::TCKind base_kind = base->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (return_value);

          if (base_kind == CORBA::tk_alias)
            {
              base = base->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_CHECK_RETURN (return_value);
            }
          else
            break;
        }

      CORBA::TCKind base_kind = base->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (return_value);

      if (base_kind == CORBA::tk_sequence)
        {
          base = base->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (return_value);

          base_kind = base->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (return_value);
          return_value = base_kind;
        }
    }

  return return_value;
}

TAO_END_VERSIONED_NAMESPACE_DECL
