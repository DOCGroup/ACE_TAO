// $Id$

#ifndef TAO_LOCAL_INTERFACE_TYPECODE_CPP
#define TAO_LOCAL_INTERFACE_TYPECODE_CPP

#include "tao/Local_Interface_TypeCode.h"

#ifndef __ACE_INLINE__
# include "tao/Local_Interface_TypeCode.inl"
#endif  /* !__ACE_INLINE__ */


template <typename StringType, class RefCountPolicy>
CORBA::TCKind
TAO::TypeCode::Local_Interface<StringType, RefCountPolicy>::kind_i (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED) const
{
  return CORBA::tk_local_interface;
}

template <typename StringType, class RefCountPolicy>
CORBA::TypeCode_ptr
TAO::TypeCode::Local_Interface<StringType,
                               RefCountPolicy>::get_compact_typecode_i (
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

  return adapter->create_local_interface_tc (this->attributes_.id (),
                                             ""  /* empty name */
                                             ACE_ENV_ARG_PARAMETER);
}

#endif  /*  TAO_LOCAL_INTERFACE_TYPECODE_CPP */
