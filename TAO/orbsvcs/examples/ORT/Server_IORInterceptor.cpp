// $Id$

#include "Server_IORInterceptor.h"
#include "Gateway_ObjRef_Factory.h"

ACE_RCSID (ORT,
           Server_IORInterceptor,
           "$Id$")


Server_IORInterceptor::
Server_IORInterceptor (Gateway::Object_Factory_ptr
                        gateway_object_factory)
  : gateway_object_factory_ (Gateway::Object_Factory::_duplicate (gateway_object_factory))
{
  /// Constructor
  cout << __FILE__ << __LINE__ << endl;
}

char *
Server_IORInterceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("Server_IORInterceptor");
}

void
Server_IORInterceptor::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->gateway_object_factory_ = Gateway::Object_Factory::_nil ();
}

void
Server_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var name = this->name (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Get the length of the string plus one for the NULL terminator.
  CORBA::ULong data_len = ACE_OS_String::strlen (name.in ()) + 1;

  // Construct a tagged component.
  IOP::TaggedComponent ORT_Component;

  const IOP::ComponentId COMPONENT_ID = 12345;

  ORT_Component.tag = COMPONENT_ID;

  ORT_Component.component_data.length (data_len);

  // Copy the name of this IOR interceptor into the component_data
  // buffer.  Later, it will be extracted on the client side.
  CORBA::Octet *buffer = ORT_Component.component_data.get_buffer ();

  ACE_OS_String::memcpy (buffer, name.in (), data_len);

  // Add the tagged component to all profiles.
  info->add_ior_component (ORT_Component
                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Added tagged component to the IOR\n"
              "(%P|%t) containing the string: %s\n",
              name.in ()));
}

void
Server_IORInterceptor::
components_established (PortableInterceptor::IORInfo_ptr ior_info)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DECLARE_NEW_CORBA_ENV;

  Gateway_ObjRef_Factory *my_factory;

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
    PortableInterceptor::AdapterManagerId ,
    PortableInterceptor::AdapterState)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

void
Server_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}
