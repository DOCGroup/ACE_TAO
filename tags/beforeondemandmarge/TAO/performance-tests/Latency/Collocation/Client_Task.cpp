//
// $Id$
//

#include "Client_Task.h"
#include "ace/Stats.h"
#include "ace/Sample_History.h"
#include "ace/High_Res_Timer.h"
#include "ace/SString.h"

Client_Task::Client_Task (Test::Roundtrip_ptr reference,
                          int iterations,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr),
    niterations (iterations),
    remote_ref_ (Test::Roundtrip::_duplicate (reference))

{
}

int
Client_Task::svc (void)
{
  ACE_TRY_NEW_ENV
    {
      if (CORBA::is_nil (this->remote_ref_.in ()))
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Nil Test::Roundtrip reference\n"),
                          1);
      }

      //// Start Test ///////////////////////////////
      CORBA::Long test_time = 1234L;

      // Warm up the system
      for (int i=0; i < 1000; i++)
        this->remote_ref_->test_method (test_time);

      // Start for actual Measurements
      ACE_Sample_History history (niterations);

      ACE_hrtime_t test_start = ACE_OS::gethrtime ();
      for (int itercounter = 0; itercounter < niterations; ++itercounter)
        {
          ACE_hrtime_t start = ACE_OS::gethrtime ();

          (void) this->remote_ref_->test_method (start ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_hrtime_t now = ACE_OS::gethrtime ();
          history.sample (now - start);
        }

      ACE_hrtime_t test_end = ACE_OS::gethrtime ();

      ACE_DEBUG ((LM_DEBUG, "test finished\n"));

      ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
      ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
      ACE_DEBUG ((LM_DEBUG, "done\n"));

      ACE_Basic_Stats stats;
      history.collect_basic_stats (stats);
      stats.dump_results ("Total", gsf);

      ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                             test_end - test_start,
                                             stats.samples_count ());

      //shutdown the server ORB
      this->remote_ref_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;

}
