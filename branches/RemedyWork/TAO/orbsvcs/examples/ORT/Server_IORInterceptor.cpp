// $Id$

#include "Server_IORInterceptor.h"
#include "Gateway_ObjRef_Factory.h"

Server_IORInterceptor::
Server_IORInterceptor (Gateway::Object_Factory_ptr factory)
  : gateway_object_factory_ (Gateway::Object_Factory::_duplicate (factory))
{
}

Server_IORInterceptor::~Server_IORInterceptor (void)
{
  CORBA::release (this->gateway_object_factory_);
}


char *
Server_IORInterceptor::name (void)
{
  return CORBA::string_dup ("Server_IORInterceptor");
}

void
Server_IORInterceptor::destroy (void)
{
  CORBA::release (this->gateway_object_factory_);
  this->gateway_object_factory_ = Gateway::Object_Factory::_nil ();
}

void
Server_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr)
{
}

void
Server_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr ior_info)
{
  Gateway_ObjRef_Factory *my_factory = 0;

  PortableInterceptor::ObjectReferenceFactory_var current_factory =
    ior_info->current_factory ();

  ACE_NEW_THROW_EX (my_factory,
                    Gateway_ObjRef_Factory (this->gateway_object_factory_,
                                            current_factory.in ()),
                    CORBA::NO_MEMORY ());

  ior_info->current_factory (my_factory);
}

void
Server_IORInterceptor::adapter_manager_state_changed (
    const char *,
    PortableInterceptor::AdapterState)
{
}

void
Server_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState)
{
}
