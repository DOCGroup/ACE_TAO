// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Thread.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Select_Reactor_Base.h" //for ACE_Select_Reactor_Impl::DEFAULT_SIZE

#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Limit.h"

#include "Kokyu_EC.h"
#include "Consumer.h"
#include "Supplier.h"

#ifdef ACE_HAS_DSUI
#include <dsui.h>
#include "federated_config.h"
#include "federated_dsui_families.h"
#endif //ACE_HAS_DSUI

namespace
{
  int config_run = 0;
  ACE_CString sched_type ="rms";
  FILE * ior_output_file;
}

class Supplier_EC : public Kokyu_EC
{
  TAO_EC_Gateway_Sched gateway;
public:
  Supplier_EC()
  {
  } //Supplier_EC()

  ~Supplier_EC(void)
  {
  } //~Supplier_EC()

  void init_gateway(CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    const char* consumer_ec_ior)
  {
      CORBA::Object_var obj;
      RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec, consumer_ec;

      ACE_CHECK;
      obj = orb->string_to_object(consumer_ec_ior ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      consumer_ec = RtEventChannelAdmin::RtSchedEventChannel::_narrow(obj.in()
                                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::EventChannel_var supplier_event_channel =
        this->event_channel(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::EventChannel_var consumer_event_channel =
        consumer_ec->event_channel(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::Scheduler_var supplier_scheduler =
        this->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      RtecScheduler::Scheduler_var consumer_scheduler =
        consumer_ec->scheduler(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      gateway.init(supplier_event_channel.in(),
                   consumer_event_channel.in(),
                   supplier_scheduler.in(),
                   consumer_scheduler.in(),
                   "gateway1", "gateway2a"
                   ACE_ENV_ARG_PARAMETER);

      ACE_CHECK;
            PortableServer::ObjectId_var gateway_oid =
         poa->activate_object(&gateway ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Object_var gateway_obj =
         poa->id_to_reference(gateway_oid.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

       RtecEventChannelAdmin::Observer_var obs =
         RtecEventChannelAdmin::Observer::_narrow(gateway_obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      RtecEventChannelAdmin::Observer_Handle local_ec_obs_handle =
         consumer_event_channel->append_observer (obs.in () ACE_ENV_ARG_PARAMETER);
      ACE_UNUSED_ARG(local_ec_obs_handle);
      ACE_CHECK;

      consumer_ec->start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
  }

  virtual void start (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecScheduler::UNKNOWN_TASK
        , RtecScheduler::INTERNAL
        , RtecScheduler::SYNCHRONIZATION_FAILURE
      ))
  {
    Supplier *supplier_impl1_1;
    Timeout_Consumer *timeout_consumer_impl1_1;
    ACE_NEW(supplier_impl1_1,
            Supplier(1));
    ACE_NEW(timeout_consumer_impl1_1,
            Timeout_Consumer(supplier_impl1_1));
    ACE_Time_Value tv(0,200000); //period
    add_supplier_with_timeout(supplier_impl1_1,
                              "supplier1_1",
                              ACE_ES_EVENT_UNDEFINED,
                              timeout_consumer_impl1_1,
                              "supplier1_1_timeout_consumer",
                              tv,
                              RtecScheduler::VERY_LOW_CRITICALITY,
                              RtecScheduler::VERY_LOW_IMPORTANCE
                              ACE_ENV_ARG_PARAMETER
                              );
    ACE_CHECK;

    Supplier *supplier_impl1_2;
    ACE_NEW(supplier_impl1_2,
            Supplier(2));
    Consumer * consumer_impl1_1;
    ACE_NEW(consumer_impl1_1,
            Consumer(supplier_impl1_2));

    tv.set(0,50000);
    consumer_impl1_1->setWorkTime(tv);
    //consumer's rate will get propagated from the supplier.
    //so no need to specify a period here.
    tv.set(0,200000); //Period
    add_consumer_with_supplier(consumer_impl1_1, //deleted in consumer
                               "consumer1_1",
                               tv,
                               ACE_ES_EVENT_UNDEFINED,
                               RtecScheduler::VERY_LOW_CRITICALITY,
                               RtecScheduler::VERY_LOW_IMPORTANCE,
                               supplier_impl1_2,
                               "supplier1_2",
                               ACE_ES_EVENT_UNDEFINED+1
                               ACE_ENV_ARG_PARAMETER
                               );
    ACE_CHECK;

    Kokyu_EC::start(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
};

int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
  //TAO_EC_Default_Factory::init_svcs ();
#ifdef ACE_HAS_DSUI
  ds_control* ds_cntl = new ds_control ("Chain_Test_Supplier","supplier_enabled.dsui");
#endif //ACE_HAS_DSUI

  TAO_EC_Kokyu_Factory::init_svcs ();
  TAO_EC_Gateway_IIOP_Factory::init_svcs ();

  //@BT
  //DSTRM_EVENT(MAIN_GROUP_FAM, START,1,0,NULL);
  ACE_Time_Value now(ACE_OS::gettimeofday());
  ACE_OS::printf("Supplier_EC START at %isec %iusec\n",now.sec(),now.usec());
  DSTRM_EVENT(MAIN_GROUP_FAM, START,0,0,NULL);

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // ORB initialization boiler plate...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Usage:  %s -s <rms|muf|edf>"
                      "\n",
                      argv [0]));
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

      Supplier_EC supplier_ec;
      if (supplier_ec.init(sched_type.c_str(), poa.in()) == -1)
        {
          ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Kokyu_EC"), 1);
        }

      supplier_ec.init_gateway(orb.in(),
                               poa.in(),
                               "file://consumer_supplier_ec.ior" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************
      RtEventChannelAdmin::RtSchedEventChannel_var supplier_ec_ior =
        supplier_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(supplier_ec_ior.in()
                                                    ACE_ENV_ARG_PARAMETER);

      ACE_OS::fprintf(ior_output_file, ior.in());
      ACE_OS::fclose(ior_output_file);


      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);

      supplier_ec.start(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //@BT: Timeouts start when orb starts, similar to starting the DT worker thread
      //DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 1, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t WORKER_ACTIVATED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (MAIN_GROUP_FAM, WORKER_ACTIVATED, 0, 0, NULL);
      //DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, m_id, 0, NULL);
      ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t WORKER_STARTED at %u\n",ACE_OS::gettimeofday().msec()));
      DSTRM_EVENT (WORKER_GROUP_FAM, WORKER_STARTED, 0, 0, NULL);

#ifdef ACE_HAS_DSUI
      EC_Event_Limit* e_limit = new EC_Event_Limit (orb, ds_cntl); //will delete ds_cntl
#else
      EC_Event_Limit* e_limit = new EC_Event_Limit (orb);
#endif //ACE_HAS_DSUI
      ACE_Time_Value ticker (100);//310);
      //orb->orb_core()->reactor()->schedule_timer(e_limit,0, ticker);
      long timer_id = rt.reactor()->schedule_timer(e_limit,0,ticker);
      if (timer_id < 0)
        {
          ACE_DEBUG((LM_DEBUG,"Supplier_EC (%t) could not schedule EC_Event_Limit timer\n"));
        }

      rt.activate(); //need thread creation flags? or priority?
      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // ****************************************************************

      //@BT: ORB shutting down; currently, this isn't expected to happen
      //DSTRM_EVENT (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 1, 0, NULL);
      //ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t CALL_SERVER_SHUTDOWN at %u\n",ACE_OS::gettimeofday().msec()));
      //DSTRM_EVENT (MAIN_GROUP_FAM, CALL_SERVER_SHUTDOWN, 0, 0, NULL);

      //@BT: Scheduler shuts down with the EC and ORB
      //DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 1, 0, NULL);
      //ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t SCHEDULER_SHUTDOWN at %u\n",ACE_OS::gettimeofday().msec()));
      //DSTRM_EVENT (MAIN_GROUP_FAM, SCHEDULER_SHUTDOWN, 0, 0, NULL);

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

      //@BT: Done clean up
      //DSTRM_EVENT (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 1, 0, NULL);
      //ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t AFTER_SERVER_SHUTDOWN at %u\n",ACE_OS::gettimeofday().msec()));
      //DSTRM_EVENT (MAIN_GROUP_FAM, AFTER_SERVER_SHUTDOWN, 0, 0, NULL);

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Service");
      return 1;
    }
  ACE_ENDTRY;
  /*
  //@BT
  //DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);
  ACE_DEBUG((LM_DEBUG,"Supplier_EC thread %t STOP at %u\n",ACE_OS::gettimeofday().msec()));
  DSTRM_EVENT(MAIN_GROUP_FAM, STOP, 1, 0, NULL);

  //Clean up DSUI stuff
  delete ds_cntl;
  */
  return 0;
}

// ****************************************************************

int parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "cs:o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = ACE_OS::fopen (get_opts.opt_arg (), "w");
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
                           "Usage:  %s -s <rms|muf|edf>"
                           "\n",
                           argv [0]),
                          -1);
      }
  if (ior_output_file == 0)
    ior_output_file = ACE_OS::fopen ("supplier_ec.ior", "w");
  // Indicates sucessful parsing of the command line
  return 0;
}
