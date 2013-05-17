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

::CORBA::ValueBase *
Gateway_ObjRef_Factory::_copy_value (void)
{
  Gateway_ObjRef_Factory *ret_val= 0;
  ACE_NEW_THROW_EX (
    ret_val,
    Gateway_ObjRef_Factory (
      gateway_object_factory_,
      old_factory_
    ),
    ::CORBA::NO_MEMORY ()
  );

  return ret_val;
}

CORBA::Object_ptr
Gateway_ObjRef_Factory::
make_object (const char *interface_repository_id,
             const PortableInterceptor::ObjectId & id)
{
  CORBA::Object_var object =
    this->old_factory_->make_object (interface_repository_id,
                                     id);

  CORBA::Object_ptr object_ptr =
    this->gateway_object_factory_->create_object (interface_repository_id,
                                                  object.in ());

  return object_ptr;
}
