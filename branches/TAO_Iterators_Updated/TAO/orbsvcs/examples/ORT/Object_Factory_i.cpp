// $Id$

#include "Object_Factory_i.h"
#include "tao/PortableServer/Root_POA.h"

Object_Factory_i::Object_Factory_i (CORBA::ORB_ptr orb,
                                    PortableServer::POA_ptr gateway_poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    gateway_poa_ (gateway_poa)
{
  /// Constructor
}


CORBA::Object_ptr
Object_Factory_i::create_object (const char *interface_repository_id,
                                 CORBA::Object_ptr gatewayed_object)
{
  CORBA::String_var stringified_object =
    this->orb_->object_to_string (gatewayed_object);

  const PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (stringified_object.in ());

  const PortableInterceptor::ObjectId *obj_id =
    reinterpret_cast<const PortableInterceptor::ObjectId *> (&id.in ());
  ACE_UNUSED_ARG(obj_id);
  ACE_UNUSED_ARG(interface_repository_id);
/*
  TAO_POA *poa = dynamic_cast <TAO_POA *> (this->gateway_poa_);

  PortableInterceptor::ObjectReferenceTemplate *ort_template =
    poa->get_adapter_template();

  TAO_ObjectReferenceTemplate *ort =
    dynamic_cast <TAO_ObjectReferenceTemplate *> (ort_template);

  CORBA::Object_ptr object_ptr =
    ort->make_object (interface_repository_id,
                      *obj_id);
*/

  return CORBA::Object::_nil();
}
