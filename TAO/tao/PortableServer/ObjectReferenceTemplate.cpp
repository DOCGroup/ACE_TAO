// -*- C++ -*-
//
// $Id$

#include "tao/PortableServer/ObjectReferenceTemplate.h"
#include "tao/PortableServer/POA.h"

#include "tao/ORB_Core.h"

ACE_RCSID (PortableServer,
           ObjectReferenceTemplate,
           "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)

TAO_ObjectReferenceTemplate::
TAO_ObjectReferenceTemplate (const char *server_id,
                             const char *orb_id,
                             PortableInterceptor::AdapterName
                             *adapter_name,
                             TAO_POA *poa)
  : server_id_ (server_id),
    orb_id_ (orb_id),
    adapter_name_ (adapter_name),
    poa_ (poa)
{
  /// Constructor
}

TAO_ObjectReferenceTemplate::~TAO_ObjectReferenceTemplate ()
{
  /// Destructor
}

char *
TAO_ObjectReferenceTemplate::server_id (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ACE_const_cast (char *, this->server_id_);
}

char *
TAO_ObjectReferenceTemplate::orb_id (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ACE_const_cast (char *, this->orb_id_);
}

PortableInterceptor::AdapterName *
TAO_ObjectReferenceTemplate::adapter_name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->adapter_name_;
}

CORBA::Object *
TAO_ObjectReferenceTemplate::make_object (const char * intf,
                                          const PortableInterceptor::ObjectId & id
                                          TAO_ENV_ARG_DECL)
{
  CORBA::OctetSeq object_id = id;

  PortableServer::ObjectId *user_id =
    ACE_reinterpret_cast (PortableServer::ObjectId *,
                          &object_id);

  // Create a reference
  CORBA::Object_var object =
    this->poa_->invoke_key_to_object (intf,
                                      *user_id
                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::String_var ior =
    this->poa_->orb_core ().orb ()->object_to_string (object.in ()
                                                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return object._retn ();
}

void
TAO_ObjectReferenceTemplate::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  delete this;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class
TAO_Valuetype_Manager<PortableInterceptor::ObjectReferenceTemplate, PortableInterceptor::ObjectReferenceTemplate_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate
TAO_Valuetype_Manager<PortableInterceptor::ObjectReferenceTemplate, PortableInterceptor::ObjectReferenceTemplate_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* TAO_HAS_INTERCEPTORS == 1 */
