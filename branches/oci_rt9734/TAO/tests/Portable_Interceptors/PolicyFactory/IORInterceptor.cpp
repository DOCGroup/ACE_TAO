#include "IORInterceptor.h"
#include "testC.h"


ACE_RCSID (PolicyFactory,
           IORInterceptor,
           "$Id$")


IORInterceptor::IORInterceptor (void)
  : success_ (false)
{
}

IORInterceptor::~IORInterceptor (void)
{
  ACE_ASSERT (this->success_);
}

char *
IORInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("");
}

void
IORInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ASSERT (this->success_);
}

void
IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      PortableInterceptor::ObjectReferenceTemplate_var t =
        info->adapter_template (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableInterceptor::AdapterName_var a =
        t->adapter_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Only execute if POA is not RootPOA.  The RootPOA will not
      // have our custom policy, but the child POA we created will.
      if (a->length () > 1)
        {
          CORBA::Policy_var policy (
            info->get_effective_policy (Test::POLICY_TYPE
                                        ACE_ENV_ARG_PARAMETER));
          ACE_TRY_CHECK;

          Test::Policy_var test_policy (Test::Policy::_narrow (
            policy.in ()
            ACE_ENV_ARG_PARAMETER));
          ACE_TRY_CHECK;

          this->success_ = true;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "EXCEPTION: "
                           "IORInterceptor::establish_components:");

      ACE_ASSERT (false);
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

