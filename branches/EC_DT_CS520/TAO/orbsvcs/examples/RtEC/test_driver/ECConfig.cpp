// $Id$

#ifndef ECCONFIG_C
#define ECCONFIG_C

#include "ECConfig.h"
#include "Consumer.h"
#include "Supplier.h"

#include <sstream> //for ostringstream

#include "ace/Array.h"
#include "ace/Bound_Ptr.h"
#include "ace/Thread_Manager.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SString.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"

namespace TestConfig {

//TODO: Obviously, we can't just hardcode these!
//And assuming only one supplier and consumer is bad, too.
const char *supplierEC_iorfile =
  "supplierEC.ior";
const char *supplierSched_iorfile =
  "supplierSched.ior";
const char *consumerEC_iorfile =
  "consumerEC.ior";
const char *consumerSched_iorfile =
  "consumerSched.ior";
const char *supplier_schedule =
  "supplier_schedule.out";
const char *consumer_schedule =
  "consumer_schedule.out";
const char *remote_inet_addr =
  "bhangra.doc.wustl.edu";
int remote_inet_port = 424242;

template <class SCHED_STRAT>
ECConfig<SCHED_STRAT>::ECConfig (void)
  : Test_Config ()
  , ec_impl(0)
  , sched_impl(0)
  , periods(0)
  , importances(0)
  , crits(0)
  , test_done(new ACE_RW_Mutex())
  //, udp_mcast_address(ACE_DEFAULT_MULTICAST_ADDR ":10001")
  , configured (0) //false
  , use_federated (1) //TODO Check whether or not FEDERATED; default to true
{
}

template <class SCHED_STRAT>
ECConfig<SCHED_STRAT>::~ECConfig (void)
{
  this->reset(ACE_ENV_SINGLE_ARG_PARAMETER);

  delete this->test_done;
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::reset (ACE_ENV_SINGLE_ARG_DECL)
{
  // We should do a lot of cleanup (disconnect from the EC,
  // deactivate all the objects with the POA, etc.).

  this->disconnect_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER); //should release all read locks on this->test_done

  this->disconnect_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);

  {
    // Deactivate the EC
    PortableServer::POA_var poa =
      this->ec_impl->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    PortableServer::ObjectId_var id =
      poa->servant_to_id (this->ec_impl ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "EC deactivated\n"));
  }

  {
    // Deactivate the Scheduler
    PortableServer::POA_var poa =
      this->sched_impl->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    PortableServer::ObjectId_var id =
      poa->servant_to_id (this->sched_impl ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    ACE_DEBUG ((LM_DEBUG, "scheduler deactivated\n"));
  }

  if (this->use_federated && !CORBA::is_nil(this->gateway_obs.in()))
    {
      // Deactivate the Gateway if it exists
      PortableServer::POA_var poa =
        this->gateway_impl->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this->gateway_impl ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      ACE_DEBUG ((LM_DEBUG, "gateway deactivated\n"));
    }

  delete this->ec_impl;
  this->ec_impl = 0;

  delete this->sched_impl;
  this->sched_impl = 0;

  delete this->gateway_impl;
  this->gateway_impl = 0;

  //TODO clear config_infos?

  //TODO clear RT_Infos from scheduler?

  configured = 0; //false
}

