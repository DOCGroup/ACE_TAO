/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = FILENAME
//   ECConfig
//
// = AUTHOR
//   Bryan Thrall (thrall@cse.wustl.edu)
//
// ============================================================================

#ifndef ECCONFIG_H
#define ECCONFIG_H

#include "ace/Array.h"
//#include "ace/Synch.h"
#include "ace/RW_Mutex.h"
#include "orbsvcs/RtecSchedulerS.h" //for POA_RtecScheduler
#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

#include "TestConfig.h"
#include "Consumer.h"
#include "TimeoutConsumer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TestConfig {

typedef ACE_Array<RtecEventChannelAdmin::ProxyPushConsumer_var> ProxyList;
typedef ACE_Array<RtecScheduler::handle_t> ConfigList;
typedef ACE_Array<RtecEventComm::EventSourceID> SupplierIDList;
typedef ACE_Array<TimeBase::TimeT> PeriodList;
typedef ACE_Array<RtecScheduler::Importance_t> ImportanceList;
typedef ACE_Array<RtecScheduler::Criticality_t> CritList;

typedef ACE_Array<Consumer*> ConsumerList;
typedef ACE_Array<TimeoutConsumer*> SupplierList;

template <class SCHED_STRAT>
class ECConfig : public Test_Config {
public:
  ECConfig (void);

  virtual ~ECConfig (void);

  virtual int configure (TCFG_SET_WPTR configs);
  //does not take ownership of the Test_Config_Set but
  //needs to use the test_config_t in that set until
  //the ECConfig goes out of scope

  virtual int run (void);
  //If we try to distinguish between final-push and final-receipt,
  //that might be tracked in the Consumer (which would be easy as long
  //as filtering and correlation isn't used -- in that case, there
  //might be more than one receiver of an event, so you might get
  //multiple receipt-notices). There could also be a race-condition
  //problem between the various threads when reporting the receipt of
  //events.

protected:
  // TODO USE DEFAULTS FOR ANY OF THESE?

  virtual void initEC (ACE_ENV_SINGLE_ARG_DECL);

  void connect_suppliers (ACE_ENV_SINGLE_ARG_DECL);

  void disconnect_suppliers (ACE_ENV_SINGLE_ARG_DECL);

  void connect_consumers (ACE_ENV_SINGLE_ARG_DECL);

  void disconnect_consumers (ACE_ENV_SINGLE_ARG_DECL);

  virtual void reset (ACE_ENV_SINGLE_ARG_DECL);

private:

  void print_RT_Infos (ACE_Array<RtecScheduler::handle_t> cfg_set);

  static ACE_THR_FUNC_RETURN run_orb(void *data); //thread fcn for running ORB

  Test_Config_Set testcfgs;
  //copy of the currently configured Test_Config_Set
  //using the same test_config_t objects

  CORBA::ORB_var orb;

  PortableServer::POA_var poa;

  PortableServer::POAManager_var poa_manager;

  RtecEventChannelAdmin::EventChannel_var event_channel;

  RtecScheduler::Scheduler_var scheduler;

  TAO_EC_Event_Channel *ec_impl;

  SCHED_STRAT *sched_impl;

  ConsumerList consumers;

  SupplierList suppliers;

  PeriodList periods;

  ImportanceList importances;

  CritList crits;

  ACE_RW_Mutex* test_done;
  //TimeoutConsumers acquire read locks; when the ECConfig can acquire
  //a write lock, all TimeoutConsumers must've finished, so the test
  //is finished.

  int configured; //boolean
};

} /* namespace TestConfig */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ECConfig.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ECConfig.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ECCONFIG_H */
