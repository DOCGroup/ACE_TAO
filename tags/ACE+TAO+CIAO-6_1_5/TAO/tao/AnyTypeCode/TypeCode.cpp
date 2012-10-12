// $Id$

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (__ACE_INLINE__)
# include "tao/AnyTypeCode/TypeCode.inl"
#endif /* ! __ACE_INLINE__ */

#include "tao/CDR.h"
#include "tao/ORB_Constants.h"
#include "tao/debug.h"
#include "tao/SystemException.h"

#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::TypeCode::~TypeCode (void)
{
}

bool
CORBA::TypeCode::tao_marshal_kind (TAO_OutputCDR & cdr) const
{
  return cdr << this->kind_;
}

CORBA::Boolean
CORBA::TypeCode::equal (TypeCode_ptr tc) const
{
  if (this == tc)
    {
      return true;
    }
  else if (CORBA::is_nil (tc))
    {
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 13, CORBA::COMPLETED_NO);
    }

  CORBA::TCKind const tc_kind = tc->kind ();

  if (tc_kind != this->kind_)
    return false;

  try
    {
      char const * const tc_id = tc->id ();

      char const * const this_id = this->id ();

      if (ACE_OS::strcmp (this_id, tc_id) != 0)
        return false;

      char const * const tc_name = tc->name ();

      char const * const this_name = this->name ();

      if (ACE_OS::strcmp (this_name, tc_name) != 0)
        return false;
    }
  catch (const ::CORBA::TypeCode::BadKind&)
    {
      // Some TypeCodes do not support the id() and name()
      // operations.  Ignore the failure, and continue equality
      // verification using TypeCode subclass-specific techniques
      // below.
    }

  return this->equal_i (tc);
}

CORBA::Boolean
CORBA::TypeCode::equivalent (TypeCode_ptr tc) const
{
  if (this == tc)
    {
      return true;
    }
  else if (CORBA::is_nil (tc))
    {
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 13, CORBA::COMPLETED_NO);
    }

  CORBA::TypeCode_ptr const mutable_this =
    const_cast<CORBA::TypeCode_ptr> (this);

  CORBA::TypeCode_var unaliased_this = TAO::unaliased_typecode (mutable_this);
  CORBA::TypeCode_var unaliased_tc = TAO::unaliased_typecode (tc);

  CORBA::TCKind const this_kind = unaliased_this->kind ();
  CORBA::TCKind const tc_kind = unaliased_tc->kind ();

  if (tc_kind != this_kind)
    return false;

  try
    {
      char const * const this_id = unaliased_this->id ();
      char const * const tc_id = unaliased_tc->id ();

      if (ACE_OS::strlen (this_id) != 0
          && ACE_OS::strlen (tc_id) != 0)
        {
          return ACE_OS::strcmp (this_id, tc_id) == 0;
        }
    }
  catch (const ::CORBA::TypeCode::BadKind&)
    {
      // Some TypeCodes do not support the id() operation.  Ignore the
      // failure, and continue equivalence verification using TypeCode
      // subclass-specific techniques.
    }

  return unaliased_this->equivalent_i (unaliased_tc.in ());
}

char const *
CORBA::TypeCode::id_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

char const *
CORBA::TypeCode::name_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::ULong
CORBA::TypeCode::member_count_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

char const *
CORBA::TypeCode::member_name_i (CORBA::ULong /* index */) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::member_type_i (CORBA::ULong /* index */) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::Any *
CORBA::TypeCode::member_label_i (CORBA::ULong /* index */) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::discriminator_type_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::Long
CORBA::TypeCode::default_index_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::ULong
CORBA::TypeCode::length_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::content_type_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::UShort
CORBA::TypeCode::fixed_digits_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::UShort
CORBA::TypeCode::fixed_scale_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::Visibility
CORBA::TypeCode::member_visibility_i (CORBA::ULong /* index */) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::ValueModifier
CORBA::TypeCode::type_modifier_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
}

