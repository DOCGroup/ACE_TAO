// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Counter.h"

#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "tao/ORB_Core.h"

#include "Kokyu_EC.h"
#include "Dynamic_Consumer.h"
#include "Service_Handler.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#include <unistd.h>
#endif //ACE_HAS_DSUI

namespace
{
  int config_run = 0;
  ACE_CString sched_type ="rms";
  ACE_CString ior_output_filename;
  FILE * ior_output_file;
}

class Consumer_EC : public Kokyu_EC
{
public:
  Consumer_EC(void)
    : once_(0)
  {
  } //Consumer_EC()

  ~Consumer_EC(void)
  {
  } //~Consumer_EC()

  void set_up_supp_and_cons(CORBA::ORB_var orb
                            ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ))
  {
    ACE_Time_Value tv;

    //TODO: Create Dynamic_Consumer for merging fault-tolerant case!

    //// CONSUMER 3 ////
    Consumer::EventType cons_normal_type  = ACE_ES_EVENT_UNDEFINED+6;
    Consumer::EventType cons_normal_type2 = ACE_ES_EVENT_UNDEFINED+7;
    Consumer::EventType cons_ft_type  = ACE_ES_EVENT_UNDEFINED+8;
    Consumer::EventType cons_ft_type2 = ACE_ES_EVENT_UNDEFINED+9;
    Kokyu_EC::EventType_Vector cons1_3_types(2);
    cons1_3_types.push_back(cons_normal_type);
    cons1_3_types.push_back(cons_ft_type);
    cons1_3_types.push_back(cons_normal_type2);
    cons1_3_types.push_back(cons_ft_type2);

    Dynamic_Consumer * consumer_impl1_3;
    ACE_NEW(consumer_impl1_3,
            Dynamic_Consumer(cons_normal_type,cons_ft_type,
                             cons_normal_type2,cons_ft_type2));

    tv.set(0,50000);
    consumer_impl1_3->setWorkTime(tv);
    tv.set(0,200000); //period
    add_consumer(consumer_impl1_3,
                 "consumer1_3",
                 tv,
                 cons1_3_types,
                 RtecScheduler::VERY_HIGH_CRITICALITY,
                 RtecScheduler::VERY_HIGH_IMPORTANCE
                 ACE_ENV_ARG_PARAMETER
                 );
    ACE_CHECK;
    /*
    //DEBUG: print out schedule
    RtecScheduler::Scheduler_ptr scheduler = this->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
    //RtecEventChannelAdmin::EventChannel_ptr event_channel = this->event_channel(ACE_ENV_SINGLE_ARG_DECL);

    RtecScheduler::RT_Info_Set_var infos;
    RtecScheduler::Config_Info_Set_var configs;
    RtecScheduler::Dependency_Set_var dependencies;
    RtecScheduler::Scheduling_Anomaly_Set unsafe_anomalies;
    RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

    int min_os_priority =
      ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                      ACE_SCOPE_THREAD);
    int max_os_priority =
      ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                      ACE_SCOPE_THREAD);

    scheduler->compute_scheduling (min_os_priority,
                                   max_os_priority,
                                   infos.out (),
                                   dependencies.out (),
                                   configs.out (),
                                   anomalies.out ()
                                   ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    std::stringstream sched_out;
    ACE_CString ior_prefix(ior_output_filename.c_str(),ior_output_filename.length()-4); //cut off '.ior'
    sched_out << "schedule_" << ior_prefix.c_str() << ".out";

    ACE_DEBUG((LM_DEBUG,"Consumer_EC writing schedule to %s\n",sched_out.str().c_str()));

    ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                          dependencies.in (),
                                          configs.in (),
                                          anomalies.in (),
                                          sched_out.str().c_str());
    *///END DEBUG
    ACE_DEBUG((LM_DEBUG,"Consumer_EC set_up_supp_and_cons() DONE\n"));
  } //set_up_supp_and_cons()

private:
  Service_Handler * once_;
}; //class Consumer_EC

int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
  //TAO_EC_Default_Factory::init_svcs ();
  TAO_EC_Kokyu_Factory::init_svcs ();

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
#ifdef ACE_HAS_DSUI
      ds_control ds_cntl("Dynamic_Test_Consumer","consumer_enabled.dsui");
#endif // ACE_HAS_DSUI

      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        {
          return 1;
        }

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::POAManager_var poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //spawn thread to run the reactor event loop
      Reactor_Task rt;
      rt.initialize();

      // ****************************************************************

      Consumer_EC consumer_ec;
      if (consumer_ec.init(sched_type.c_str(), poa.in()) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Consumer_EC"), 1);
        }

      consumer_ec.set_up_supp_and_cons(orb ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************
      RtEventChannelAdmin::RtSchedEventChannel_var consumer_ec_ior =
        consumer_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(consumer_ec_ior.in()
                                                    ACE_ENV_ARG_PARAMETER);

      ACE_OS::fprintf(ior_output_file, ior.in());
      ACE_OS::fclose(ior_output_file);

      // ****************************************************************

      // At this point the consumer and supplier are connected to the
      // EC, they have provided their QoS info to the Scheduling
      // Service and the EC has informed the Scheduler about the
      // dependencies between them.
      // We can now compute the schedule for this configuration...

      // The schedule is returned in this variables....

      //consumer_ec.start(ACE_ENV_SINGLE_ARG_PARAMETER);
      //ACE_TRY_CHECK;

      // ****************************************************************

      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

#ifdef ACE_HAS_DSUI
      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      //DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Consumer_EC thread %t WORKER_ACTIVATED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 0, 0, NULL);

      //@BT
      //  ACE_Object_Counter::object_id oid = ACE_OBJECT_COUNTER->increment();
      //  DSTRM_EVENT(MAIN_GROUP_FAM, START, 1, sizeof(EC_Event_Counter::event_id), (char*)&eid);
      ACE_Time_Value now(ACE_OS::gettimeofday());
      ACE_OS::printf("Consumer_EC START at %isec %iusec\n",now.sec(),now.usec());
      DSTRM_EVENT(MAIN_GROUP_FAM, START, 0, 0, NULL);
#endif //ACE_HAS_DSUI

      rt.activate(); //need thread creation flags? or priority?
      ACE_Time_Value stop_time(310,0);
      orb->run (stop_time ACE_ENV_ARG_PARAMETER);
      //orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

#ifdef ACE_HAS_DSUI
      //@BT
      //DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Consumer_EC thread %t STOP at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
#endif //ACE_HAS_DSUI

      // ****************************************************************

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Consumer_EC");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

// ****************************************************************

int parse_args (int argc, char *argv[])
{
ACE_Get_Opt get_opts (argc, argv, "s:o:");
int c;

while ((c = get_opts ()) != -1)
  switch (c)
{
 case 'o':
   ior_output_filename = get_opts.opt_arg();
   ior_output_file = ACE_OS::fopen (ior_output_filename.c_str(), "w");
   if (ior_output_file == 0)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "Unable to open %s for writing: %p\n",
                        get_opts.opt_arg ()), -1);
   break;
 case 's':
   sched_type = ACE_TEXT_ALWAYS_CHAR(get_opts.opt_arg ());
   break;

 case '?':
 default:
   ACE_ERROR_RETURN ((LM_ERROR,
                      "usage:  %s -s <rms|muf|edf>"
                      "\n",
                      argv [0]),
                     -1);
}
// Indicates sucessful parsing of the command line
 if (ior_output_file == 0)
   {
     ior_output_filename = "consumer_ec.ior";
     ior_output_file = ACE_OS::fopen (ior_output_filename.c_str(), "w");
   }
 return 0;
}
