#include "FOO_IORInterceptor.h"
#include "testS.h"


ACE_RCSID (IORInterceptor,
           FOO_IORInterceptor,
           "$Id$")


FOO_IORInterceptor::FOO_IORInterceptor (IOP::Codec_ptr codec)
  : codec_ (IOP::Codec::_duplicate (codec))
{
}

char *
FOO_IORInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("FOO_IORInterceptor");
}

void
FOO_IORInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // According to the Portable Interceptors specification,
  // IORInterceptor::establish_components() must not throw an
  // exception.  We check for exceptions despite this fact.  The ORB
  // does the right thing, and ignores any IOR interceptors that throw
  // an exception.

  CORBA::String_var name = this->name (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Any data;
  data <<= name.in ();

  CORBA::OctetSeq_var encoded_data =
    this->codec_->encode_value (data
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

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
  info->add_ior_component (component
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Added tagged component containing the\n"
              "(%P|%t)   string \"%s\" to all profiles.\n",
              name.in ()));


  // Add the tagged component to all IIOP profiles.
  info->add_ior_component_to_profile (component,
                                      IOP::TAG_INTERNET_IOP
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Added tagged component containing the\n"
              "(%P|%t)   string \"%s\" to all IIOP profiles.\n",
              name.in ()));


  ACE_TRY
    {
      // Verify that policy retrieval internals work, and do not cause
      // memory access violations.
      CORBA::Policy_var policy =
        info->get_effective_policy (PortableServer::THREAD_POLICY_ID
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Successfully retrieved effective policy.\n"));
    }
  ACE_CATCH (CORBA::INV_POLICY, ex)
    {
      if (ex.minor () != (CORBA::OMGVMCID | 3))
        ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
FOO_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Construct a dummy tagged component.
  IOP::TaggedComponent component;

  component.tag = 0;
  component.component_data.length (1);


  ACE_TRY_EX (ALL)
    {
      // Add the tagged component to all profiles.
      //
      // This call should fail since it shouldn't be possible to add
      // an IOR component after establish_components() has been
      // called.
      info->add_ior_component (component
                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (ALL);

      // The above call should NOT have succeeded.
      ACE_TRY_THROW_EX (CORBA::INTERNAL (), ALL);
    }
  ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
    {
      if (ex.minor () != (CORBA::OMGVMCID | 14))
        {
          ACE_PRINT_EXCEPTION (ex,
                               "Unexpected exception");

          ACE_RE_THROW_EX (ALL);
        }
    }
  ACE_ENDTRY;
  ACE_CHECK;

  ACE_TRY_EX (PROFILE)
    {
      // Add the tagged component to all profiles.
      //
      // This call should fail since it shouldn't be possible to add
      // an IOR component after establish_components() has been
      // called.
      info->add_ior_component_to_profile (component,
                                          IOP::TAG_INTERNET_IOP
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (PROFILE);

      // The above call should NOT have succeeded.
      ACE_TRY_THROW_EX (CORBA::INTERNAL (), PROFILE);
    }
  ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
    {
      if (ex.minor () != (CORBA::OMGVMCID | 14))
        {
          ACE_PRINT_EXCEPTION (ex,
                               "Unexpected exception");

          ACE_RE_THROW_EX (PROFILE);
        }
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
FOO_IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId ,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
