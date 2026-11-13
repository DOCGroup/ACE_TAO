#include "FOO_IORInterceptor.h"
#include "testS.h"

#include "tao/ORB_Constants.h"

#include <utility>

FOO_IORInterceptor::FOO_IORInterceptor (IOP::Codec_ptr codec)
  : codec_ (IOP::Codec::_duplicate (codec))
{
}

char *
FOO_IORInterceptor::name ()
{
  return CORBA::string_dup ("FOO_IORInterceptor");
}

void
FOO_IORInterceptor::destroy ()
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

  CORBA::ULong const length = encoded_data->length ();

  ACE_ASSERT (length > 1);   // Sanity check.

  component.component_data = std::move (*encoded_data.ptr ());

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
  catch (CORBA::INV_POLICY const &ex)
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

  component.tag = {};
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
  catch (CORBA::BAD_INV_ORDER const &ex)
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
  catch (CORBA::BAD_INV_ORDER const &ex)
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
    char const *,
    PortableInterceptor::AdapterState)
{
}

void
FOO_IORInterceptor::adapter_state_changed (
    PortableInterceptor::ObjectReferenceTemplateSeq const &,
    PortableInterceptor::AdapterState)
{
}
