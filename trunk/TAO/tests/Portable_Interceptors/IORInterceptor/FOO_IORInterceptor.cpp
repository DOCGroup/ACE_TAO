// -*- C++ -*-

#include "FOO_IORInterceptor.h"
#include "testC.h"

ACE_RCSID (IORInterceptor,
           FOO_IORInterceptor,
           "$Id$")

char *
FOO_IORInterceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("FOO_IORInterceptor");
}

void
FOO_IORInterceptor::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // According to the Portable Interceptors specification,
  // IORInterceptor::establish_components() must not throw an
  // exception.  We check for exceptions despite this fact.  The ORB
  // does the right thing, and ignores any IOR interceptors that throw
  // an exception.

  CORBA::String_var name = this->name (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Get the length of the string plus one for the NULL terminator.
  CORBA::ULong data_len = ACE_OS_String::strlen (name.in ()) + 1;

  // Construct a tagged component.
  IOP::TaggedComponent FOO_Component;

  FOO_Component.tag = FOO::COMPONENT_ID;

  FOO_Component.component_data.length (data_len);

  // Copy the name of this IOR interceptor into the component_data
  // buffer.  Later, it will be extracted on the client side.
  CORBA::Octet *buffer = FOO_Component.component_data.get_buffer ();

  ACE_OS_String::memcpy (buffer, name.in (), data_len);

  // Add the tagged component to all profiles.
  info->add_ior_component (FOO_Component
                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Added tagged component to the IOR\n"
              "(%P|%t) containing the string: %s\n",
              name.in ()));
}

void
FOO_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

void
FOO_IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId ,
    PortableInterceptor::AdapterState
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

void
FOO_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}