template <class SCHED_STRAT> int
ECConfig<SCHED_STRAT>::configure (TCFG_SET_WPTR testconfigs)
{
  if (this->configured) {
    ACE_DEBUG((LM_DEBUG,ACE_TEXT("Resetting EC\n")));
    this->reset(); //delete memory used by previous configuration
  }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->initEC();

      ACE_DEBUG((LM_DEBUG,ACE_TEXT("EC Initialized\n")));

      ////////////////// EC ready; do config ////////////////////
      size_t tsize = testconfigs->size();
      size_t supp_size = 0; //number of suppliers (sources)
      size_t cons_size = 0; //number of consumers (sinks)
      this->testcfgs.size(tsize);
      this->periods.size(tsize);
      this->importances.size(tsize);
      this->crits.size(tsize);
      for (size_t i=0; i<tsize; ++i)
        {
          //ACE_Weak_Bound_Ptr doesn't have operator*()! ??
          //test_config_t *curcfg = (*testconfigs)[i];
          test_config_t *curcfg = (*testconfigs.unsafe_get())[i];
          this->testcfgs[i] = curcfg;

          switch (curcfg->criticality) {
            case VERY_LOW_CRITICALITY :
              this->crits[i] = RtecScheduler::VERY_LOW_CRITICALITY;
              break;
            case LOW_CRITICALITY :
              this->crits[i] = RtecScheduler::LOW_CRITICALITY;
              break;
            case MEDIUM_CRITICALITY :
              this->crits[i] = RtecScheduler::MEDIUM_CRITICALITY;
              break;
            case HIGH_CRITICALITY :
              this->crits[i] = RtecScheduler::HIGH_CRITICALITY;
              break;
            case VERY_HIGH_CRITICALITY :
              this->crits[i] = RtecScheduler::VERY_HIGH_CRITICALITY;
              break;
          }

          switch (curcfg->importance) {
            case VERY_LOW_IMPORTANCE :
              this->importances[i] = RtecScheduler::VERY_LOW_IMPORTANCE;
              break;
            case LOW_IMPORTANCE :
              this->importances[i] = RtecScheduler::LOW_IMPORTANCE;
              break;
            case MEDIUM_IMPORTANCE :
              this->importances[i] = RtecScheduler::MEDIUM_IMPORTANCE;
              break;
            case HIGH_IMPORTANCE :
              this->importances[i] = RtecScheduler::HIGH_IMPORTANCE;
              break;
            case VERY_HIGH_IMPORTANCE :
              this->importances[i] = RtecScheduler::VERY_HIGH_IMPORTANCE;
              break;
          }

          ACE_Time_Value tv;
          tv.msec(curcfg->period);
          ORBSVCS_Time::Time_Value_to_TimeT (this->periods[i], tv);

          if (curcfg->comptype == TestConfig::SOURCE)
            {
              ++supp_size;
            }
          else if (curcfg->comptype == TestConfig::SINK)
            {
              ++cons_size;
            }
        }

      //SUPPLIER writes IORs and blocks
      if (supp_size > 0 && this->use_federated)
        {
          //since there are suppliers, we assume we are a supplier only
          //save EC and scheduler IOR for federation
          CORBA::String_var ior =
            orb->object_to_string (this->event_channel.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          ACE_DEBUG((LM_DEBUG,"Writing supplier EC IOR\n"));
          // Output the EC's ior to the supplierEC iorfile
          FILE *output_file = ACE_OS::fopen (supplierEC_iorfile, "w");
          if (output_file == 0)
            ACE_ERROR ((LM_ERROR,
                        "Cannot open output file for writing EC IOR: %s\n",
                        supplierEC_iorfile));
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          ior = orb->object_to_string (this->scheduler.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          ACE_DEBUG((LM_DEBUG,"Writing supplier Sched IOR\n"));
          // Output the scheduler's ior to the supplierSched iorfile
          output_file = ACE_OS::fopen (supplierSched_iorfile, "w");
          if (output_file == 0)
            ACE_ERROR ((LM_ERROR,
                        "Cannot open output file for writing scheduler IOR: %s\n",
                        supplierSched_iorfile));
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          //now we block until the client writes its IOR
          this->barrier(true);
        }

      //CONSUMER writes IORs and blocks
      if (cons_size > 0 && this->use_federated)
        {
          //since there are consumers, we assume we are a consumer only
          //save IOR for federation
          CORBA::String_var ior =
            orb->object_to_string (this->event_channel.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          ACE_DEBUG((LM_DEBUG,"Writing consumer EC IOR\n"));
          // Output the EC's ior to the consumerEC iorfile
          FILE *output_file = ACE_OS::fopen (consumerEC_iorfile, "w");
          if (output_file == 0)
            ACE_ERROR ((LM_ERROR,
                        "Cannot open output file for writing EC IOR: %s\n",
                        consumerEC_iorfile));
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          ACE_DEBUG((LM_DEBUG,"Writing consumer Sched IOR\n"));
          ior = orb->object_to_string (this->scheduler.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          // Output the scheduler's ior to the consumerSched iorfile
          output_file = ACE_OS::fopen (consumerSched_iorfile, "w");
          if (output_file == 0)
            ACE_ERROR ((LM_ERROR,
                        "Cannot open output file for writing scheduler IOR: %s\n",
                        consumerSched_iorfile));
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);

          //now we block until the supplier writes its IOR
          this->barrier(false);
        }

      if (this->use_federated && cons_size>0) //only federate on consumer side
        {
          //gateway EC_Control does not appear to setup gateway on
          //activation, so we need to set it up BEFORE any consumers
          //or suppliers connect!
          this->make_federated(ACE_ENV_SINGLE_ARG_PARAMETER);
        }

      this->consumers.size(cons_size);
      this->connect_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
      this->suppliers.size(supp_size);
      this->connect_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);

      if (this->consumers.size() > 0)
        {
          //this->barrier(false); //wait until both apps are ready to schedule
        }

      ////////////////// Configured; compute schedule ///////////
      ACE_DEBUG ((LM_DEBUG, "Computing schedule\n"));
      RtecScheduler::RT_Info_Set_var infos;
      RtecScheduler::Config_Info_Set_var configs;
      RtecScheduler::Scheduling_Anomaly_Set_var anomalies;
      RtecScheduler::Dependency_Set_var deps;

      // Obtain the range of valid priorities in the current
      // platform, the scheduler hard-code this values in the
      // generated file, but in the future we may just use the
      // "logical" priorities and define the mapping to OS
      // priorities at run-time.
      int min_os_priority =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);
      int max_os_priority =
        ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);
      this->scheduler->compute_scheduling (min_os_priority,
                                           max_os_priority,
                                           infos.out (),
                                           deps.out (),
                                           configs.out (),
                                           anomalies.out ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Dump the schedule to a file... (different files for supplier and consumer)
      if (this->suppliers.size() > 0)
        {
          ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                                deps.in(),
                                                configs.in (),
                                                anomalies.in (),
                                                supplier_schedule);
        }
      if (this->consumers.size() > 0)
        {
          ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                                deps.in(),
                                                configs.in (),
                                                anomalies.in (),
                                                consumer_schedule);
        }

      ///////////// Activate the EC /////////////////
      ACE_DEBUG ((LM_DEBUG, "activating EC\n"));
      this->ec_impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "EC activated\n"));

      if (this->suppliers.size() > 0)
        {
          //this->barrier(true); //wait until both apps are ready to schedule
        }

      configured = 1; //true
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ECConfig");
      return 1;
    }
  ACE_ENDTRY;
  return 0; //successful config
}

