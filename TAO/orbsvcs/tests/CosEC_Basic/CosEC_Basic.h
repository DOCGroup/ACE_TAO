/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = FILENAME
//   CosEC_Basic.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This is a simple test class for the Cos Event Channel.
// ============================================================================
#ifndef COSECBASIC_H
#define COSECBASIC_H

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Sched/Config_Scheduler.h"
#include "orbsvcs/Runtime_Scheduler.h"
#include "orbsvcs/Event/Event_Channel.h"
#include "orbsvcs/Event/Module_Factory.h"
#include "orbsvcs/CosEvent/EventChannel_i.h"
#include "CosECConsumer.h"
#include "CosECSupplier.h"

class CosEC_Basic
{
  // = TITLE
  //   class CosEC_Basic
  // = DESCRIPTION
  //   creates a CosEC (based on the RtEC) and sends an event across.
  //
public:
  // = Initialization and termination methods.
  CosEC_Basic (void);
  ~CosEC_Basic ();

  int initEC (int argc, char *argv []);
  // Starts up an ORB, a Rtec and the CosEC.

  void runtest ();
  // Connects a consumer and a supplier to the CosEC and sends 1 event across.

  void finish ();
  // Closes down the Rtec and CosEC.

private:
  CORBA::ORB_var orb_;
  // The ORB that we use.

  TAO_Reactive_Module_Factory module_factory;
  // The Module_Factory required by the Rtec.

  ACE_Config_Scheduler scheduler_impl;
  // Scheduler used by the Rtec.

  ACE_EventChannel ec_impl;
  // The servant object of the Rtec.

  RtecEventChannelAdmin::EventChannel_var rtec;
  // Reference to the Rtec returned after activating it in the ORB.

  ACE_ConsumerQOS_Factory consumer_qos;
  // The Consumer QOS.

  ACE_SupplierQOS_Factory supplier_qos;
  // The Supplier QOS.

  EventChannel_i ec_i;
  // The servant object of the COS Event Channel.

  CosEventChannelAdmin::EventChannel_ptr cos_ec;
  // Reference to the CosEC returned after activating it in the ORB.

  CosECConsumer consumer;
  // The Cos Consumer that will receive the event.

  CosECSupplier supplier;
  // The Cos Supplier that will supply the event.
};
#endif // COSECBASIC_H
