// $Id$

#ifndef TAO_ALIAS_TYPECODE_CPP
#define TAO_ALIAS_TYPECODE_CPP

#include "tao/AnyTypeCode/Alias_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Alias_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

#include "tao/ORB_Core.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"
#include "tao/AnyTypeCode/TypeCode_Traits.h"

#include "ace/Dynamic_Service.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename StringType,
          typename TypeCodeType,
          class RefCountPolicy>
bool
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::tao_marshal (TAO_OutputCDR & cdr,
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
                Traits<StringType>::get_typecode (this->content_type_),
                offset + enc.total_length ())
    && cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
void
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
void
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::equal_i (CORBA::TypeCode_ptr tc) const
{
  // The CORBA::TypeCode base class already verified equality of the
  // base attributes (id and name).  Perform an equality comparison of
  // the content.

  CORBA::TypeCode_var rhs_content_type = tc->content_type ();

  return
    Traits<StringType>::get_typecode (this->content_type_)->equal (
      rhs_content_type.in ());
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::equivalent_i (CORBA::TypeCode_ptr) const
{
  // Equivalence already verified in the base class
  // CORBA::TypeCode::equivalent() method.

  return true;
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::get_compact_typecode_i (void) const
{
  TAO_TypeCodeFactory_Adapter * const adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ());

  if (adapter == 0)
    {
      throw ::CORBA::INITIALIZE ();
    }

  CORBA::TypeCode_var compact_content_type =
    Traits<StringType>::get_typecode (
      this->content_type_)->get_compact_typecode ();

  if (this->kind_ == CORBA::tk_alias)
    {
      return adapter->create_alias_tc (this->attributes_.id (),
                                       "",  /* empty name */
                                       compact_content_type.in ());
    }
  else
    {
      return adapter->create_value_box_tc (this->attributes_.id (),
                                           "",  /* empty name */
                                           compact_content_type.in ());
    }
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
char const *
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::id_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.id ();
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
char const *
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::name_i (void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.name ();
}

template<typename StringType,
         typename TypeCodeType,
         class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     RefCountPolicy>::content_type_i (void) const
{
  return
     CORBA::TypeCode::_duplicate (
       Traits<StringType>::get_typecode (this->content_type_));
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /*  TAO_ALIAS_TYPECODE_CPP */
