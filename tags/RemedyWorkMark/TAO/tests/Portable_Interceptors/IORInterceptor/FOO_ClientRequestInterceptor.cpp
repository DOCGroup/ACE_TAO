// $Id$

#include "FOO_ClientRequestInterceptor.h"
#include "testC.h"

#include "ace/Log_Msg.h"

#include "tao/AnyTypeCode/TypeCode_Constants.h"

FOO_ClientRequestInterceptor::FOO_ClientRequestInterceptor (
  IOP::Codec_ptr codec)
  : codec_ (IOP::Codec::_duplicate (codec))
{
}

char *
FOO_ClientRequestInterceptor::name ()
{
  return CORBA::string_dup ("FOO_ClientRequestInterceptor");
}

void
FOO_ClientRequestInterceptor::destroy (void)
{
}

void
FOO_ClientRequestInterceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri)
{
  // Verify that the tagged component that was supposed to be embedded
  // into the IOR by the FOO_IORInterceptor is actually in the IOR
  // profile.
  IOP::TaggedComponent_var component =
    ri->get_effective_component (FOO::COMPONENT_ID);

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

      throw CORBA::INTERNAL ();
    }

  CORBA::ULong maximum   = component->component_data.maximum ();
  CORBA::ULong length    = component->component_data.length ();
  CORBA::Octet * buffer  = component->component_data.get_buffer ();
  CORBA::Boolean release = 0;  // TaggedComponent retains ownership.

  CORBA::OctetSeq data (maximum, length, buffer, release);

  // Extract the data from the octet sequence.
  CORBA::Any_var decoded_data =
    this->codec_->decode_value (data,
                                CORBA::_tc_string);

  const char * name = 0;

  if (!(decoded_data.in () >>= name))
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: Unable to extract tagged component "
                  "data from Any.\n"));

      throw CORBA::INTERNAL ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) String extracted from tagged component "
              "data: %C\n",
              name));
}

void
FOO_ClientRequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
FOO_ClientRequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
FOO_ClientRequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
FOO_ClientRequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}
