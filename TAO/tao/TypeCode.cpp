// $Id$

#include "TypeCode.h"
#include "TypeCodeFactory_Adapter.h"
#include "ORB_Core.h"

#include "ace/Dynamic_Service.h"


ACE_RCSID (tao,
           TypeCode,
           "$Id$")


#if !defined (__ACE_INLINE__)
# include "TypeCode.inl"
#endif /* ! __ACE_INLINE__ */

#include "SystemException.h"
#include "CDR.h"
#include "ORB_Constants.h"

#include "debug.h"

#include "ace/OS_NS_string.h"


CORBA::Boolean
CORBA::TypeCode::equal (TypeCode_ptr tc
                        ACE_ENV_ARG_DECL) const
{
  if (this == tc)
    {
      return true;
    }
  else if (CORBA::is_nil (tc))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 13,
                                          CORBA::COMPLETED_NO),
                        false);
    }

  CORBA::TCKind const tc_kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind != this->kind_)
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

CORBA::Boolean
CORBA::TypeCode::equivalent (TypeCode_ptr tc
                             ACE_ENV_ARG_DECL) const
{
  if (this == tc)
    {
      return true;
    }
  else if (CORBA::is_nil (tc))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 13,
                                          CORBA::COMPLETED_NO),
                        false);
    }

  return this->equivalent_i (tc
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

CORBA::UShort
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

CORBA::TypeCode::Bounds::Bounds (void)
  : CORBA::UserException ("IDL:omg.org/CORBA/TypeCode/Bounds:1.0",
                          "Bounds")
{
}

CORBA::TypeCode::Bounds*
CORBA::TypeCode::Bounds::_downcast (CORBA::Exception *ex)
{
   return dynamic_cast <CORBA::TypeCode::Bounds*> (ex);
}

CORBA::Exception *
CORBA::TypeCode::Bounds::_alloc (void)
{
  CORBA::Exception *retval = 0;
  ACE_NEW_RETURN (retval, ::CORBA::TypeCode::Bounds, 0);
  return retval;
}

CORBA::Exception *
CORBA::TypeCode::Bounds::_tao_duplicate (void) const
{
  CORBA::Exception *result;
  ACE_NEW_RETURN (result,
                  CORBA::TypeCode::Bounds (*this),
                  0);
  return result;
}

void
CORBA::TypeCode::Bounds::_raise (void) const
{
  TAO_RAISE(*this);
}

void
CORBA::TypeCode::Bounds::_tao_encode (TAO_OutputCDR &cdr
                                      ACE_ENV_ARG_DECL) const
{
  if (cdr << this->_rep_id ())
    {
      return;
    }

  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA::TypeCode::Bounds::_tao_decode (TAO_InputCDR &
                                      ACE_ENV_ARG_DECL_NOT_USED)
{
}

// ****************************************************************

CORBA::TypeCode::BadKind::BadKind (void)
  : CORBA::UserException ("IDL:omg.org/CORBA/TypeCode/BadKind:1.0",
                          "BadKind")
{
}

CORBA::TypeCode::BadKind*
CORBA::TypeCode::BadKind::_downcast (CORBA::Exception *ex)
{
  return dynamic_cast <CORBA::TypeCode::BadKind*> (ex);
}

CORBA::Exception *
CORBA::TypeCode::BadKind::_tao_duplicate (void) const
{
  CORBA::Exception *result;
  ACE_NEW_RETURN (result,
                  CORBA::TypeCode::BadKind (*this),
                  0);
  return result;
}

void
CORBA::TypeCode::BadKind::_raise (void) const
{
  TAO_RAISE(*this);
}

void
CORBA::TypeCode::BadKind::_tao_encode (TAO_OutputCDR &cdr
                                       ACE_ENV_ARG_DECL) const
{
  if (cdr << this->_rep_id ())
    {
      return;
    }

  ACE_THROW (CORBA::MARSHAL ());
}

void
CORBA::TypeCode::BadKind::_tao_decode (TAO_InputCDR &
                                       ACE_ENV_ARG_DECL_NOT_USED)
{
}

// ---------------------------------------------------------------

bool
operator<< (TAO_OutputCDR & cdr,
            CORBA::TypeCode_ptr tc)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  if (tc == 0)
    {
      ACE_THROW_RETURN (CORBA::MARSHAL (0,
                                        CORBA::COMPLETED_MAYBE),
                        false);
    }

  CORBA::ULong const kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  return (cdr << kind) && tc->tao_marshal (cdr);
}

// ---------------------------------------------------------------

CORBA::TypeCode_ptr
TAO::unaliased_typecode (CORBA::TypeCode_ptr tc
                         ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (tc))
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (CORBA::OMGVMCID | 13,
                                          CORBA::COMPLETED_NO),
                        tc);
    }

  CORBA::TCKind tc_kind = tc->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (tc);

  if (tc_kind == CORBA::tk_alias)
    {
      CORBA::TypeCode_var tc_content;

      // Iterate until we get to the actual unaliased type.
      do
        {
          tc_content =
            tc_content->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (tc);

          tc_kind = tc_content->kind (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (tc);
        }
      while (tc_kind == CORBA::tk_alias);

      return tc_content._retn ();
    }

  return CORBA::TypeCode::_duplicate (tc);
}

// =========================================================

// Traits specializations for CORBA::TypeCode.
namespace TAO
{
  CORBA::TypeCode_ptr
  Objref_Traits<CORBA::TypeCode>::duplicate (CORBA::TypeCode_ptr p)
  {
    return CORBA::TypeCode::_duplicate (p);
  }

  void
  Objref_Traits<CORBA::TypeCode>::release (CORBA::TypeCode_ptr p)
  {
    CORBA::release (p);
  }

  CORBA::TypeCode_ptr
  Objref_Traits<CORBA::TypeCode>::nil (void)
  {
    return CORBA::TypeCode::_nil ();
  }

  CORBA::Boolean
  Objref_Traits<CORBA::TypeCode>::marshal (CORBA::TypeCode_ptr p,
                                           TAO_OutputCDR & cdr)
  {
    return cdr << p;
  }
}
