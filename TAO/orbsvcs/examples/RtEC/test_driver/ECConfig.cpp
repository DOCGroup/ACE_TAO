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
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecEventCommC.h"

#include "orbsvcs/Event/ECG_Mcast_EH.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "orbsvcs/Event/ECG_UDP_Receiver.h"
#include "orbsvcs/Event/ECG_UDP_Out_Endpoint.h"
#include "tao/ORB_Core.h"

namespace TestConfig {

template <class SCHED_STRAT>
ECConfig<SCHED_STRAT>::ECConfig (void)
  : Test_Config ()
  , ec_impl(0)
  , sched_impl(0)
  , periods(0)
  , importances(0)
  , crits(0)
  , test_done(new ACE_RW_Mutex())
  , udp_mcast_address(ACE_DEFAULT_MULTICAST_ADDR ":10001")
  , configured (0) //false
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

  delete this->ec_impl;
  this->ec_impl = 0;

  delete this->sched_impl;
  this->sched_impl = 0;

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

      this->consumers.size(cons_size);
      this->connect_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
      this->suppliers.size(supp_size);
      this->connect_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);

      if (1) ///TODO Check whether or not FEDERATED; default to true
        {
          this->make_federated(ACE_ENV_SINGLE_ARG_PARAMETER);
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

      // Dump the schedule to a file..
      ACE_Scheduler_Factory::dump_schedule (infos.in (),
                                            deps.in(),
                                            configs.in (),
                                            anomalies.in (),
                                            "ecconfig.out");

      ///////////// Activate the EC /////////////////
      ACE_DEBUG ((LM_DEBUG, "activating EC\n"));
      this->ec_impl->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "EC activated\n"));

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
  // DO these need to remain in scope beyond this function?

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
  //This code taken from the $TAO_ROOT/orbsvcs/examples/RtEC/MCast example

  // The next step is to setup the multicast gateways.
  // There are two gateways involved, one sends the locally
  // generated events to the federated peers, the second gateway
  // receives multicast traffic and turns it into local events.

  // The sender requires a helper object to select what
  // multicast group will carry what traffic, this is the
  // so-called 'Address Server'.
  // The intention is that advanced applications can use different
  // multicast groups for different events, this can exploit
  // network interfaces that filter unwanted multicast traffic.
  // The helper object is accessed through an IDL interface, so it
  // can reside remotely.
  // In this example, and in many application, using a fixed
  // multicast group is enough, and a local address server is the
  // right approach.

  // First we convert the string into an INET address, then we
  // convert that into the right IDL structure:
  ACE_INET_Addr udp_addr (udp_mcast_address);
  ACE_DEBUG ((LM_DEBUG,
              "Multicast address is: %s\n",
              udp_mcast_address));
  RtecUDPAdmin::UDP_Addr addr;
  addr.ipaddr = udp_addr.get_ip_address ();
  addr.port   = udp_addr.get_port_number ();

  // Now we create and activate the servant
  this->as_impl.set_addr (addr);
  this->address_server = as_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // We need a local socket to send the data, open it and check
  // that everything is OK:
  if (this->endpoint.dgram ().open (ACE_Addr::sap_any) == -1)
    {
      ACE_ERROR ((LM_ERROR, "Cannot open send endpoint\n"));
    }

  // Now we setup the sender:
  this->sender = TAO_ECG_UDP_Sender::create();
  this->sender->init (this->event_channel.in (),
                      this->address_server.in (),
                      &(this->endpoint)
                      ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // Now we connect the sender as a consumer of events, it will
  // receive any event from any source and send it to the "right"
  // multicast group, as defined by the address server set above:
  RtecEventChannelAdmin::ConsumerQOS sub;
  sub.is_gateway = 1;

  sub.dependencies.length (1);
  sub.dependencies[0].event.header.type =
    ACE_ES_EVENT_ANY;        // first free event type
  sub.dependencies[0].event.header.source =
    ACE_ES_EVENT_SOURCE_ANY; // Any source is OK

  sender->connect (sub ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // To receive events we need to setup an event handler:
  this->receiver = TAO_ECG_UDP_Receiver::create();
  TAO_ECG_Mcast_EH mcast_eh (&(*this->receiver));

  // The event handler uses the ORB reactor to wait for multicast
  // traffic:
  mcast_eh.reactor (this->orb->orb_core ()->reactor ());

  // The multicast Event Handler needs to know to what multicast
  // groups it should listen to.  To do so it becomes an observer
  // with the event channel, to determine the list of events
  // required by all the local consumer.
  // Then it register for the multicast groups that carry those
  // events:
  mcast_eh.open (this->event_channel.in ()
                 ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // Again the receiver connects to the event channel as a
  // supplier of events, using the Observer features to detect
  // local consumers and their interests:
  receiver->init (this->event_channel.in (),
                  &endpoint,
                  this->address_server.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  // The Receiver is also a supplier of events.  The exact type of
  // events is only known to the application, because it depends
  // on the traffic carried by all the multicast groups that the
  // different event handlers subscribe to.
  // In this example we choose to simply describe our publications
  // using wilcards, any event from any source.  More advanced
  // application could use the Observer features in the event
  // channel to update this information (and reduce the number of
  // multicast groups that each receive subscribes to).
  // In a future version the event channel could perform some of
  // those tasks automatically
  RtecEventChannelAdmin::SupplierQOS pub;
  pub.publications.length (1);
  pub.publications[0].event.header.type   = ACE_ES_EVENT_ANY;
  pub.publications[0].event.header.source = ACE_ES_EVENT_SOURCE_ANY;
  pub.is_gateway = 1;

  this->receiver->connect (pub ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;
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
