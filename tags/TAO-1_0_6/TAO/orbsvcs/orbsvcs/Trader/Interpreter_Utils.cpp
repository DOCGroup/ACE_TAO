// $Id$

#include "Interpreter_Utils.h"

// = Methods to deal with ACE_Hash_Map_Manager.

TAO_String_Hash_Key::TAO_String_Hash_Key (void)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (char * name)
  : CORBA_String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const char * name)
  : CORBA_String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const CORBA::String_var &src)
  : CORBA_String_var (src)
{
}

int
TAO_String_Hash_Key::operator == (const TAO_String_Hash_Key &hash_key) const
{
  return ACE_OS::strcmp (this->in (), hash_key.in ()) == 0;
}

int
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

TAO_String_Hash_Key::~TAO_String_Hash_Key (void)
{
}

// TAO_Sequence_Extracter_Base

CORBA::TCKind
TAO_Sequence_Extracter_Base::
sequence_type (CORBA::TypeCode* type_code,
               CORBA::Environment& ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // @@ Seth, why do you use a comma to separate the two statements?
  CORBA::TCKind return_value = CORBA::tk_void,
  type_kind = type_code->kind (ACE_TRY_ENV);
  ACE_CHECK_RETURN (return_value);

  if (type_kind == CORBA::tk_alias
      || type_kind == CORBA::tk_sequence)
    {
      CORBA::TypeCode_var base = CORBA::TypeCode::_duplicate (type_code);

      while (base->kind (ACE_TRY_ENV) == CORBA::tk_alias)
        {
          base = base->content_type (ACE_TRY_ENV);
          ACE_CHECK_RETURN (return_value);
        }

      CORBA::TCKind base_kind = base->kind (ACE_TRY_ENV);
      ACE_CHECK_RETURN (return_value);

      if (base_kind == CORBA::tk_sequence)
        {
          base = base->content_type (ACE_TRY_ENV);
          ACE_CHECK_RETURN (return_value);

          base_kind = base->kind (ACE_TRY_ENV);
          ACE_CHECK_RETURN (return_value);
          return_value = base_kind;
        }
    }

  return return_value;
}
