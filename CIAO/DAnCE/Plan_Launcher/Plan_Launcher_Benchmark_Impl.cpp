// $Id$
#include "Plan_Launcher_Benchmark_Impl.h"

#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "DAnCE/Utils/Plan_Handler.h"

namespace CIAO
{
  namespace Plan_Launcher
  {
    Plan_Launcher_Benchmark_i::~Plan_Launcher_Benchmark_i ()
    {
    }

    const char *
    Plan_Launcher_Benchmark_i::launch_plan (
        const ::Deployment::DeploymentPlan &plan)
    {
      try
        {
          ///// Start Test ////////////////////////////////////////////
          ACE_Sample_History history_prepare_plan (this->niterations_);
          ACE_Sample_History history_start_launch (this->niterations_);
          ACE_Sample_History history_finish_launch (this->niterations_);
          ACE_Sample_History history_start_system (this->niterations_);
          ACE_hrtime_t test_start = ACE_OS::gethrtime ();
          ACE_hrtime_t call_start, call_end;

          while (this->niterations_ > 0)
          {
            --this->niterations_;

            if (CORBA::is_nil (this->em_.in ()))
              {
                ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("CIAO::Plan_Launcher_Benchmark_i: ")
                  ACE_TEXT ("launch_plan called witn an uninitialized EM.\n")));
                return 0;
              }

            if (CIAO::debug_level () > 9)
              {
                ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CIAO::Plan_Launcher_Benchmark_i: " )
                  ACE_TEXT ("about to call this->em_->preparePlan\n")));
              }

            //////// Measure the latency for preparePlan()  ////////////////
            call_start = ACE_OS::gethrtime ();
            ::Deployment::DomainApplicationManager_var dam (this->em_->preparePlan (plan, 1));
            call_end = ACE_OS::gethrtime ();
            history_prepare_plan.sample (call_end - call_start);

            if (CIAO::debug_level () > 9)
              {
                ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("CIAO::Plan_Launcher_Benchmark_i: " )
                  ACE_TEXT ("after to call this->em_->preparePlan\n")));
              }

            if (CORBA::is_nil (dam.in ()))
              {
                ACE_ERROR ((LM_ERROR,
                  "(%P|%t) CIAO_PlanLauncher:preparePlan call failed: "
                  "nil DomainApplicationManager reference\n"));
                return 0;
              }

            if (CIAO::debug_level () > 9)
              {
                ACE_DEBUG ((LM_DEBUG,
                  "CIAO_PlanLauncher: Obtained DAM ref \n"));
              }

            ::Deployment::Properties_var properties;
            ACE_NEW_RETURN (properties,
                Deployment::Properties,
                0);

            if (CIAO::debug_level ())
            {
              ACE_DEBUG ((LM_DEBUG,
                "CIAO_PlanLauncher: start Launch application...\n"));
            }



            // Do not start the Application immediately since it violates
            // the semantics of component activation sequence
            int start = 0;

            //////// Measure the latency for startLaunch()  ////////////////
            call_start = ACE_OS::gethrtime ();
            dam->startLaunch (properties.in (), 0);
            call_end = ACE_OS::gethrtime ();
            history_start_launch.sample (call_end - call_start);


            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

            // Call finish Launch to complete the connections
            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG,
                          "CIAO_PlanLauncher: finish Launch application...\n"));


            //////// Measure the latency for finishLaunch()  ////////////////
            call_start = ACE_OS::gethrtime ();
            dam->finishLaunch (start, false); // is_ReDAC by default is <false>
            call_end = ACE_OS::gethrtime ();
            history_finish_launch.sample (call_end - call_start);

            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

            // Call start to activate components
            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG,
                          "CIAO_PlanLauncher: start activating components...\n"));

            //////// Measure the latency for start()  ////////////////
            call_start = ACE_OS::gethrtime ();
            dam->start ();
            call_end = ACE_OS::gethrtime ();
            history_start_system.sample (call_end - call_start);


            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("CIAO_PlanLauncher: ")
                          ACE_TEXT ("Application Deployed successfully\n")));

            map_.bind_dam_reference (plan.UUID.in (),
                                    Deployment::DomainApplicationManager::_duplicate (dam.in ()));

            // In order to run the launch_plan() multiple times to collect benchmarking
            // results, we have to tear down the plan and then redeploy.
            if (this->niterations_ > 0)
              {
                dam->destroyApplication ();
                this->destroy_dam_by_plan (plan.UUID.in ());
              }
          }


          ///// End Test ////////////////////////////////////////////
          ACE_hrtime_t test_end = ACE_OS::gethrtime ();

          ACE_DEBUG ((LM_DEBUG, "test finished\n"));

          ACE_DEBUG ((LM_DEBUG, "High resolution timer calibration...."));
          ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
          ACE_DEBUG ((LM_DEBUG, "done\n"));

          ACE_Basic_Stats stats_1;
          history_prepare_plan.dump_samples ("HISTORY -- pareparePlan", gsf);
          history_prepare_plan.collect_basic_stats (stats_1);
          stats_1.dump_results ("TOTAL -- pareparePlan", gsf);

          ACE_Basic_Stats stats_2;
          history_start_launch.dump_samples ("HISTORY -- startLaunch", gsf);
          history_start_launch.collect_basic_stats (stats_2);
          stats_2.dump_results ("TOTAL -- startLaunch", gsf);

          ACE_Basic_Stats stats_3;
          history_finish_launch.dump_samples ("HISTORY -- finishLaunch", gsf);
          history_finish_launch.collect_basic_stats (stats_3);
          stats_3.dump_results ("TOTAL -- finishLaunch", gsf);

          ACE_Basic_Stats stats_4;
          history_start_system.dump_samples ("HISTORY -- activateSystem", gsf);
          history_start_system.collect_basic_stats (stats_4);
          stats_4.dump_results ("TOTAL -- activateSystem", gsf);


          ACE_Throughput_Stats::dump_throughput ("Total", gsf,
                                                test_end - test_start,
                                                stats_1.samples_count ());

        }
      catch (Deployment::ResourceNotAvailable& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: ResourceNotAvaiable exception caught: %s,\n"
                      "Type: %s\n"
                      "Property: %s\n"
                      "Element: %s\n"
                      "Resource: %s\n",
                      ex.name.in (),
                      ex.resourceType.in (),
                      ex.propertyName.in (),
                      ex.elementName.in (),
                      ex.resourceName.in ()));
          throw Deployment_Failure ("");
        }
      catch (Deployment::StartError& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: StartError exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          throw Deployment_Failure ("");
        }
      catch (Deployment::InvalidProperty& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: InvalidProperty exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          throw Deployment_Failure ("");
        }
      catch (Deployment::InvalidConnection& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: InvalidConnection exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          throw Deployment_Failure  ("");
        }
      catch (const CORBA::Exception& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "CORBA EXCEPTION: %s\n",
                      ex._info().fast_rep()));
          throw Deployment_Failure  ("");
        }
      catch (...)
        {
          ACE_ERROR ((LM_ERROR,
                      "EXCEPTION: non-CORBA exception\n"));
          throw Deployment_Failure  ("");
        }

      return CORBA::string_dup (plan.UUID.in ());
    }
  }
}
