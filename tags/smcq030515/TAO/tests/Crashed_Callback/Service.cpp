//
// $Id$
//
#include "Service.h"

ACE_RCSID(Crashed_Callback, Service, "$Id$")

Service::Service (void)
  : test_count_ (0)
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
Service::run_test (Test::Crashed_Callback_ptr callback
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  int pre_crash_exceptions =
    this->call_are_you_there (callback ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  pre_crash_exceptions +=
    this->call_test_oneway (callback ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TRY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) - Service, calling crash_now_please\n"));
      callback->crash_now_please (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY {} ACE_ENDTRY;

  int pos_crash_exceptions =
    this->call_test_oneway (callback ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (pre_crash_exceptions != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: There were %d exceptions before crash\n"));
    }

  if (pos_crash_exceptions == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: There were no exceptions after crash\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Good!! %d exceptions raised after crash\n",
                  pos_crash_exceptions));
    }

  /// Increment the number of tests completed
  this->test_count_++;
}

int
Service::call_are_you_there (Test::Crashed_Callback_ptr callback
                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling are_you_there\n"));
  const int iterations = 50;

  int exception_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      ACE_TRY
        {
          (void) callback->are_you_there (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          exception_count++;
        }
      ACE_ENDTRY;
    }
  return exception_count;
}

int
Service::call_test_oneway (Test::Crashed_Callback_ptr callback
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (())
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling test_oneway\n"));
  const int iterations = 50;

  int exception_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      ACE_TRY
        {
          (void) callback->test_oneway (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          exception_count++;
        }
      ACE_ENDTRY;
    }
  return exception_count;
}