CORBA::TypeCode_ptr
CORBA::TypeCode::concrete_base_type_i (void) const
{
  throw ::CORBA::TypeCode::BadKind ();
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
  throw *this;
}

void
CORBA::TypeCode::Bounds::_tao_encode (TAO_OutputCDR &cdr) const
{
  if (cdr << this->_rep_id ())
    {
      return;
    }

  throw ::CORBA::MARSHAL ();
}

void
CORBA::TypeCode::Bounds::_tao_decode (TAO_InputCDR &)
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
  CORBA::Exception *result = 0;
  ACE_NEW_RETURN (result,
                  CORBA::TypeCode::BadKind (*this),
                  0);
  return result;
}

void
CORBA::TypeCode::BadKind::_raise (void) const
{
  throw *this;
}

void
CORBA::TypeCode::BadKind::_tao_encode (TAO_OutputCDR &cdr) const
{
  if (cdr << this->_rep_id ())
    {
      return;
    }

  throw ::CORBA::MARSHAL ();
}

void
CORBA::TypeCode::BadKind::_tao_decode (TAO_InputCDR &)
{
}

// ---------------------------------------------------------------

bool
TAO::TypeCode::marshal (TAO_OutputCDR & cdr,
                        CORBA::TypeCode_ptr tc,
                        CORBA::ULong offset)
{
  // Marshal the TypeCode TCKind and TypeCode body.
  //
  // Update the offset value in case a recursive TypeCode is being
  // marshaled.

  return
    tc != 0
    && tc->tao_marshal_kind (cdr)
    && tc->tao_marshal (cdr, aligned_offset (offset) + sizeof (CORBA::ULong));
}

CORBA::ULong
TAO::TypeCode::aligned_offset (CORBA::ULong offset)
{
  ptrdiff_t const unaligned_offset =
    static_cast<ptrdiff_t> (offset);

  return
    static_cast<CORBA::ULong> (ACE_align_binary (unaligned_offset,
                                                 ACE_CDR::LONG_ALIGN));
}

// ---------------------------------------------------------------

CORBA::Boolean
operator<< (TAO_OutputCDR & cdr,
            const CORBA::TypeCode_ptr tc)
{
  return TAO::TypeCode::marshal (cdr, tc, 0);
}

// ---------------------------------------------------------------

#if defined (GEN_OSTREAM_OPS)

std::ostream &
operator<< (std::ostream & strm,
            const CORBA::TypeCode_ptr)
{
  return strm << "CORBA::TypeCode";
}

#endif /* GEN_OSTREAM_OPS */

// ---------------------------------------------------------------

CORBA::TypeCode_ptr
TAO::unaliased_typecode (CORBA::TypeCode_ptr tc)
{
  if (CORBA::is_nil (tc))
    {
      throw ::CORBA::BAD_PARAM (CORBA::OMGVMCID | 13, CORBA::COMPLETED_NO);
    }

  CORBA::TCKind tc_kind = tc->kind ();

  if (tc_kind == CORBA::tk_alias)
    {
      CORBA::TypeCode_var tc_content = CORBA::TypeCode::_duplicate (tc);

      // Iterate until we get to the actual unaliased type.
      do
        {
          tc_content = tc_content->content_type ();

          tc_kind = tc_content->kind ();
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
    ::CORBA::release (p);
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

  void
  In_Object_Argument_Cloner_T<CORBA::TypeCode_ptr>::duplicate (
                                                CORBA::TypeCode_ptr p)
  {
    CORBA::TypeCode::_duplicate (p);
  }

  void
  In_Object_Argument_Cloner_T<CORBA::TypeCode_ptr>::release (
                                                CORBA::TypeCode_ptr p)
  {
    CORBA::release (p);
  }
}

// =========================================================

CORBA::TCKind
TAO::unaliased_kind (CORBA::TypeCode_ptr tc)
{
  CORBA::TypeCode_var unaliased_tc = TAO::unaliased_typecode (tc);

  return unaliased_tc->kind ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
