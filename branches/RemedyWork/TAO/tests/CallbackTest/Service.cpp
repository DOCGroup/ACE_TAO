//
// $Id$
//
#include "Service.h"

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
Service::run_test (Test::Callback_ptr callback)
{
  int exceptions =
    this->call_are_you_there (callback);

  exceptions +=
    this->call_test_oneway (callback);

  if (exceptions != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: There were %d exceptions\n"));
    }

  /// Increment the number of tests completed
  this->test_count_++;

  try
    {
      callback->shutdown ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_ERROR, "(%P|%t) - Service, client shutdown FAILED\n"));
    }

  // shutdown ourselves
  this->orb_->shutdown (0);
}


int
Service::call_are_you_there (Test::Callback_ptr callback)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling are_you_there\n"));
  const int iterations = 10;

  int exception_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      CORBA::String_var outstr;
      CORBA::String_out out_str (outstr.out ());
      try
        {
          (void) callback->are_you_there (out_str);
        }
      catch (const CORBA::Exception&)
        {
          exception_count++;
        }

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, answer = %C\n", outstr.in ()));
    }
  return exception_count;
}

int
Service::call_test_oneway (Test::Callback_ptr callback)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling test_oneway\n"));
  const int iterations = 10;

  int exception_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      try
        {
          (void) callback->test_oneway ();
        }
      catch (const CORBA::Exception&)
        {
          exception_count++;
        }
    }
  return exception_count;
}
