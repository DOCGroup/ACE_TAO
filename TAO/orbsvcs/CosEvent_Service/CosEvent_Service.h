/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   CosEvent_Service.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   The COS Event Channel service.
//
// ============================================================================

#ifndef COSEVENT_SERVICE_H
#define COSEVENT_SERVICE_H

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"
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

class CosEvent_Service
{
  // = TITLE
  //   CosEvent_Service
  //
  // = DESCRIPTION
  //   Implementation of the COS Event Service.

 public:
  // = Initialization and termination methods.
  CosEvent_Service (void);
  // Constructor.

  ~CosEvent_Service (void);
  // Destructor.

  int startup (int argc, char *argv[]);
  // Initializes the COS Event Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // run the COS Event Service.
  // Returns 0 on success, -1 on error.

  int shutdown (void);
  // Shutdown the COS Event Service.
  // Returns 0 on success, -1 on error.

 private:
  int init_ORB  (int argc, char *argv []);
  // initialize the ORB.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  int init_NamingService (void);
  // Initializes the Naming Client.
  // Returns 0 on success, -1 on error.

  int get_Rtec_viaNamingService (void);
  // Gets a reference to a Rtec via the  naming service.
  // Returns 0 on success, -1 on error.

  int start_Scheduler (void);
  // start the Scheduler used ny the RtEC.
  // Returns 0 on success, -1 on error.

  int create_local_RtecService (void);
  // Creates a local Rtec.
  // Returns 0 on success, -1 on error.

  int create_CosEC (void);
  // Creates a local CosEC.
  // Returns 0 on success, -1 on error.

  int register_CosEC (void);
  // Registers the Cos EC with the Naming Service.
  // Returns 0 on success, -1 on error.

  const char* service_name;
  // The name we use to register with the Naming Service.

  const char* rt_service_name;
  // The name of the Real Time Event Service.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  TAO_Naming_Client naming_client_;
  // An instance of the name client used for resolving the factory
  // objects.

 TAO_Reactive_Module_Factory module_factory_;
  // The Module_Factory required by the Rtec.

  ACE_Config_Scheduler scheduler_impl_;
  // Scheduler used by the Rtec.

  RtecScheduler::Scheduler_var scheduler_;
  // Reference to the Scheduler after activating it in the ORB.

  ACE_EventChannel ec_impl_;
  // The servant object of the Rtec.

  RtecEventChannelAdmin::EventChannel_var rtec_;
  // Reference to the Rtec returned after activating it in the ORB.

  ACE_ConsumerQOS_Factory consumer_qos_;
  // The Consumer QOS.

  ACE_SupplierQOS_Factory supplier_qos_;
  // The Supplier QOS.

  EventChannel_i ec_i_;
  // The servant object of the COS Event Channel.

  CosEventChannelAdmin::EventChannel_ptr cos_ec_;
  // Reference to the CosEC returned after activating it in the ORB.
};
#endif /* COSEVENT_SERVICE_H */
