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
#include "orbsvcs/Event/EC_Basic_Factory.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

class TAO_EC_Factory;

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

  virtual int shutdown (void);
  // Shutdown the COS Event Service.
  // Returns 0 on success, -1 on error.

protected:
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

  virtual int create_local_RtecService (void);
  // Creates a local Rtec.
  // Returns 0 on success, -1 on error.

  int start_Scheduler (void);
  // start the Scheduler used ny the RtEC.
  // Returns 0 on success, -1 on error.

  void init_SupplierQOS (RtecScheduler::handle_t supp_handle);
  // Initialize the SupplierQOS Factory.

  void init_ConsumerQOS (RtecScheduler::handle_t cons_handle);
  // Initialize the ConsumerQOS Factory.

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

  const char* schedule_name_;
  // The name of the scheduler service.

  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  TAO_Naming_Client naming_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  TAO_EC_Factory *factory_;
  // The factory for the new EC.

  ACE_Config_Scheduler scheduler_impl_;
  // Scheduler used by the Rtec.

  RtecScheduler::Scheduler_var scheduler_;
  // Reference to the Scheduler after activating it in the ORB.

  POA_RtecEventChannelAdmin::EventChannel *ec_impl_;
  // The Event Channel implementation.

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

  int global_scheduler_;
  // Flag to indicate if the scheduler is local/global,
  // 0 => local, 1 => global, default is local.

  int remote_Rtec_;
  // Flag to indicate if the RtEC is local/remote,
  // 0 => local, 1 => remote, default is remote.

  char* eventTypeIds_;
  // The list of EventTypeIDs (for ConsumerQOS) seperated by spaces. e.g. "1 2 3 4"

  char* eventSourceIds_;
  // The list of EventSourceIDs (for ConsumerQOS) seperated by spaces. e.g. "1 2 3 4"

  char *source_type_pairs_;
  // The pairs of Source and EventType Ids (for the SupplierQOS).
};

class OLD_CosEvent_Service : public CosEvent_Service
{
  // = TITLE
  //    class  OLD_CosEvent_Service
  // = DESCRIPTION
  //   class to keep the old rtEC implementation around.
public:
  // = Initialization and termination methods.
  OLD_CosEvent_Service (void);
  ~OLD_CosEvent_Service (void);

  virtual int create_local_RtecService (void);
  // Creates a local Rtec.
  // Returns 0 on success, -1 on error.

  virtual int shutdown (void);
  // Shutdown the COS Event Service.
  // Returns 0 on success, -1 on error.

protected:
 TAO_Module_Factory *module_factory_;
  // The module factory for the EC.
};

#endif /* COSEVENT_SERVICE_H */
