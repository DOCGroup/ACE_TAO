// $Id$

#ifndef ECCONFIG_C
#define ECCONFIG_C

#include "ECConfig.h"

#include <sstream> //for ostringstream

#include "ace/Array.h"
#include "ace/Bound_Ptr.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"

namespace TestConfig {

template <class SCHED_STRAT>
ECConfig<SCHED_STRAT>::ECConfig (void)
  : Test_Config (),
    configured (0) //false
{
}

template <class SCHED_STRAT>
ECConfig<SCHED_STRAT>::~ECConfig (void)
{
  this->reset();
}

template <class SCHED_STRAT> void
ECConfig<SCHED_STRAT>::reset (void)
{
  // We should do a lot of cleanup (disconnect from the EC,
  // deactivate all the objects with the POA, etc.).

  delete this->ec_impl;

  delete this->sched_impl;

  for(size_t i=0; i<consumers.size(); ++i) {
    delete this->consumers[i];
  }

  for(size_t i=0; i<suppliers.size(); ++i) {
    delete this->suppliers[i];
  }

  configured = 0; //false
}

template <class SCHED_STRAT> int
ECConfig<SCHED_STRAT>::configure (TCFG_SET_WPTR testconfigs)
{
  if (configured) {
    this->reset(); //delete memory used by previous configuration
  }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->initEC();

      ////////////////// EC ready; do config ////////////////////
      size_t tsize = testconfigs->size();
      supplier_cfgs.size(tsize);
      consumer_cfgs.size(tsize);
      testcfgs.size(tsize);
      consumers.size(tsize);
      suppliers.size(tsize);
      for (size_t i=0; i<tsize; ++i)
        {
          //ACE_Weak_Bound_Ptr doesn't have operator*()! ??
          //test_config_t *curcfg = (*testconfigs)[i];
          test_config_t *curcfg = (*testconfigs.unsafe_get())[i];
          testcfgs[i] = curcfg;

          RtecScheduler::Criticality_t criticality;
          switch (curcfg->criticality) {
            case VERY_LOW_CRITICALITY :
              criticality = RtecScheduler::VERY_LOW_CRITICALITY;
              break;
            case LOW_CRITICALITY :
              criticality = RtecScheduler::LOW_CRITICALITY;
              break;
            case MEDIUM_CRITICALITY :
              criticality = RtecScheduler::MEDIUM_CRITICALITY;
              break;
            case HIGH_CRITICALITY :
              criticality = RtecScheduler::HIGH_CRITICALITY;
              break;
            case VERY_HIGH_CRITICALITY :
              criticality = RtecScheduler::VERY_HIGH_CRITICALITY;
              break;
          }

          RtecScheduler::Importance_t importance;
          switch (curcfg->importance) {
            case VERY_LOW_IMPORTANCE :
              importance = RtecScheduler::VERY_LOW_IMPORTANCE;
              break;
            case LOW_IMPORTANCE :
              importance = RtecScheduler::LOW_IMPORTANCE;
              break;
            case MEDIUM_IMPORTANCE :
              importance = RtecScheduler::MEDIUM_IMPORTANCE;
              break;
            case HIGH_IMPORTANCE :
              importance = RtecScheduler::HIGH_IMPORTANCE;
              break;
            case VERY_HIGH_IMPORTANCE :
              importance = RtecScheduler::VERY_HIGH_IMPORTANCE;
              break;
          }

          //create supplier RT_Info
          std::ostringstream supp_entry_pt;
          supp_entry_pt << "Supplier Event Class " << i; //unique RT_Info entry point
          RtecScheduler::handle_t rt_info =
            this->scheduler->create (supp_entry_pt.str().c_str() ACE_ENV_ARG_PARAMETER);
          ACE_Time_Value tv (0, curcfg->period);
          TimeBase::TimeT time;
          ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
          this->scheduler->set (rt_info,
                                criticality,
                                time, time, time,
                                time,
                                importance,
                                time,
                                0,
                                RtecScheduler::OPERATION
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (this->supplier_cfgs.set(rt_info,i) != 0) {
            ACE_DEBUG((LM_DEBUG, "Could not set supplier RT_Info into config: %d of %d\n",
                       i,consumer_cfgs.max_size()));
            return 1;
          }

          //create consumer RT_Info
          std::ostringstream cons_entry_pt;
          cons_entry_pt << "Consumer Event Class " << i; //unique RT_Info entry point
          rt_info =
            this->scheduler->create (cons_entry_pt.str().c_str() ACE_ENV_ARG_PARAMETER);
          tv.set (0, curcfg->period);
          ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
          this->scheduler->set (rt_info,
                                criticality,
                                time, time, time,
                                time,
                                importance,
                                time,
                                0,
                                RtecScheduler::OPERATION
                                ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (this->consumer_cfgs.set(rt_info,i) != 0) {
            ACE_DEBUG((LM_DEBUG, "Could not set consumer RT_Info into config: %d of %d\n",
                       i,consumer_cfgs.max_size()));
            return 1;
          }
        }

      this->connectConsumers();
      this->connectSuppliers();

      ACE_DEBUG ((LM_DEBUG, "Consumer RT_Infos:\n"));
      print_RT_Infos (this->consumer_cfgs);
      ACE_DEBUG ((LM_DEBUG, "\nSupplier RT_Infos:\n"));
      print_RT_Infos (this->supplier_cfgs);

      ////////////////// Configured; compute schedule ///////////
      ACE_DEBUG ((LM_DEBUG, "Computing schedule\n"));
      RtecScheduler::RT_Info_Set_var infos;
      RtecScheduler::Config_Info_Set_var configs;
      RtecScheduler::Scheduling_Anomaly_Set_var anomalies;

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
                                           configs.out (),
                                           anomalies.out ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Dump the schedule to a file..
      ACE_Scheduler_Factory::dump_schedule (infos.in (),
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
      RtecEventComm::EventSet event (1);
      event.length (1);

      ACE_Array<int> evt_counts(this->testcfgs.size());
      for(size_t i=0; i<this->testcfgs.size(); ++i)
        {
          //copy over total number of events per test_config_t to send
          evt_counts[i] = this->testcfgs[i]->num_entities;
        }

      size_t num_done = 0; //total number of testcfgs which have no more events to push
      while (num_done<this->testcfgs.size())
        {
          //for each consumer, push an event
          for(size_t i=0; i<this->testcfgs.size() && i<this->consumer_proxys.size(); ++i)
            {
              if (evt_counts[i]<=0)
                {
                  if (evt_counts[i]==0)
                    {
                      //just finished
                      ++num_done;
                      evt_counts[i]--; //indicate accounted for in num_done
                    } //else already incr num_done for this one
                  continue; //no more events of this to push
                } //else...
              test_config_t *tcfg = this->testcfgs[i];
              ProxyList::TYPE curproxy = this->consumer_proxys[i];

              event[0].header.type   = ACE_ES_EVENT_UNDEFINED+tcfg->type;
              event[0].header.source = supplier_ids[i];
              event[0].header.ttl    = 1;

              curproxy->push (event ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;

              //event pushed, so decr corresponding evt_count
              evt_counts[i]--;

              // BT TODO sleep until next period expires
              ACE_Time_Value rate (0, 10000);
              ACE_OS::sleep (rate);

          }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ECConfig");
      return 1;
    }
  ACE_ENDTRY;

  return 0; //successful run
}

template <class SCHED_STRAT> int
ECConfig<SCHED_STRAT>::initEC()
{
  TAO_EC_Kokyu_Factory::init_svcs ();

  ACE_DEBUG ((LM_DEBUG, "Initializing event channel\n"));
  ACE_TRY
    {
      // ORB initialization boiler plate...
      int argc = 0;
      char** argv = 0;
      this->orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->poa =
        PortableServer::POA::_narrow (object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      this->poa_manager =
        poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      // DO these need to remain in scope beyond this function?

      // Create a scheduling service
      ACE_NEW_RETURN (this->sched_impl,SCHED_STRAT,1);

      this->scheduler = sched_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create an event channel implementation...
      TAO_EC_Event_Channel_Attributes attributes (poa.in (),
                                                  poa.in ());
      attributes.scheduler = scheduler.in (); // no need to dup

      ACE_NEW_RETURN (this->ec_impl,TAO_EC_Event_Channel (attributes),1);

      this->event_channel =
        this->ec_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ECConfig");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}

template <class SCHED_STRAT> int
ECConfig<SCHED_STRAT>::connectConsumers()
{
  ACE_TRY
    {
      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        this->event_channel->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "connecting consumers\n"));
      for (size_t i=0; i<this->consumer_cfgs.size() && i<this->testcfgs.size(); ++i)
        {
          ACE_NEW_RETURN(this->consumers[i],Consumer(i),1);

          RtecScheduler::handle_t hndl = this->consumer_cfgs[i];
          test_config_t *tcfg = this->testcfgs[i];

          ACE_ConsumerQOS_Factory consumer_qos;
          //consumer_qos.start_disjunction_group ();
          // The types in the range [0,ACE_ES_EVENT_UNDEFINED) are
          // reserved for the EC...
          consumer_qos.insert_type (ACE_ES_EVENT_UNDEFINED+tcfg->type,
                                    hndl);

          RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy =
            consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RtecEventComm::PushConsumer_var consumerv =
            consumers[i]->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          supplier_proxy->connect_push_consumer (consumerv.in (),
                                                 consumer_qos.get_ConsumerQOS ()
                                                 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ECConfig");
      return 1;
    }
  ACE_ENDTRY;

  return 0; //successful run
}

template <class SCHED_STRAT> int
ECConfig<SCHED_STRAT>::connectSuppliers()
{
  ACE_TRY
    {
      // The canonical protocol to connect to the EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        event_channel->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "connecting suppliers\n"));
      supplier_ids.size(supplier_cfgs.size());
      consumer_proxys.size(supplier_cfgs.size());
      for (size_t i=0; i<supplier_cfgs.size() && i<this->testcfgs.size(); ++i)
        {
          ACE_NEW_RETURN(this->suppliers[i],Supplier(),1);

          RtecScheduler::handle_t hndl = this->supplier_cfgs[i];
          test_config_t *tcfg = this->testcfgs[i];

          RtecEventComm::EventSourceID supplier_id = i;
          this->supplier_ids[i] = supplier_id;

          ACE_SupplierQOS_Factory supplier_qos;
          supplier_qos.insert (supplier_id,
                               ACE_ES_EVENT_UNDEFINED+tcfg->type,
                               hndl,
                               1); // number of calls, but what does that mean?

          consumer_proxys[i] =
            supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          RtecEventComm::PushSupplier_var supplier =
            suppliers[i]->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          //PROBLEM: Occasional segfault here:
          consumer_proxys[i]->connect_push_supplier (supplier.in (),
                                                     supplier_qos.get_SupplierQOS ()
                                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      ACE_DEBUG ((LM_DEBUG, "suppliers connected\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ECConfig");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
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

} /* namespace TestConfig */

#endif /* ECCONFIG_C */
