// -*- C++ -*-
// $Id$

#include "tao/AnyTypeCode/Objref_TypeCode_Static.h"

#ifndef __ACE_INLINE__
# include "tao/AnyTypeCode/Objref_TypeCode_Static.inl"
#endif  /* !__ACE_INLINE__ */

#include "tao/CDR.h"
#include "tao/TypeCodeFactory_Adapter.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::TypeCode::Objref<char const *, TAO::Null_RefCount_Policy>::tao_marshal (
  TAO_OutputCDR & cdr,
  CORBA::ULong) const
{
  // A tk_objref TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  TAO_OutputCDR enc;

  return
    enc << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
    && enc << TAO_OutputCDR::from_string (this->attributes_.id (), 0)
    && enc << TAO_OutputCDR::from_string (this->attributes_.name (), 0)
    && cdr << static_cast<CORBA::ULong> (enc.total_length ())
    && cdr.write_octet_array_mb (enc.begin ());
}

void
TAO::TypeCode::Objref<char const *,
                      TAO::Null_RefCount_Policy>::tao_duplicate (void)
{
}

void
TAO::TypeCode::Objref<char const *,
                      TAO::Null_RefCount_Policy>::tao_release (void)
{
}

CORBA::Boolean
TAO::TypeCode::Objref<char const *, TAO::Null_RefCount_Policy>::equal_i (
  CORBA::TypeCode_ptr /* tc */) const
{
  // Equality has already been established in the
  // CORBA::TypeCode base class.

  return true;
}

CORBA::Boolean
TAO::TypeCode::Objref<char const *, TAO::Null_RefCount_Policy>::equivalent_i (
  CORBA::TypeCode_ptr) const
{
  // Equivalence already verified in the base class
  // CORBA::TypeCode::equivalent() method.

  return true;
}

CORBA::TypeCode_ptr
TAO::TypeCode::Objref<char const *,
                      TAO::Null_RefCount_Policy>::get_compact_typecode_i (void) const
{
  TAO_TypeCodeFactory_Adapter * const adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ());

  if (adapter == 0)
    {
      throw ::CORBA::INTERNAL ();
    }

  if (this->kind_ == CORBA::tk_abstract_interface)
    {
      return adapter->create_abstract_interface_tc (this->attributes_.id (),
                                                    ""  /* empty name */);
    }
  else if (this->kind_ == CORBA::tk_component)
    {
      return adapter->create_component_tc (this->attributes_.id (),
                                           ""  /* empty name */);
    }
  else if (this->kind_ == CORBA::tk_home)
    {
      return adapter->create_home_tc (this->attributes_.id (),
                                      ""  /* empty name */);
    }
  else if (this->kind_ == CORBA::tk_local_interface)
    {
      return adapter->create_local_interface_tc (this->attributes_.id (),
                                                 ""  /* empty name */);

    }
  else if (this->kind_ == CORBA::tk_native)
    {
      return adapter->create_native_tc (this->attributes_.id (),
                                        ""  /* empty name */);
    }
  else // CORBA::tk_objref
    {
      return adapter->create_interface_tc (this->attributes_.id (),
                                           ""  /* empty name */);
    }
}

char const *
TAO::TypeCode::Objref<char const *, TAO::Null_RefCount_Policy>::id_i (
  void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.id ();
}

char const *
TAO::TypeCode::Objref<char const *, TAO::Null_RefCount_Policy>::name_i (
  void) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.name ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
