// $Id$

#include "orbsvcs/Trader/Interpreter_Utils.h"
#include "ace/ACE.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// = Methods to deal with ACE_Hash_Map_Manager.

TAO_String_Hash_Key::TAO_String_Hash_Key (void)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (char * name)
  : CORBA::String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const char * name)
  : CORBA::String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const CORBA::String_var &src)
  : CORBA::String_var (src)
{
}

bool
TAO_String_Hash_Key::operator == (const TAO_String_Hash_Key &hash_key) const
{
  return ACE_OS::strcmp (this->in (), hash_key.in ()) == 0;
}

bool
operator < (const TAO_String_Hash_Key &left,
            const TAO_String_Hash_Key &right)
{
  return ACE_OS::strcmp (left.in (), right.in ()) < 0;
}

u_long
TAO_String_Hash_Key::hash (void) const
{
  u_long ret = ACE::hash_pjw (this->in ());
  return ret;
}

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
