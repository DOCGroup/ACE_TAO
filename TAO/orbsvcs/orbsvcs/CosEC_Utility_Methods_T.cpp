// $Id$

#ifndef TAO_COSEC_UTILITY_METHODS_T_CPP
#define TAO_COSEC_UTILITY_METHODS_T_CPP

#include "CosEC_Utility_Methods_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


template <class T>
T*
CosEC_Utility_Methods<T>::activate (PortableServer::POA_ptr poa,
                                    PortableServer::Servant p_servant,
                                    // this should be ref counted
                                    CORBA_Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var oid =
    poa->activate_object (p_servant,
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  p_servant->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  CORBA::Object_var obj =
    poa->id_to_reference (oid,
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  return T::_narrow (obj.in ());
}


template <class T>
T*
CosEC_Utility_Methods<T>::activate (PortableServer::POA_ptr poa,
                                    PortableServer::Servant p_servant,
                                    const char *servant_id,
                                    CORBA_Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var oid =
    TAO_POA::string_to_ObjectId (servant_id);

  poa->activate_object_with_id (oid,
                                p_servant,
                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  p_servant->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  CORBA::Object_var obj =
    poa->id_to_reference (oid,
                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  return T::_narrow (obj.in ());
}


template <class T>
void
CosEC_Utility_Methods<T>::deactivate (PortableServer::POA_ptr poa,
                                      PortableServer::Servant p_servant,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (p_servant,
                        ACE_TRY_ENV);
  ACE_CHECK;

  // deactivate from the poa.
  poa->deactivate_object (oid,
                          ACE_TRY_ENV);
  ACE_CHECK;
}

template <class T>
T*
CosEC_Utility_Methods<T>::locate (CosNaming::NamingContext_ptr naming,
                                  const char *object_name,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name ref_name (1);
  ref_name.length (1);
  ref_name[0].id =
    CORBA::string_dup (object_name);

  CORBA::Object_var obj =
    naming->resolve (ref_name,
                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  T *t = T::_narrow (obj.in (),
                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (T::_nil ());

  return t;
}

template <class T>
void
CosEC_Utility_Methods<T>::bind (CosNaming::NamingContext_ptr naming,
                                const char *object_name,
                                CORBA::Object_ptr obj,
                                CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (object_name);

  naming->rebind (name,
                  obj,
                  ACE_TRY_ENV);
  ACE_CHECK;
}

template <class T>
void
CosEC_Utility_Methods<T>::unbind (CosNaming::NamingContext_ptr naming,
                                  const char *object_name,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (object_name);

  naming->unbind (name,
                  ACE_TRY_ENV);
  ACE_CHECK;
}

#endif /* TAO_COSEC_UTILITY_METHODS_T_CPP */
