/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   RTEvent_Logging_Service.h
 *
 *  $Id$
 *
 *  Front End of the Telecom Log Service 
 *  RTEvent_Logging_Service
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef RTEVENT_LOGGING_SERVICE_H
#define RTEVENT_LOGGING_SERVICE_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecSchedulerS.h"

#include "orbsvcs/RTEventLogAdminS.h"

#include "orbsvcs/Log/RTEventLogFactory_i.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Module_Factory;

class RTEvent_Logging_Service 
{
  //
  // = TITLE
  //   
  //   
  //
  // = DESCRIPTION
  //   
  //   
  //   
  //   
  //
public:
  RTEvent_Logging_Service (void);
  virtual ~RTEvent_Logging_Service (void);

  int run (int argc, char* argv[]);
  // Run the event service.

private:
  int parse_args (int argc, char* argv[]);
  // parse the command line args

  enum {
    ES_NEW, // Use the Service Configurator to find factory
    ES_OLD_REACTIVE,   // Reactive dispatching, old EC.
    ES_OLD_MT          // Prioritized dispatching, old EC.
  };

private:
  TAO_Module_Factory *module_factory_;
  // The module factory for the EC.

  POA_RtecScheduler::Scheduler *sched_impl_;
  // The Scheduler implementation.

  const char* service_name_;
  // The name we use to bind with the NameService

  const char* ior_file_name_;
  // The name of the file were we output the Event_Service IOR.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  int event_service_type_;
  // The type of event service we will use

  int global_scheduler_;
  // Should we use a global scheduler or a local one?

  const char* rtevent_log_factory_name_;
  // The name of the factory registered with the naming service.

  const char* child_poa_name_;
  // The name of the Child POA.

  TAO_RTEventLogFactory_i *factory_servant_;
  // The factory servant.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  RTEventLogAdmin::EventLogFactory_var factory_;
  // The corba object after activation.

  TAO_Naming_Client naming_client_;

};

#endif /* RTEVENT_LOGGING_SERVICE_H */
