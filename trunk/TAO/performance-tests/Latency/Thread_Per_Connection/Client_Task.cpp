//
// $Id$
//
#include "Client_Task.h"
#include "ace/OS_NS_time.h"

Client_Task::Client_Task (Test::Roundtrip_ptr roundtrip,
                          int niterations)
  : roundtrip_ (Test::Roundtrip::_duplicate (roundtrip))
  , niterations_ (niterations)
{
}

int
Client_Task::svc (void)
{
  try
    {
      this->validate_connection ();

      for (int i = 0; i != this->niterations_; ++i)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          (void) this->roundtrip_->test_method (start);

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          this->latency_.sample (now - start);
        }
    }
  catch (const CORBA::Exception&)
    {
      return 0;
    }
  return 0;
}

void
Client_Task::accumulate_and_dump (
  ACE_Basic_Stats &totals,
  const ACE_TCHAR *msg,
  ACE_High_Res_Timer::global_scale_factor_type gsf)
{
  totals.accumulate (this->latency_);
  this->latency_.dump_results (msg, gsf);
}

void
Client_Task::validate_connection (void)
{
  CORBA::ULongLong dummy = 0;
  for (int i = 0; i != 100; ++i)
    {
      try
        {
          (void) this->roundtrip_->test_method (dummy);
        }
      catch (const CORBA::Exception&){}
    }
}
