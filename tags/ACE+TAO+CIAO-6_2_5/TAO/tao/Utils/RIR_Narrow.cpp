// $Id$

#ifndef TAO_UTILS_RIR_NARROW_CPP
#define TAO_UTILS_RIR_NARROW_CPP

#include "tao/Utils/RIR_Narrow.h"
#include "tao/SystemException.h"
#include <stdexcept>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class T> typename TAO::Utils::RIR_Narrow<T>::_ptr_type
TAO::Utils::RIR_Narrow<T>::narrow (CORBA::ORB_ptr orb, char const * id)
{
  CORBA::Object_var object =
    orb->resolve_initial_references (id);

  return RIR_Narrow<T>::narrow_object (object.in ());
}

template<class T> typename TAO::Utils::RIR_Narrow<T>::_ptr_type
TAO::Utils::RIR_Narrow<T>::narrow (PortableInterceptor::ORBInitInfo_ptr info,
                                   char const * id)
{
  CORBA::Object_var object = info->resolve_initial_references (id);

  return RIR_Narrow<T>::narrow_object (object.in ());
}

template<class T> typename TAO::Utils::RIR_Narrow<T>::_ptr_type
TAO::Utils::RIR_Narrow<T>::narrow_object (CORBA::Object_ptr object)
{
  _var_type narrowed_object = T::_narrow (object);

  if (CORBA::is_nil (narrowed_object.in ()))
  {
    throw ::CORBA::INV_OBJREF ();
  }
  return narrowed_object._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /*TAO_UTILS_RIR_NARROW_CPP*/
