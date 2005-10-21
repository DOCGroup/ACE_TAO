// $Id$

#include "RT_Class.h"
#include "ORB_Holder.h"
#include "Servant_var.h"
#include "RIR_Narrow.h"
#include "RTServer_Setup.h"
#include "Peer_Base.h"
#include "Auto_Disconnect.h"
#include "Send_Task.h"
#include "Task_Activator.h"
#include "Client_Pair.h"

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Default_Factory.h"

#include "tao/Messaging/Messaging.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Continuous_Priority_Mapping.h"
#include "tao/RTPortableServer/RTPortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/Auto_Ptr.h"
#include "ace/High_Res_Timer.h"
#include "ace/Sample_History.h"
#include "ace/Basic_Stats.h"
#include "ace/Stats.h"
#include "ace/Sched_Params.h"
#include "ace/Barrier.h"

ACE_RCSID(TAO_RTEC_PERF_Roundtrip, client, "$Id$")

const char *ior = "file://test.ior";
int nthreads   = 0;
int high_priority_period = 0;
int high_priority_workload = 0;
int low_priority_period  = 0;
int low_priority_workload = 0;
int disable_low_priority = 0;
int use_rt_corba = 0;

class Roundtrip_Peer : public Peer_Base
{
public:
  Roundtrip_Peer (CORBA::ORB_ptr orb,
                  RTServer_Setup &rtserver_setup,
                  RT_Class &rt_class
                  ACE_ENV_ARG_DECL)
    : Peer_Base (orb, rtserver_setup
                 ACE_ENV_ARG_PARAMETER)
    , rt_class_ (&rt_class)
  {
  }

  //@{
  /** @name The Federated_Test::Peer methods
   */
  virtual Federated_Test::Experiment_Results *
      run_experiment (CORBA::Long experiment_id,
                      CORBA::Long iterations
                      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

private:
  RT_Class *rt_class_;
};

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:n:l:h:w:v:zr");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'h':
        high_priority_period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'l':
        low_priority_period = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'w':
        high_priority_workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'v':
        low_priority_workload = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'z':
        disable_low_priority = 1;
        break;

      case 'r':
        use_rt_corba = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-k <IOR> "
                           "-i iterations (iterations) "
                           "-h high_priority_period (usecs) "
                           "-l low_priority_period (usecs) "
                           "-w high_priority_workload (usecs) "
                           "-v low_priority_workload (usecs) "
                           "-r (enable RT-CORBA) "
                           "-n nthreads (low priority thread) "
                           "-z (disable low priority) "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int main (int argc, char *argv[])
{
  TAO_EC_Default_Factory::init_svcs();
  RT_Class rt_class;

  ACE_TRY_NEW_ENV
    {
      ORB_Holder orb (argc, argv, ""
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      RTServer_Setup rtserver_setup (use_rt_corba,
                                     orb,
                                     rt_class,
                                     nthreads
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        RIR_Narrow<PortableServer::POA>::resolve (orb,
                                                  "RootPOA"
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished ORB and POA configuration\n"));

      Servant_var<Roundtrip_Peer> peer_impl (
          new Roundtrip_Peer (orb,
                              rtserver_setup,
                              rt_class
                              ACE_ENV_ARG_PARAMETER)
          );
      ACE_TRY_CHECK;

      Federated_Test::Peer_var peer =
        peer_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Finished peer configuration and activation\n"));

      CORBA::Object_var object =
        orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Federated_Test::Control_var control =
        Federated_Test::Control::_narrow (object.in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      control->join (peer.in ()
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
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

Federated_Test::Experiment_Results *
Roundtrip_Peer::run_experiment (CORBA::Long experiment_id,
                                CORBA::Long iterations
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int thread_count = 1;
#if 0
  if (disable_low_priority == 0)
    thread_count += nthreads;
#endif

  ACE_Barrier the_barrier (thread_count);

  ACE_DEBUG ((LM_DEBUG, "Calibrating high res timer ...."));
  ACE_High_Res_Timer::calibrate ();

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_DEBUG ((LM_DEBUG, "Done (%d)\n", gsf));

#if 0
  Low_Priority_Setup<Client_Pair> low_priority_setup (
          nthreads,
          0, // no limit on the number of iterations
          1, // each client gets its own type
          experiment_id,
          ACE_ES_EVENT_UNDEFINED + 2,
          low_priority_workload,
          gsf,
          disable_low_priority ? 0 : 1,
          this->rt_class_.priority_low (),
          this->rt_class_.thr_sched_class (),
          low_priority_period,
          this->poa_.in (),
          this->poa_.in (),
          this->event_channel_.in (),
          &the_barrier
          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
#endif

  Client_Pair high_priority_group;
  high_priority_group.init (experiment_id,
                            ACE_ES_EVENT_UNDEFINED,
                            iterations,
                            high_priority_workload,
                            gsf,
                            this->poa_.in (),
                            this->poa_.in ());
  high_priority_group.connect (this->event_channel_.in ()
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  Auto_Disconnect<Client_Pair> high_priority_disconnect (&high_priority_group);

  Send_Task high_priority_task;
  high_priority_task.init (iterations,
                           high_priority_period,
                           0,
                           ACE_ES_EVENT_UNDEFINED,
                           experiment_id,
                           high_priority_group.supplier (),
                           &the_barrier);
  {
    // Artificial scope to wait for the high priority task...
    Task_Activator<Send_Task> high_priority_act (this->rt_class_->priority_high (),
                                                 this->rt_class_->thr_sched_class (),
                                                 1,
                                                 &high_priority_task);
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - high priority task completed\n"));

#if 0
  if (disable_low_priority == 0)
    {
      low_priority_setup.stop_all_threads ();
    }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - low priority task(s) stopped\n"));
#endif

  ACE_Sample_History &history =
    high_priority_group.consumer ()->sample_history ();

  Federated_Test::Experiment_Results_var results (
     new Federated_Test::Experiment_Results (iterations)
     );
  results->length (iterations);
  for (CORBA::ULong i = 0; i != results->length (); ++i)
    {
      results[i] = history.get_sample (i) / gsf;
    }

  return results._retn ();
}
