#ifndef TAO_UTILS_RIR_NARROW_CPP
#define TAO_UTILS_RIR_NARROW_CPP

#include "RIR_Narrow.h"
#include <stdexcept>

ACE_RCSID (Utils,
           RIR_Narror,
           "$Id$")

template<class T> TAO::Utils::RIR_Narrow<T>::_ptr_type
TAO::Utils::RIR_Narrow<T>::narrow (CORBA::ORB_ptr orb,
                                   char const * id
                                   ACE_ENV_ARG_DECL)

{
  CORBA::Object_var object =
    orb->resolve_initial_references (id
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (T::_nil ());

  return RIR_Narrow<T>::narrow_object (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

template<class T> TAO::Utils::RIR_Narrow<T>::_ptr_type
TAO::Utils::RIR_Narrow<T>::narrow (PortableInterceptor::ORBInitInfo_ptr info,
                                   char const * id
                                   ACE_ENV_ARG_DECL)
{
  CORBA::Object_var object =
    info->resolve_initial_references (id
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (T::_nil ());

  return RIR_Narrow<T>::narrow_object (object.in ()
                                       ACE_ENV_ARG_PARAMETER);
}

template<class T> TAO::Utils::RIR_Narrow<T>::_ptr_type
TAO::Utils::RIR_Narrow<T>::narrow_object (CORBA::Object_ptr object
                                          ACE_ENV_ARG_DECL)
{
  _var_type narrowed_object = T::_narrow (object
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (T::_nil ());

  if (CORBA::is_nil (narrowed_object.in ()))
  {
    ACE_THROW (CORBA::INV_OBJREF ());
  }
  return narrowed_object._retn ();
}

#endif /*TAO_UTILS_RIR_NARROW_CPP*/