template <class SCHED_STRAT> int
ECConfig<SCHED_STRAT>::run (void)
{
  ACE_DEBUG ((LM_DEBUG, "Running ECConfig\n"));
  if (!this->configured) {
    ACE_DEBUG ((LM_DEBUG, "Tried to run before configured\n"));
    return 1;
  }

  ACE_TRY
    {
      ACE_Thread_Manager *inst = ACE_Thread_Manager::instance();

      // Spawn orb thread (which calls orb.run(), then terminates on return)
      ACE_DEBUG((LM_DEBUG,"SPAWNING ORB thread\n"));
      int ret = inst->spawn(ECConfig<SCHED_STRAT>::run_orb,&(this->orb));
      //no need for getting tid?
      if (ret == -1)
        {
          ACE_DEBUG ((LM_DEBUG, "ERROR: Couldn't spawn ORB->run() thread: %s\n",
                      ACE_OS::strerror(errno)));
          return 1;
        }

      //      Block waiting for consumers to finish
      //when can acquire write lock, all Suppliers are finished
      ret = this->test_done->acquire_write();
      if (ret == -1)
        {
          ACE_DEBUG((LM_DEBUG, "ERROR: could not acquire write lock for ECConfig: %s\n",
                     ACE_OS::strerror(errno)));
          return 1;
        }

      //all Suppliers done, so stop EC and ORB
      //Shutdown EC
      this->reset();

      // Shutdown ORB
      this->orb->shutdown(1); //argument is TRUE

      if (inst->wait() == -1) //wait for ORB thread to terminate
        {
          ACE_ERROR((LM_ERROR, "ERROR: Thread_Manager wait failed: %s\n",
                     ACE_OS::strerror(errno)));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "suppliers finished\n"));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ECConfig");
      return 1;
    }
  ACE_ENDTRY;

  return 0; //successful run
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::initEC(ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_EC_Kokyu_Factory::init_svcs ();

  ACE_DEBUG ((LM_DEBUG,ACE_TEXT("Initializing event channel\n")));

  // ORB initialization boiler plate...
  int argc = 0;
  char** argv = 0;
  this->orb =
    CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,ACE_TEXT("Resolving initial references\n")));

  CORBA::Object_var object =
    orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->poa =
    PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->poa_manager =
    poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  //TODO: do these need to remain in scope beyond this function?

  ACE_DEBUG((LM_DEBUG,ACE_TEXT("Creating sched service\n")));

  // Create a scheduling service
  ACE_NEW (this->sched_impl,SCHED_STRAT);

  this->scheduler = sched_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create an event channel implementation...
  TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                              poa.in ());
  attributes.scheduler = scheduler.in (); // no need to dup

  ACE_NEW (this->ec_impl,TAO_EC_Event_Channel (attributes));

  ACE_DEBUG((LM_DEBUG,ACE_TEXT("Created ec_impl\n")));

  this->event_channel =
    this->ec_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::make_federated (ACE_ENV_SINGLE_ARG_DECL)
{
  //TODO: IOR location is hardcoded. Obviously, this is BAD BAD BAD.
  //TODO: This assumes make_federated() is only called by consumer!!!

  //get IOR of remote EC
  ACE_CString remoteEC_ior("file://");
  ACE_CString remoteSched_ior("file://");
  //since we're hardcoding filenames anyway, this isn't much worse!

  remoteEC_ior += supplierEC_iorfile;
  remoteSched_ior += supplierSched_iorfile;

  ACE_DEBUG((LM_DEBUG,"Reading EC IOR: %s\n",remoteEC_ior.c_str()));
  CORBA::Object_var ec_obj =
    orb->string_to_object (remoteEC_ior.c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::EventChannel_var remote_ec =
    RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in ()
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"Reading Sched IOR: %s\n",remoteSched_ior.c_str()));
  CORBA::Object_var sched_obj = orb->string_to_object(remoteSched_ior.c_str() ACE_ENV_ARG_PARAMETER);
  RtecScheduler::Scheduler_var remote_sch =
    RtecScheduler::Scheduler::_narrow (sched_obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"Creating gateway\n"));
  TAO_EC_Gateway_Sched *gateway = new TAO_EC_Gateway_Sched();

  ACE_DEBUG((LM_DEBUG,"Supplier gateway init\n"));
  //for consumer, remote is supplier EC
  gateway->init (remote_ec.in (),
                 this->event_channel.in (),
                 remote_sch.in (),
                 this->scheduler.in(),
                 supplierEC_iorfile, //use EC IOR files as names, since they should be unique
                 consumerEC_iorfile
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->gateway_impl = gateway;
  ACE_DEBUG((LM_DEBUG,"Gateway init completed\n"));

  this->gateway_obs = this->gateway_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TRY
    {
      //Might throw a CANT_APPEND_OBSERVER exception
      //But I think we can ignore it if that happens
      ACE_DEBUG((LM_DEBUG,"Appending gateway observer\n"));
      //append to consumer EC so that connecting consumers update the gateway!
      RtecEventChannelAdmin::Observer_Handle h =
        this->event_channel->append_observer (this->gateway_obs.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->gateway_impl->observer_handle (h);

      ACE_DEBUG((LM_DEBUG,"Gateway observer appended\n"));
    }
  ACE_CATCH(RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER,exc)
    {
      //ignore
      ACE_DEBUG((LM_DEBUG,"Caught CANT_APPEND_OBSERVER\n"));
    }
  ACE_ENDTRY;

  ACE_DEBUG((LM_DEBUG,"Gateway created\n"));
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::connect_suppliers (ACE_ENV_SINGLE_ARG_DECL)
{
  RtecEventComm::EventSet event(1);
  event.length(1);

  ACE_DEBUG((LM_DEBUG,"Suppliers to connect: %d\n",this->suppliers.size()));
  //this->suppliers already has correct size
  size_t supp_idx = 0;
  for (size_t i=0; i<this->testcfgs.size(); ++i)
    {
      if (this->testcfgs[i]->comptype == SOURCE)
        {
          ACE_ASSERT(supp_idx < this->suppliers.size());
          ACE_NEW (this->suppliers[supp_idx], Supplier ());

          event[0].header.type   = ACE_ES_EVENT_UNDEFINED+this->testcfgs[i]->type;
          event[0].header.source = supp_idx; //supplier_id
          event[0].header.ttl    = 1;

          std::ostringstream entry_prefix;
          entry_prefix << "Supplier " << supp_idx;

          ACE_DEBUG((LM_DEBUG,"Supplier.connect() for %s\n",entry_prefix.str().c_str()));
          this->suppliers[supp_idx]->connect (this->test_done,
                                              this->scheduler.in(),
                                              entry_prefix.str().c_str(),
                                              this->periods[i], //period
                                              this->importances[i], //importance
                                              this->crits[i], //criticality
                                              supp_idx, //supplier_id
                                              this->testcfgs[i]->num_entities, //to_send
                                              event, //event set
                                              this->event_channel.in()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          ++supp_idx; //proceed to next supplier
        }
    }
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::connect_consumers (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_DEBUG((LM_DEBUG,"Consumers to connect: %d\n",this->consumers.size()));
  //this->consumers already has correct size
  size_t cons_idx = 0;
  for (size_t i=0; i<this->testcfgs.size(); ++i)
    {
      if (this->testcfgs[i]->comptype == SINK)
        {
          ACE_ASSERT(cons_idx < this->consumers.size());
          ACE_NEW (this->consumers[cons_idx], Consumer ());

          std::ostringstream entry_prefix;
          entry_prefix << "Consumer " << cons_idx;

          ACE_DEBUG((LM_DEBUG,"Consumer.connect() for %s\n",entry_prefix.str().c_str()));
          //don't set the RT_Info values
          this->consumers[cons_idx]->connect (this->scheduler.in(),
                                              entry_prefix.str().c_str(),
                                              cons_idx, //consumer id
                                              ACE_ES_EVENT_UNDEFINED+this->testcfgs[i]->type, //type
                                              this->periods[i],
                                              this->importances[i],
                                              this->crits[i],
                                              this->event_channel.in()
                                              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          ++cons_idx; //proceed to next sink
        }
    }
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::disconnect_suppliers (ACE_ENV_SINGLE_ARG_DECL)
{
  for (size_t i = 0; i < this->suppliers.size(); ++i)
    {
      this->suppliers[i]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete this->suppliers[i];
      this->suppliers[i] = 0;
    }
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::disconnect_consumers (ACE_ENV_SINGLE_ARG_DECL)
{
  for (size_t i = 0; i < this->consumers.size(); ++i)
    {
      this->consumers[i]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      delete this->consumers[i];
      this->consumers[i] = 0;
    }
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::print_RT_Infos (ACE_Array<RtecScheduler::handle_t> cfg_set)
{
  char *rt_info_format = "{%20s, %10d, %10d, %10d, "
    "%10d, %10d, "
    "(RtecScheduler::Criticality_t) %d, "
    "(RtecScheduler::Importance_t) %d, "
    "%10d, %10d, %10d, %10d, %10d, "
    "(RtecScheduler::Info_Type_t) %d }";
  ACE_TRY
    {
      for (size_t i=0; i<cfg_set.size(); ++i) {
        RtecScheduler::handle_t hndl = cfg_set[i];
        RtecScheduler::RT_Info info = *(this->scheduler->get(hndl));
        ACE_TRY_CHECK;

        if (i!=0)
          {
            //finish previous line
            ACE_DEBUG ((LM_DEBUG, "\n"));
          }
        ACE_DEBUG ((LM_DEBUG, rt_info_format,
                    (const char *) info.entry_point,
                    info.handle,
                    ACE_CU64_TO_CU32 (info.worst_case_execution_time),
                    ACE_CU64_TO_CU32 (info.typical_execution_time),
                    ACE_CU64_TO_CU32 (info.cached_execution_time),
                    info.period,
                    info.criticality,
                    info.importance,
                    ACE_CU64_TO_CU32 (info.quantum),
                    info.threads,
                    info.priority,
                    info.preemption_subpriority,
                    info.preemption_priority,
                    info.info_type));
      }
      //finish last line
      ACE_DEBUG ((LM_DEBUG, "\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ECConfig");
    }
  ACE_ENDTRY;

}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::barrier(bool is_supplier)
{
  //if we are the server (we have suppliers), then we accept.
  //if we are the client (we have consumers), then we connect.

  //first, create address
  ACE_CString addr_str(remote_inet_addr);
  char port_str[7];
  ACE_OS::sprintf(port_str,"%d",remote_inet_port);
  port_str[6] = 0;
  addr_str += ":";
  addr_str += port_str;
  remote_inet_port++; //increment port for next barrier

  if (is_supplier)
    {
      //now we block on a socket connect until a consumer opens it
      //this way, we don't start running until the consumer is ready
      ACE_SOCK_Stream accstrm;
      ACE_DEBUG((LM_DEBUG,"Opening supplier socket %s\n",addr_str.c_str()));
      ACE_INET_Addr addr(addr_str.c_str());
      ACE_SOCK_Acceptor acc(addr);
      if (acc.accept(accstrm,&addr) != 0) //blocks until consumer opens
        {
          ACE_ERROR((LM_ERROR,
                     "Cannot accept socket: %s\n",
                     ACE_OS::strerror(errno)));
        }
      ACE_DEBUG((LM_DEBUG,"Supplier: unblocked on socket\n"));

      //once opened, no need for socket any more
      acc.close();
      accstrm.close();
      ACE_DEBUG((LM_DEBUG, "Supplier: closed socket\n"));
    }
  else
    {
      //now we open a socket to start up the supplier
      ACE_DEBUG((LM_DEBUG,"Connecting consumer socket %s\n",addr_str.c_str()));
      ACE_SOCK_Stream connstrm;
      ACE_INET_Addr addr(addr_str.c_str());
      ACE_SOCK_Connector conn;
      if (conn.connect(connstrm,addr) != 0) //blocks until supplier opens
        {
          ACE_ERROR((LM_ERROR,
                     "Consumer cannot connect socket: %s\n",
                     ACE_OS::strerror(errno)));
        }
      ACE_DEBUG((LM_DEBUG,"Consumer: connected socket\n"));

      //once opened, no need for socket any more
      connstrm.close();
      ACE_DEBUG((LM_DEBUG, "Consumer: closed socket\n"));
    }
}

template <class SCHED_STRAT> ACE_THR_FUNC_RETURN
ECConfig<SCHED_STRAT>::run_orb(void *data)
{
  //ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      ACE_DEBUG((LM_DEBUG, "ORB thread: Casting %x\n",data));

      CORBA::ORB_var orb = *(ACE_reinterpret_cast(CORBA::ORB_var*,data));

      ACE_DEBUG((LM_DEBUG, "ORB thread: Running orb\n"));

      orb->run();
      //this method returns when orb->shutdown() is called; then thread exits

      ACE_DEBUG((LM_DEBUG, "ORB thread: Shutdown\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "ECConfig ORB thread");
    }
  ACE_ENDTRY;

  return 0;
}

} /* namespace TestConfig */

#endif /* ECCONFIG_C */
