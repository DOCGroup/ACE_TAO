#include "FOO_ClientRequestInterceptor.h"
#include "testC.h"


ACE_RCSID (IORInterceptor,
           FOO_ClientRequestInterceptor,
           "$Id$")


FOO_ClientRequestInterceptor::FOO_ClientRequestInterceptor (
  IOP::Codec_ptr codec)
  : codec_ (IOP::Codec::_duplicate (codec))
{
}

char *
FOO_ClientRequestInterceptor::name (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("FOO_ClientRequestInterceptor");
}

void
FOO_ClientRequestInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_ClientRequestInterceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Verify that the tagged component that was supposed to be embedded
  // into the IOR by the FOO_IORInterceptor is actually in the IOR
  // profile.
  IOP::TaggedComponent_var component =
    ri->get_effective_component (FOO::COMPONENT_ID
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // The correct tagged component appears to exist.  Display the
  // contents of the component data, which should be a NULL terminated
  // string.  In the case of this test, that string should be the name
  // of the IORInterceptor that added the tagged component to the IOR.

  if (component->component_data.length () == 0)
    {
      // This should never happen because the IORInterceptor name is
      // not an empty string.
      ACE_ERROR ((LM_ERROR,
                  "Internal test error.  Empty tagged component.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  CORBA::ULong maximum   = component->component_data.maximum ();
  CORBA::ULong length    = component->component_data.length ();
  CORBA::Octet * buffer  = component->component_data.get_buffer ();
  CORBA::Boolean release = 0;  // TaggedComponent retains ownership.

  CORBA::OctetSeq data (maximum, length, buffer, release);

  // Extract the data from the octet sequence.
  CORBA::Any_var decoded_data =
    this->codec_->decode_value (data,
                                CORBA::_tc_string
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const char * name = 0;

  if (!(decoded_data.in () >>= name))
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Unable to extract tagged component "
                  "data from Any.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) String extracted from tagged component "
              "data: %s\n",
              name));
}

void
FOO_ClientRequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_ClientRequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_ClientRequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
FOO_ClientRequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}
