// $Id$

#include "Object_Factory_i.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/ObjectReferenceTemplate.h"

Object_Factory_i::Object_Factory_i (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr gateway_poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    gateway_poa_ (gateway_poa)
{
  /// Constructor
}


CORBA::Object_ptr
Object_Factory_i::create_object (const char *interface_repository_id,
                                 CORBA::Object_ptr gatewayed_object
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var stringified_object =
    this->orb_->object_to_string (gatewayed_object ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  const PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (stringified_object.in ());

  const PortableInterceptor::ObjectId *obj_id =
    ACE_reinterpret_cast (const PortableInterceptor::ObjectId *,
                          &id.in ());

  TAO_POA *poa = ACE_dynamic_cast (TAO_POA *,
                                   this->gateway_poa_);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableInterceptor::ObjectReferenceTemplate *ort_template =
    poa->get_adapter_template();

  TAO_ObjectReferenceTemplate *ort =
    ACE_dynamic_cast (TAO_ObjectReferenceTemplate *,
                      ort_template);

  CORBA::Object_ptr object_ptr =
    ort->make_object (interface_repository_id,
                      *obj_id
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return object_ptr;
}
