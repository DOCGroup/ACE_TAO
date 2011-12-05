//
// $Id$
//

#include "Client_Task.h"
#include "tao/Strategies/advanced_resource.h"

#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"

Client_Task::Client_Task (int &argc, ACE_TCHAR **argv)
  : argc_ (argc)
  , argv_ (argv)
  , ior_ (ACE_TEXT("file://test.ior"))
  , iterations_ (1000)
{
}

int
Client_Task::parse_args (void)
{
    ACE_Get_Opt get_opts (this->argc_, this->argv_, "k:n:O:");
    int c;

    while ((c = get_opts ()) != -1)
      {
        switch (c)
          {
          case 'k':
            {
              this->ior_ = get_opts.opt_arg ();
              break;
            }
          case 'n':
            {
              int iterations = ACE_OS::atoi (get_opts.opt_arg ());
              if (iterations >= 0 )
                this->iterations_ = iterations;
              break;
            }
          case '?':
            ACE_ERROR_RETURN ((LM_ERROR,
                               "usage:  %s "
                               "-k <file that contains IOR> "
                               "-n <iterations> "
                               "\n",
                               this->argv_ [0]),
                              -1);
          default:
            break;
          }
      }
    // Indicates successful parsing of the command line
    return 1;
}

void
Client_Task::try_RT_scheduling (void)
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                              priority);

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (errno == EPERM)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed\n"));
    }
}

int
Client_Task::narrow_servant (CORBA::ORB_ptr orb)
{
  try
    {
      CORBA::Object_var object =
        orb->string_to_object (this->ior_);

      this->roundtrip_ =
        Test::Roundtrip::_narrow (object.in ());

      if (CORBA::is_nil (this->roundtrip_.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Test::Roundtrip reference <%s>\n",
                             this->ior_),
                            0);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught trying to narrow servant\n");
      return 0;
    }
  return 1;
}

int
Client_Task::run_test (void)
{
  ACE_hrtime_t test_start = 0;
  ACE_hrtime_t test_end = 0;

  try
    {
      test_start = ACE_OS::gethrtime ();

      this->roundtrip_->start_test ();

      this->svc ();

      this->roundtrip_->end_test ();

      test_end = ACE_OS::gethrtime ();
    }
  catch (const CORBA::Exception&)
    {
      return 0;
    }


  // High resolution timer calibration
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_Basic_Stats totals;

  this->accumulate_and_dump (totals, ACE_TEXT("Task"), gsf);

  totals.dump_results (ACE_TEXT("Total"), gsf);

  ACE_Throughput_Stats::dump_throughput (ACE_TEXT("Total"), gsf,
                                         test_end - test_start,
                                         totals.samples_count ());

  return 1;
}

int
Client_Task::svc (void)
{
  try
    {
      for (int i = 0; i != this->iterations_; ++i)
        {
          CORBA::ULongLong start = ACE_OS::gethrtime ();

          (void) this->roundtrip_->test_method (start);

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          this->latency_.sample (now - start);
        }
    }
  catch (const CORBA::Exception&)
    {
      return 0;
    }
  return 1;
}

void
Client_Task::accumulate_and_dump (ACE_Basic_Stats &totals,
                                  const ACE_TCHAR *msg,
                                  ACE_UINT32 gsf)
{
  totals.accumulate (this->latency_);
  this->latency_.dump_results (msg, gsf);
}

