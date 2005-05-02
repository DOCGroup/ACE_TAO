// $Id$

#ifndef TAO_OBJREF_TYPECODE_CPP
#define TAO_OBJREF_TYPECODE_CPP

#include "tao/Objref_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/Objref_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */

#include "tao/ORB_Core.h"
#include "tao/CDR.h"

#include "ace/Dynamic_Service.h"


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

#endif  /*  TAO_OBJREF_TYPECODE_CPP */
