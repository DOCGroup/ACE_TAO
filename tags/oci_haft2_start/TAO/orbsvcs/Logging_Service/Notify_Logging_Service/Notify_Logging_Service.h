/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Notify_Logging_Service.h
 *
 *  $Id$
 *
 *  Front End of the Telecom Log Service
 *  Notify_Logging_Service
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef NOTIFY_LOGGING_SERVICE_H
#define NOTIFY_LOGGING_SERVICE_H

#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNamingC.h"
#include "../../orbsvcs/Notify/Service.h"
#include "orbsvcs/DsLogAdminC.h"
#include "orbsvcs/DsNotifyLogAdminC.h"
#include "orbsvcs/Log/NotifyLogFactory_i.h"

#define NOTIFY_KEY "NotifyLogFactory"

class Notify_Logging_Service
{
  // = TITLE
  //   Notify_Service
  //
  // = DESCRIPTION
  //   Implementation of the Notification Service front end.

 public:
  // = Initialization and termination methods.
  Notify_Logging_Service (void);
  // Constructor.

  virtual ~Notify_Logging_Service (void);
  // Destructor.

  int init (int argc, char *argv[]
            ACE_ENV_ARG_DECL);
  // Initializes the Service.
  // Returns 0 on success, -1 on error.

  int run (void);
  // run the Service.
  // Returns 0 on success, -1 on error.

  void shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  // Shutdown the Service.
  // Returns 0 on success, -1 on error.

  // CosNotifyChannelAdmin::EventChannelFactory_var obj;
  //
protected:
  int init_ORB (int& argc, char *argv []
                ACE_ENV_ARG_DECL);
  // initialize the ORB.

  TAO_Notify_Service* notify_service_;

  void resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Resolve the naming service.

  // = Data members

  ACE_CString notify_factory_name_;
  // The Factory name.

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
  // The Factory.

  TAO_NotifyLogFactory_i *notify_log_factory_;
  // The Notify Log Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContextExt_var naming_;
  // A naming context.

};
#endif /* NOTIFY_SERVICE_H */
