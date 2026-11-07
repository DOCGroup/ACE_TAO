#include "Service.h"

void
Service::dump_results ()
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
  std::size_t const pre_crash_exceptions =
    this->call_are_you_there (callback) + this->call_test_oneway (callback);

  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) - Service, calling crash_now_please\n"));
      callback->crash_now_please ();
    }
  catch (CORBA::Exception const&) {}

  std::size_t const pos_crash_exceptions =
    this->call_test_oneway (callback);

  if (pre_crash_exceptions != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: There were %B exceptions before crash\n",
                  pre_crash_exceptions));
    }

  if (pos_crash_exceptions == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "ERROR: There were no exceptions after crash\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Good!! %B exceptions raised after crash\n",
                  pos_crash_exceptions));
      orb_->shutdown (false);
    }

  /// Increment the number of tests completed
  this->test_count_++;
}

std::size_t
Service::call_are_you_there (Test::Crashed_Callback_ptr callback)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling are_you_there\n"));
  constexpr std::size_t iterations = 50;

  std::size_t exception_count {};
  for (std::size_t i = 0; i != iterations; ++i)
    {
      try
        {
          (void) callback->are_you_there ();
        }
      catch (CORBA::Exception const&)
        {
          exception_count++;
        }
    }
  return exception_count;
}

std::size_t
Service::call_test_oneway (Test::Crashed_Callback_ptr callback)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) - Service, calling test_oneway\n"));
  constexpr std::size_t iterations = 50;

  std::size_t exception_count {};
  for (std::size_t i = 0; i != iterations; ++i)
    {
      try
        {
          (void) callback->test_oneway ();
        }
      catch (CORBA::Exception const&)
        {
          exception_count++;
        }
    }
  return exception_count;
}
