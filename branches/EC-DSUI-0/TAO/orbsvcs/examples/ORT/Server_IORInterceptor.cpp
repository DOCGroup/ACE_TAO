// $Id$

#include "Server_IORInterceptor.h"
#include "Gateway_ObjRef_Factory.h"

ACE_RCSID (ORT,
           Server_IORInterceptor,
           "$Id$")


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
Server_IORInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Server_IORInterceptor");
}

void
Server_IORInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::release (this->gateway_object_factory_);
  this->gateway_object_factory_ = Gateway::Object_Factory::_nil ();
}

void
Server_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr ior_info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Gateway_ObjRef_Factory *my_factory;

  PortableInterceptor::ObjectReferenceFactory_var current_factory =
    ior_info->current_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_NEW_THROW_EX (my_factory,
                    Gateway_ObjRef_Factory (this->gateway_object_factory_,
                                            current_factory.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ior_info->current_factory (my_factory
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Server_IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
