// $Id$

#ifndef TAO_HOME_TYPECODE_CPP
#define TAO_HOME_TYPECODE_CPP

#include "tao/Home_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/Home_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


template <typename StringType, class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::Home<StringType, RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return CORBA::tk_home;
}

template <typename StringType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Home<StringType, RefCountPolicy>::get_compact_typecode_i (
  ACE_ENV_SINGLE_ARG_DECL) const
{
  TAO_TypeCodeFactory_Adapter * adapter =
    ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>::instance (
        TAO_ORB_Core::typecodefactory_adapter_name ()
      );

  if (adapter == 0)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CORBA::TypeCode::_nil ());
    }

  return adapter->create_home_tc (this->attributes_.id (),
                                  ""  /* empty name */
                                  ACE_ENV_ARG_PARAMETER);
}

#endif  /*  TAO_HOME_TYPECODE_CPP */
