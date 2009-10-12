// $Id$

#ifndef TAO_EC_LIFETIME_UTILS_T_CPP
#define TAO_EC_LIFETIME_UTILS_T_CPP

#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Lifetime_Utils_T.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename T>
void
activate (T & obj_ref,
          PortableServer::POA_ptr poa,
          PortableServer::ServantBase * servant,
          TAO_EC_Object_Deactivator & suggested_object_deactivator)
{
  // Activate the servant into the POA.
  PortableServer::ObjectId_var obj_id =
    poa->activate_object (servant);

  suggested_object_deactivator.set_values (poa, obj_id.in ());

  // Get the object reference of the activated object.
  CORBA::Object_var obj =
    poa->id_to_reference (obj_id.in ());

  // Don't try to use T::_obj_type::_narrow, some compilers don't like it so
  // do this in two steps
  typedef typename T::_obj_type my_object_type;

  obj_ref =
    my_object_type::_narrow (obj.in());

  if (CORBA::is_nil (obj_ref.in ()))
  {
    throw CORBA::INTERNAL ();
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_EC_LIFETIME_UTILS_T_CPP */
