// $Id$

#include "ace/SString.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"

#include "Kokyu_EC.h"
#include "Consumer.h"
#include "Supplier.h"

#include <dsui_types.h>
#include "federated_dsui_families.h"

namespace
{
  int config_run = 0;
  ACE_CString sched_type ="rms";
  FILE * ior_output_file;
}

inline RtecScheduler::Period_t time_val_to_period (const ACE_Time_Value &tv)
{
  //100s of nanoseconds
  return (tv.sec () * 1000000 + tv.usec ())*10;
}

int parse_args (int argc, char *argv[]);

int
main (int argc, char* argv[])
{
  //TAO_EC_Default_Factory::init_svcs ();
  ds_control ctrl ("Federated_Test_Kokyu","federated_enabled.dsui");

  TAO_EC_Kokyu_Factory::init_svcs ();


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
                      "Usage: Service [-o IOR_file_name]\n"));
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

      // ****************************************************************

      Kokyu_EC kokyu_ec;
      if (kokyu_ec.init(sched_type.c_str(), poa.in()) == -1)
        ACE_ERROR_RETURN((LM_ERROR, "Unable to initialize Kokyu_EC"), 1);

      // ****************************************************************
      RtecEventComm::EventSourceID supplier_id1 = 1, supplier_id2 = 2;
      RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy1, consumer_proxy2;
      Supplier supplier_impl1(supplier_id1);
      Supplier supplier_impl2(supplier_id2);
      RtecEventComm::PushSupplier_var supplier1, supplier2;

      supplier1 = supplier_impl1._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      supplier2 = supplier_impl2._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t supplier1_rt_info =
        kokyu_ec.register_supplier("supplier1",
                                    supplier_id1,
                                    ACE_ES_EVENT_UNDEFINED,
                                    supplier1,
                                    consumer_proxy1
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t supplier2_rt_info =
        kokyu_ec.register_supplier("supplier2",
                                    supplier_id2,
                                    ACE_ES_EVENT_UNDEFINED+1,
                                    supplier2,
                                    consumer_proxy2
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      supplier_impl1.set_consumer_proxy(consumer_proxy1.in());
      supplier_impl2.set_consumer_proxy(consumer_proxy2.in());

      ACE_DEBUG ((LM_DEBUG, "suppliers connected\n"));

      // *************************************************************
      // Create a consumer, intialize its RT_Info structures, and
      // connnect to the event channel....

      Consumer consumer_impl1, consumer_impl2;
      RtecEventChannelAdmin::ProxyPushSupplier_var  proxy_supplier1,
                                                    proxy_supplier2;

      //consumer's rate will get propagated from the supplier.
      //so no need to specify a period here. Specifying
      //criticality is crucial since it propagates from
      //consumer to supplier.
      ACE_Time_Value tv (0,0);
      TimeBase::TimeT tmp;
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      RtEventChannelAdmin::SchedInfo info;
      info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecEventComm::PushConsumer_var consumer1 =
        consumer_impl1._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;


      RtecScheduler::handle_t consumer1_rt_info =
        kokyu_ec.register_consumer("consumer1",
                                   info,
                                   ACE_ES_EVENT_UNDEFINED,
                                   consumer1.in(),
                                   proxy_supplier1.out()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      info.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
      info.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;

      RtecEventComm::PushConsumer_var consumer2 =
        consumer_impl2._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      RtecScheduler::handle_t consumer2_rt_info =
        kokyu_ec.register_consumer("consumer2",
                                   info,
                                   ACE_ES_EVENT_UNDEFINED + 1,
                                   consumer2.in(),
                                   proxy_supplier2.out()
                                   ACE_ENV_ARG_PARAMETER);




      // ****************************************************************

      //Timer Registration part

      //Timeout consumers for the two suppliers.
      Timeout_Consumer timeout_consumer_impl1(&supplier_impl1);
      Timeout_Consumer timeout_consumer_impl2(&supplier_impl2);

      RtecEventChannelAdmin::ProxyPushSupplier_var timeout_supplier_proxy1,
                                                   timeout_supplier_proxy2;
      RtecEventComm::PushConsumer_var safe_timeout_consumer1,
                                      safe_timeout_consumer2;

      safe_timeout_consumer1= timeout_consumer_impl1._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      safe_timeout_consumer2= timeout_consumer_impl2._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      //Period = 1sec
      tv.set (1,0);
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);

      info.criticality = RtecScheduler::VERY_LOW_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_LOW_IMPORTANCE;
      info.threads = 0;
      info.info_type = RtecScheduler::OPERATION;

      RtecScheduler::handle_t supplier1_timeout_consumer_rt_info =
        kokyu_ec.register_consumer("supplier1_timeout_consumer",
                                   info,
                                   ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                   safe_timeout_consumer1.in(),
                                   timeout_supplier_proxy1.out()
                                   ACE_ENV_ARG_PARAMETER);


      tv.set (3, 0);
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);
      info.criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
      info.period = time_val_to_period (tv);
      info.importance = RtecScheduler::VERY_HIGH_IMPORTANCE;

      RtecScheduler::handle_t supplier2_timeout_consumer_rt_info =
        kokyu_ec.register_consumer("supplier2_timeout_consumer",
                                   info,
                                   ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                   safe_timeout_consumer2.in(),
                                   timeout_supplier_proxy2.out()
                                   ACE_ENV_ARG_PARAMETER);

      ACE_DEBUG ((LM_DEBUG, "timeout consumers connected\n"));

      // ****************************************************************
      //Registering dependency between timeout consumers and our suppliers
      //with the scheduler

      kokyu_ec.add_dependency (supplier1_timeout_consumer_rt_info,
                                 supplier1_rt_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      kokyu_ec.add_dependency (supplier2_timeout_consumer_rt_info,
                                 supplier2_rt_info,
                                 1,
                                 RtecBase::TWO_WAY_CALL
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      // ****************************************************************
      RtEventChannelAdmin::RtSchedEventChannel_var kokyu_ec_ior =
        kokyu_ec._this(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior = orb->object_to_string(kokyu_ec_ior.in()
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


      // ****************************************************************

      ACE_hthread_t thr_handle;
      ACE_Thread::self (thr_handle);

      int prio = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO);
      ACE_OS::thr_setprio (thr_handle, prio);


      orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);

      // ****************************************************************

      // We should do a lot of cleanup (disconnect from the EC,
      // deactivate all the objects with the POA, etc.) but this is
      // just a simple demo so we are going to be lazy.

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Kokyu_EventService - Service");
      return 1;
    }
  ACE_ENDTRY;
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
                           "usage:  %s -s <rms|muf>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}
