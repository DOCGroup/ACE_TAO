// $Id$

#include "tao/AnyTypeCode/Alias_TypeCode_Static.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Alias_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

#include "tao/AnyTypeCode/TypeCode_Traits.h"

#include "tao/ORB_Core.h"
#include "tao/CDR.h"
#include "tao/TypeCodeFactory_Adapter.h"

#include "ace/Dynamic_Service.h"


ACE_RCSID (AnyTypeCode,
           Alias_TypeCode_Static,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong offset) const
{
  // A tk_alias TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.

  TAO_OutputCDR enc;

  // Account for the encoded CDR encapsulation length and byte order.
  //
  // Aligning on an octet since the next value after the CDR
  // encapsulation length will always be the byte order octet/boolean
  // in this case.
  offset = ACE_align_binary (offset + 4,
                             ACE_CDR::OCTET_ALIGN);

  return
    enc << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
    && enc << TAO_OutputCDR::from_string (this->attributes_.id (), 0)
    && enc << TAO_OutputCDR::from_string (this->attributes_.name (), 0)
    && marshal (enc,
                Traits<char const *>::get_typecode (this->content_type_),
                offset + enc.total_length ())
    && cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

void
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::tao_duplicate (void)
{
}

void
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::tao_release (void)
{
}

CORBA::Boolean
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::equal_i (CORBA::TypeCode_ptr tc
                                               ACE_ENV_ARG_DECL) const
{
  // The CORBA::TypeCode base class already verified equality of the
  // base attributes (id and name).  Perform an equality comparison of
  // the content.

  CORBA::TypeCode_var rhs_content_type =
    tc->content_type (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return
    Traits<char const *>::get_typecode (this->content_type_)->equal (
      rhs_content_type.in ()
      ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::equivalent_i (
  CORBA::TypeCode_ptr
  ACE_ENV_ARG_DECL_NOT_USED) const
{
  // Equivalence already verified in the base class
  // CORBA::TypeCode::equivalent() method.

  return true;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  TAO_TypeCodeFactory_Adapter * const adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INITIALIZE (),
                        CORBA::TypeCode::_nil ());
    }

  CORBA::TypeCode_var compact_content_type =
    Traits<char const *>::get_typecode (
      this->content_type_)->get_compact_typecode (
        ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  if (this->kind_ == CORBA::tk_alias)
    {
      return adapter->create_alias_tc (this->attributes_.id (),
                                       "",  /* empty name */
                                       compact_content_type.in ()
                                       ACE_ENV_ARG_PARAMETER);
    }
  else
    {
      return adapter->create_value_box_tc (this->attributes_.id (),
                                           "",  /* empty name */
                                           compact_content_type.in ()
                                           ACE_ENV_ARG_PARAMETER);
    }
}

char const *
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::id_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.id ();
}

char const *
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::name_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.name ();
}

CORBA::TypeCode_ptr
TAO::TypeCode::Alias<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::Null_RefCount_Policy>::content_type_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return
     CORBA::TypeCode::_duplicate (
       Traits<char const *>::get_typecode (this->content_type_));
}

TAO_END_VERSIONED_NAMESPACE_DECL
