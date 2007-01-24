//
// $Id$
//
#include "Service.h"

ACE_RCSID(Callback, Service, "$Id$")

Service::Service (CORBA::ORB_ptr orb)
  : test_count_ (0),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Service::dump_results (void)
{
  if (this->test_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ERROR: no tests were completed!\n"));
    }
}

void
Service::run_test (Test::Callback_ptr callback
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  int exceptions =
    this->call_are_you_there (callback ACE_ENV_ARG_PARAMETER);

  exceptions +=
    this->call_test_oneway (callback ACE_ENV_ARG_PARAMETER);

  if (exceptions != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: There were %d exceptions\n"));
    }

  /// Increment the number of tests completed
  this->test_count_++;

  ACE_TRY
    {
      callback->shutdown ();
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_ERROR, "(%P|%t) - Service, client shutdown FAILED\n"));
    }
  ACE_ENDTRY;

  // shutdown ourselves
  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}


int
Service::call_are_you_there (Test::Callback_ptr callback
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling are_you_there\n"));
  const int iterations = 10;

  int exception_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      char* outstr;
      CORBA::String_out out_str (outstr);
      ACE_TRY
        {
          (void) callback->are_you_there (out_str ACE_ENV_ARG_PARAMETER);
        }
      ACE_CATCHANY
        {
          exception_count++;
        }
      ACE_ENDTRY;

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, answer = %s\n", outstr));
    }
  return exception_count;
}

int
Service::call_test_oneway (Test::Callback_ptr callback
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling test_oneway\n"));
  const int iterations = 10;

  int exception_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      ACE_TRY
        {
          (void) callback->test_oneway ();
        }
      ACE_CATCHANY
        {
          exception_count++;
        }
      ACE_ENDTRY;
    }
  return exception_count;
}
