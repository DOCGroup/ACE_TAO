/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   CosEC_Basic.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This is a simple test class for the standard Cos Event Channel.
//
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
  //
  // = DESCRIPTION
  //   Creates a CORBA Standard Event Channel (COSEC) implemented with
  //   TAO's Real-time Event Channel (RtEC) and sends an event across.
public:
  // = Initialization and termination methods.
  CosEC_Basic (void);
  // Constructor.

  ~CosEC_Basic (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Starts up an ORB, a Rtec and the CosEC.
  // Returns 0 on success, -1 on error.

  void run (void);
  // Connects a consumer and a supplier to the CosEC and sends 1 event
  // across.

  void shutdown (void);
  // Closes down the Rtec and CosEC.

private:
  int init_ORB (int argc, char *argv[]);
  // initializes the ORB.
  // Returns 0 on success, -1 on error.

  int init_RtEC (void);
  // initializes the Real Time EC.
  // Returns 0 on success, -1 on error.

  int init_CosEC (void);
  // initializes the COS EC.
  // Returns 0 on success, -1 on error.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  TAO_Reactive_Module_Factory module_factory_;
  // The Module_Factory required by the Rtec.

  ACE_Config_Scheduler scheduler_impl_;
  // Scheduler used by the Rtec.

  RtecScheduler::Scheduler_var scheduler_;
  // Reference to the Scheduler after activating it in the ORB.

  ACE_EventChannel *ec_impl_;
  // The servant object of the Rtec.

  RtecEventChannelAdmin::EventChannel_var rtec_;
  // Reference to the Rtec returned after activating it in the ORB.

  ACE_ConsumerQOS_Factory consumer_qos_;
  // The Consumer QOS.

  ACE_SupplierQOS_Factory supplier_qos_;
  // The Supplier QOS.

  TAO_CosEC_EventChannel_i ec_i_;
  // The servant object of the COS Event Channel.

  CosEventChannelAdmin::EventChannel_ptr cos_ec_;
  // Reference to the CosEC returned after activating it in the ORB.

  CosECConsumer consumer_;
  // The Cos Consumer that will receive the event.

  CosECSupplier supplier_;
  // The Cos Supplier that will supply the event.
};

#endif /* COSECBASIC_H */
