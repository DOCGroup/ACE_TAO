// -*- C++ -*-

#include "FOO_ClientRequestInterceptor.h"
#include "testC.h"

ACE_RCSID (IORInterceptor,
           FOO_ClientRequestInterceptor,
           "$Id$")

char *
FOO_ClientRequestInterceptor::name (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("FOO_ClientRequestInterceptor");
}

void
FOO_ClientRequestInterceptor::destroy (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_ClientRequestInterceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Verify that the tagged component that was supposed to be embedded
  // into the IOR by the FOO_IORInterceptor is actually in the IOR
  // profile.
  IOP::TaggedComponent_var component =
    ri->get_effective_component (FOO::COMPONENT_ID
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // The correct tagged component appears to exist.  Display the
  // contents of the component data, which should be a NULL terminated
  // string.  In the case of this test, that string should be the name
  // of the IORInterceptor that added the tagged component to the IOR.

  if (component->component_data.length () == 0)
    {
      // This should never happen because the IORInterceptor name is
      // not an empty string.
      ACE_THROW (CORBA::INTERNAL ());
    }

  char *name =
    ACE_reinterpret_cast (char *,
                          component->component_data.get_buffer ());

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) String extracted from tagged component "
              "data: %s\n",
              name));
}

void
FOO_ClientRequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_ClientRequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_ClientRequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
FOO_ClientRequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
