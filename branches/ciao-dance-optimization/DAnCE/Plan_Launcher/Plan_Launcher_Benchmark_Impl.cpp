// $Id$
#include "Plan_Launcher_Benchmark_Impl.h"

#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"
#include "ace/Sample_History.h"
#include "ace/Task.h"
#include "DAnCE/Utils/Plan_Handler.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace CIAO
{
  namespace Plan_Launcher
  {
    class Task : public ACE_Task_Base
    {
    public:
      Task (Plan_Launcher_Benchmark_i &launcher,
            const ::Deployment::DeploymentPlan &plan);
      int svc (void);

    private:
      Plan_Launcher_Benchmark_i launcher_;
      const ::Deployment::DeploymentPlan plan_;

      static int plan_id_count_;
      ACE_Thread_Mutex mutex_;
    };

    int Task::plan_id_count_ = 0;

    Task::Task (Plan_Launcher_Benchmark_i &launcher,
                const ::Deployment::DeploymentPlan &plan)
      : launcher_ (launcher),
        plan_ (plan)
    {}

    int
    Task::svc (void)
    {
      Deployment::DeploymentPlan tmp_plan (plan_);
      ACE_CString uuid (tmp_plan.UUID.in ());

      char buf[256];

      mutex_.acquire ();
      uuid += ACE_OS::itoa (plan_id_count_++, buf, 10);
      mutex_.release ();

      tmp_plan.UUID = uuid.c_str ();

      launcher_.launch_plan_i (tmp_plan);
      return 0;
    }


    Plan_Launcher_Benchmark_i::~Plan_Launcher_Benchmark_i ()
    {
    }

    const char *
    Plan_Launcher_Benchmark_i::launch_plan (
        const ::Deployment::DeploymentPlan &plan)
    {
      // If it's a single threaded deployment
      if (this->nthreads_ == 1)
        return this->launch_plan_i (plan);

      // Create task.
      std::vector<Task*> task_vector;

      // Task activation flags.
      long flags = THR_NEW_LWP | THR_JOINABLE;

      for (size_t i = 0; i < nthreads_; ++i)
        {
          Deployment::DeploymentPlan tmp_plan (plan);

          DAnCE_Utils::remove_instance (tmp_plan, 
                                        "Hello-Sender-idd");       

          /* Modify the deployment plan. For each thread, we must
             give different component instance name to avoid
             confusing DAnCE to treat them as shared components */

          // Instance name starts with thread_id (i.e., task_id).
          char thread_id[256];
          ACE_OS::itoa (i, thread_id, 10);
          ACE_CString inst_name ("Task_");
          inst_name += thread_id;  
          inst_name += "_";

          // Now we add component instances into the plan 
          for (size_t j = 0; j < this->total_nodes_; ++j)
            {
              char node_id[256];
              ACE_OS::itoa (j, node_id, 10);
              ACE_CString node_name ("node");
              inst_name += node_id;  

              DAnCE_Utils::add_instances (this->total_components_,
                                          tmp_plan, 
                                          inst_name.c_str (),
                                          node_id,
                                          "Hello-Sender-mdd",
                                          0);
            }

          Task * task;

          ACE_NEW_RETURN (task, 
                          Task (*this, tmp_plan),
                          0);

          task_vector.push_back (task);

          int result = task->activate (flags);

          if (result == -1)
            {
              /*
              if (errno == EPERM)
                {
                  ACE_ERROR_RETURN ((LM_ERROR,
                                    "Cannot create thread with scheduling policy %s\n"
                                    "because the user does not have the appropriate privileges, terminating program....\n"
                                    "Check svc.conf options and/or run as root\n",
                                    sched_policy_name (orb_->orb_core ()->orb_params ()->ace_sched_policy ())),
                                    2);
                }
              else
                // Unexpected error. */
                ACE_ASSERT (0);
            }
        }

      for (size_t i = 0; i < nthreads_; ++i)
        {
          task_vector[i]->wait ();
        }

      return plan.UUID.in ();
    }

    const char *
    Plan_Launcher_Benchmark_i::launch_plan_i (
        const ::Deployment::DeploymentPlan &plan)
    {
      try
        {
          Deployment::DeploymentPlan tmp_plan (plan);
/*
          DAnCE_Utils::add_instances (100,
                                      tmp_plan, 
                                      "Sender",
                                      "SenderNode",
                                      "Hello-Sender-mdd",
                                      0);


          DAnCE_Utils::add_instances (100,
                                      tmp_plan, 
                                      "Receiver",
                                      "ReceiverNode",
                                      "Hello-Receiver-mdd",
                                      0);
*/
          // DAnCE_Utils::print_instances (tmp_plan);

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

ACE_DEBUG ((LM_DEBUG,
           "(%P|%t) **** step 10\n"));

            if (CORBA::is_nil (this->em_.in ()))
              {
                ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) CIAO::Plan_Launcher_Benchmark_i: ")
                  ACE_TEXT ("launch_plan called witn an uninitialized EM.\n")));
                return 0;
              }

            if (CIAO::debug_level () > 9)
              {
                ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) CIAO::Plan_Launcher_Benchmark_i: " )
                  ACE_TEXT ("about to call this->em_->preparePlan\n")));
              }

            //////// Measure the latency for preparePlan()  ////////////////
            call_start = ACE_OS::gethrtime ();
            ::Deployment::DomainApplicationManager_var dam (this->em_->preparePlan (tmp_plan, 1));
            call_end = ACE_OS::gethrtime ();
            history_prepare_plan.sample (call_end - call_start);

            if (CIAO::debug_level () > 9)
              {
                ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) CIAO::Plan_Launcher_Benchmark_i: " )
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
                  "(%P|%t) CIAO_PlanLauncher: Obtained DAM ref \n"));
              }

            ::Deployment::Properties_var properties;
            ACE_NEW_RETURN (properties,
                Deployment::Properties,
                0);

            if (CIAO::debug_level ())
            {
              ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) CIAO_PlanLauncher: start Launch application...\n"));
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
                          "(%P|%t) CIAO_PlanLauncher: finish Launch application...\n"));

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
                          "(%P|%t) CIAO_PlanLauncher: start activating components...\n"));

            //////// Measure the latency for start()  ////////////////
            call_start = ACE_OS::gethrtime ();
            dam->start ();
            call_end = ACE_OS::gethrtime ();
            history_start_system.sample (call_end - call_start);

            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG, "[success]\n"));

            if (CIAO::debug_level ())
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) CIAO_PlanLauncher: ")
                          ACE_TEXT ("Application Deployed successfully\n")));

            map_.bind_dam_reference (tmp_plan.UUID.in (),
                                     Deployment::DomainApplicationManager::_duplicate (dam.in ()));

            // In order to run the launch_plan() multiple times to collect benchmarking
            // results, we have to tear down the plan and then redeploy.
            if (this->niterations_ > 0)
              {
                dam->destroyApplication ();
                this->destroy_dam_by_plan (tmp_plan.UUID.in ());
              }
          }

          ///// End Test ////////////////////////////////////////////
          ACE_hrtime_t test_end = ACE_OS::gethrtime ();

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) Test finished\n"));

          ACE_DEBUG ((LM_DEBUG, "(%P|%t) High resolution timer calibration...."));
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

ACE_DEBUG ((LM_DEBUG,
           "(%P|%t) **** step 80\n"));

        }
      catch (Deployment::ResourceNotAvailable& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) EXCEPTION: ResourceNotAvaiable exception caught: %s,\n"
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
                      "(%P|%t) EXCEPTION: StartError exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          throw Deployment_Failure ("");
        }
      catch (Deployment::InvalidProperty& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) EXCEPTION: InvalidProperty exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          throw Deployment_Failure ("");
        }
      catch (Deployment::InvalidConnection& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) EXCEPTION: InvalidConnection exception caught: %s, %s\n",
                      ex.name.in (),
                      ex.reason.in ()));
          throw Deployment_Failure  ("");
        }
      catch (const CORBA::Exception& ex)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) CORBA EXCEPTION: %s\n",
                      ex._info().fast_rep()));
          throw Deployment_Failure  ("");
        }
      catch (...)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) EXCEPTION: non-CORBA exception\n"));
          throw Deployment_Failure  ("");
        }

ACE_DEBUG ((LM_DEBUG,
           "(%P|%t) **** step 90\n"));

      return CORBA::string_dup (plan.UUID.in ());
    }
  }
}
