// $Id$

#include "TypeCode.h"


ACE_RCSID (tao,
           TypeCode,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "TypeCode.inl"
#endif /* ! __ACE_INLINE__ */


CORBA::Boolean
CORBA::TypeCode::equal (TypeCode_ptr tc
                        ACE_ENV_ARG_DECL) const
{
  if (this == tc)
    return 1;

  CORBA::TCKind const tc_kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::TCKind const this_kind = this->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind != this_kind)
    return 0;

  ACE_TRY
    {
      char const * const tc_id = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      char const * const this_id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (this_id, tc_id) != 0)
        return 0;

      char const * const tc_name = tc->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      char const * const this_name = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (ACE_OS::strcmp (this_name, tc_name) != 0)
        return 0;
    }
  ACE_CATCH (CORBA::TypeCode::BadKind, ex)
    {
      // Some TypeCodes do not support the id() and name()
      // operations.  Ignore the failure, and continue equality
      // verification using TypeCode subclass-specific techniques
      // below.
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (0);

  return this->equal_i (tc
                        ACE_ENV_ARG_PARAMETER);
}


char const *
CORBA::TypeCode::id_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

char const *
CORBA::TypeCode::name_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

CORBA::ULong
CORBA::TypeCode::member_count_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

char const *
CORBA::TypeCode::member_name_i (CORBA::ULong /* index */
                                ACE_ENV_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::member_type_i (CORBA::ULong /* index */
                                ACE_ENV_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                    CORBA::TypeCode::_nil ());
}

CORBA::Any *
CORBA::TypeCode::member_label_i (CORBA::ULong /* index */
                                 ACE_ENV_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                    CORBA::TypeCode::_nil ());
}

CORBA::Long
CORBA::TypeCode::default_index_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

CORBA::ULong
CORBA::TypeCode::length_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::content_type_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                    CORBA::TypeCode::_nil ());
}

CORBA::UShort
CORBA::TypeCode::fixed_digits_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

CORBA::Short
CORBA::TypeCode::fixed_scale_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (), 0);
}

CORBA::Visibility
CORBA::TypeCode::member_visibility_i (CORBA::ULong /* index */
                                      ACE_ENV_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                    CORBA::PRIVATE_MEMBER);
}

CORBA::ValueModifier
CORBA::TypeCode::type_modifier_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                    CORBA::VM_NONE);
}

CORBA::TypeCode_ptr
CORBA::TypeCode::concrete_base_type_i (ACE_ENV_SINGLE_ARG_DECL) const
{
  ACE_THROW_RETURN (CORBA::TypeCode::BadKind (),
                    CORBA::TypeCode::_nil ());
}

// ---------------------------------------------------------------

bool
TAO::operator<< (TAO_OutputCDR & cdr,
                 CORBA::TypeCode const * tc)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  if (tc == 0)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                        CORBA::COMPLETED_MAYBE),
                        false);
    }

  CORBA::ULong const kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  return (cdr << kind) && tc->tao_marshal (cdr);
}

CORBA::TCKind
TAO::unaliased_kind (CORBA::TypeCode_ptr tc
                     ACE_ENV_ARG_DECL)
{
  CORBA::TCKind tc_kind = 0;

  if (CORBA::is_nil (tc))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 13,
                                          CORBA::COMPLETED_NO),
                        tc_kind);
    }

  tc_kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (tc_kind);

  if (tc_kind == CORBA::tk_alias)
    {
      CORBA::TypeCode_var tc_content =
        tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (tc_kind);

      // Iterate until we get to the actual unaliased type.
      do
        {
          tc_content =
            tc_content->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (tc_kind);

          tc_kind = tc_content->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (tc_kind);
        }
      while (tc_kind == CORBA::tk_alias);
    }

  return tc_kind;
}
