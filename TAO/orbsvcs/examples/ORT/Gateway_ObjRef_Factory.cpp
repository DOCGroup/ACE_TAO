// $Id$

#include "Gateway_ObjRef_Factory.h"

Gateway_ObjRef_Factory::
Gateway_ObjRef_Factory (
   Gateway::Object_Factory_ptr gateway_object_factory,
   PortableInterceptor::ObjectReferenceFactory *old_factory)
  : gateway_object_factory_ (gateway_object_factory),
    old_factory_ (old_factory)
{
  CORBA::add_ref (old_factory);
}

CORBA::Object_ptr
Gateway_ObjRef_Factory::
make_object (const char *interface_repository_id,
             const PortableInterceptor::ObjectId & id
             TAO_ENV_ARG_DECL)
{
  CORBA::Object_var object =
    this->old_factory_->make_object (interface_repository_id,
                                     id
                                     TAO_ENV_ARG_DECL);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::Object_ptr object_ptr =
    this->gateway_object_factory_->create_object (interface_repository_id,
                                                  object.in ()
                                                  TAO_ENV_ARG_DECL);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return object_ptr;
}
