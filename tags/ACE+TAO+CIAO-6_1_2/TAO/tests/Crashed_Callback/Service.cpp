//
// $Id$
//
#include "Service.h"

Service::Service (void)
  : orb_ (CORBA::ORB::_nil ()),
    test_count_ (0)
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
Service::run_test (Test::Crashed_Callback_ptr callback)
{
  int pre_crash_exceptions =
    this->call_are_you_there (callback);

  pre_crash_exceptions +=
    this->call_test_oneway (callback);

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) - Service, calling crash_now_please\n"));
      callback->crash_now_please ();
    }
  catch (const CORBA::Exception&){}

  int pos_crash_exceptions =
    this->call_test_oneway (callback);

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
      orb_->shutdown (false);
    }

  /// Increment the number of tests completed
  this->test_count_++;
}

int
Service::call_are_you_there (Test::Crashed_Callback_ptr callback)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling are_you_there\n"));
  const int iterations = 50;

  int exception_count = 0;
  for (int i = 0; i != iterations; ++i)
    {
      try
        {
          (void) callback->are_you_there ();
        }
      catch (const CORBA::Exception&)
        {
          exception_count++;
        }
    }
  return exception_count;
}

int
Service::call_test_oneway (Test::Crashed_Callback_ptr callback)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling test_oneway\n"));
  const int iterations = 50;

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
