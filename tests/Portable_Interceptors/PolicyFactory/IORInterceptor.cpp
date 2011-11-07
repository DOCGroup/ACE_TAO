// $Id$

#include "IORInterceptor.h"
#include "testC.h"

IORInterceptor::IORInterceptor (void)
  : success_ (false)
{
}

IORInterceptor::~IORInterceptor (void)
{
  ACE_ASSERT (this->success_);
}

char *
IORInterceptor::name (void)
{
  return CORBA::string_dup ("");
}

void
IORInterceptor::destroy (void)
{
  ACE_ASSERT (this->success_);
}

void
IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr info)
{
  try
    {
      PortableInterceptor::ObjectReferenceTemplate_var t =
        info->adapter_template ();

      PortableInterceptor::AdapterName_var a =
        t->adapter_name ();

      // Only execute if POA is not RootPOA.  The RootPOA will not
      // have our custom policy, but the child POA we created will.
      if (a->length () > 1)
        {
          CORBA::Policy_var policy (
            info->get_effective_policy (Test::POLICY_TYPE));

          Test::Policy_var test_policy (Test::Policy::_narrow (
            policy.in ()));

          this->success_ = true;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "EXCEPTION: ""IORInterceptor::establish_components:");

      ACE_ASSERT (false);
    }
}

