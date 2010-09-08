// $Id$

#include "FOO_IORInterceptor.h"
#include "testS.h"

#include "tao/ORB_Constants.h"

FOO_IORInterceptor::FOO_IORInterceptor (IOP::Codec_ptr codec)
  : codec_ (IOP::Codec::_duplicate (codec))
{
}

char *
FOO_IORInterceptor::name (void)
{
  return CORBA::string_dup ("FOO_IORInterceptor");
}

void
FOO_IORInterceptor::destroy (void)
{
}

void
FOO_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info)
{
  // According to the Portable Interceptors specification,
  // IORInterceptor::establish_components() must not throw an
  // exception.  We check for exceptions despite this fact.  The ORB
  // does the right thing, and ignores any IOR interceptors that throw
  // an exception.

  CORBA::String_var name = this->name ();

  CORBA::Any data;
  data <<= name.in ();

  CORBA::OctetSeq_var encoded_data =
    this->codec_->encode_value (data);

  // Construct a tagged component.
  IOP::TaggedComponent component;

  component.tag = FOO::COMPONENT_ID;

  const CORBA::ULong maximum   = encoded_data->maximum ();
  const CORBA::ULong length    = encoded_data->length ();
  CORBA::Octet * buffer  = encoded_data->get_buffer ();
  const CORBA::Boolean release = 0;  // OctetSeq retains ownership.

  ACE_ASSERT (maximum > 1);  // Sanity check.
  ACE_ASSERT (length > 1);   // Sanity check.

  component.component_data.replace (maximum, length, buffer, release);

  ACE_ASSERT (component.component_data.length () == length);

  // Add the tagged component to all profiles.
  info->add_ior_component (component);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Added tagged component containing the\n"
              "(%P|%t)   string \"%C\" to all profiles.\n",
              name.in ()));


  // Add the tagged component to all IIOP profiles.
  info->add_ior_component_to_profile (component,
                                      IOP::TAG_INTERNET_IOP);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Added tagged component containing the\n"
              "(%P|%t)   string \"%C\" to all IIOP profiles.\n",
              name.in ()));


  try
    {
      // Verify that policy retrieval internals work, and do not cause
      // memory access violations.
      CORBA::Policy_var policy =
        info->get_effective_policy (PortableServer::LIFESPAN_POLICY_ID);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Successfully retrieved effective policy.\n"));
    }
  catch (const CORBA::INV_POLICY& ex)
    {
      if (ex.minor () != (CORBA::OMGVMCID | 3))
        throw;
    }
}

void
FOO_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr info)
{
  // Construct a dummy tagged component.
  IOP::TaggedComponent component;

  component.tag = 0;
  component.component_data.length (1);


  try
    {
      // Add the tagged component to all profiles.
      //
      // This call should fail since it shouldn't be possible to add
      // an IOR component after establish_components() has been
      // called.
      info->add_ior_component (component);

      // The above call should NOT have succeeded.
      throw CORBA::INTERNAL ();
    }
  catch (const CORBA::BAD_INV_ORDER& ex)
    {
      if (ex.minor () != (CORBA::OMGVMCID | 14))
        {
          ex._tao_print_exception ("Unexpected exception");

          throw;
        }
    }

  try
    {
      // Add the tagged component to all profiles.
      //
      // This call should fail since it shouldn't be possible to add
      // an IOR component after establish_components() has been
      // called.
      info->add_ior_component_to_profile (component,
                                          IOP::TAG_INTERNET_IOP);

      // The above call should NOT have succeeded.
      throw CORBA::INTERNAL ();
    }
  catch (const CORBA::BAD_INV_ORDER& ex)
    {
      if (ex.minor () != (CORBA::OMGVMCID | 14))
        {
          ex._tao_print_exception ("Unexpected exception");

          throw;
        }
    }
}

void
FOO_IORInterceptor::adapter_manager_state_changed (
    const char *,
    PortableInterceptor::AdapterState)
{
}

void
FOO_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState)
{
}
