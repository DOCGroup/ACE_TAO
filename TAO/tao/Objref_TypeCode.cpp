// $Id$

#ifndef TAO_OBJREF_TYPECODE_CPP
#define TAO_OBJREF_TYPECODE_CPP

#include "tao/TypeCode/Objref_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/TypeCode/Objref_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

#include "tao/ORB_Core.h"
#include "tao/CDR.h"

#include "ace/Dynamic_Service.h"


template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
bool
TAO::TypeCode::Objref<StringType,
                      Kind,
                      RefCountPolicy>::tao_marshal (TAO_OutputCDR & cdr) const
{
  // A tk_objref TypeCode has a "complex" parameter list type (see
  // Table 15-2 in Section 15.3.5.1 "TypeCode" in the CDR section of
  // the CORBA specification), meaning that it must be marshaled into
  // a CDR encapsulation.

  // Create a CDR encapsulation.
  return
    (cdr << TAO_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER))
    && (cdr << TAO_OutputCDR::from_string (this->attributes_.id (), 0))
    && (cdr << TAO_OutputCDR::from_string (this->attributes_.name (), 0));
}

template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
void
TAO::TypeCode::Objref<StringType, Kind, RefCountPolicy>::tao_duplicate (void)
{
  this->RefCountPolicy::add_ref ();
}

template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
void
TAO::TypeCode::Objref<StringType, Kind, RefCountPolicy>::tao_release (void)
{
  this->RefCountPolicy::remove_ref ();
}

template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Objref<StringType, Kind, RefCountPolicy>::equal_i (
  CORBA::TypeCode_ptr /* tc */
  ACE_ENV_ARG_DECL_NOT_USED) const
{
  // Equality has already been established in the
  // CORBA::TypeCode base class.

  return 1;
}

template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
CORBA::Boolean
TAO::TypeCode::Objref<StringType, Kind, RefCountPolicy>::equivalent_i (
  CORBA::TypeCode_ptr tc
  ACE_ENV_ARG_DECL) const
{
  // We could refactor this code to the CORBA::TypeCode::equivalent()
  // method but doing so would force us to determine the unaliased
  // kind of this TypeCode.  Since we already know the unaliased kind
  // of this TypeCode, choose to optimize away the additional kind
  // unaliasing operation rather than save space.

  CORBA::TCKind const tc_kind =
    TAO::unaliased_kind (tc
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (tc_kind != Kind)
    return 0;

  char const * const this_id = this->attributes_.id ();
  char const * const tc_id   = tc->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ACE_OS::strcmp (this_id, tc_id) != 0)
    return 0;

  return 1;
}

template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::Objref<StringType, Kind, RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return Kind;
}

template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Objref<StringType,
                      Kind,
                      RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  TAO_TypeCodeFactory_Adapter * const adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CORBA::TypeCode::_nil ());
    }

  return
    Objref_Traits<Kind>::create_compact_typecode (adapter,
                                                  this->attributes_.id ()
                                                  ACE_ENV_ARG_PARAMETER);
}


template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
char const *
TAO::TypeCode::Objref<StringType, Kind, RefCountPolicy>::id_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.id ();
}

template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
char const *
TAO::TypeCode::Objref<StringType, Kind, RefCountPolicy>::name_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  // Ownership is retained by the TypeCode, as required by the C++
  // mapping.
  return this->attributes_.name ();
}


#endif  /*  TAO_OBJREF_TYPECODE_CPP */
