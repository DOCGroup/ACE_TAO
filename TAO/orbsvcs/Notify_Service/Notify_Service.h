/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = FILENAME
//   Notify_Service.h
//
// = AUTHORS
//   Pradeep Gore <pradeep@cs.wustl.edu>
//   Service options code by Wei Chiang <Wei.Chiang@nokia.com>.
//
// = DESCRIPTION
//   Notification Service front end.
//
// ============================================================================

#ifndef NOTIFY_SERVICE_H
#define NOTIFY_SERVICE_H

#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Task.h"

#define NOTIFY_KEY "NotifyEventChannelFactory"
#define NOTIFY_CHANNEL_NAME "NotifyEventChannel"

class Worker : public ACE_Task_Base
{
  // = TITLE
  //   Run a server thread
  //
  // = DESCRIPTION
  //   Use the ACE_Task_Base class to run server threads
  //
public:
  Worker (void);
  // ctor

  void orb (CORBA::ORB_ptr orb);

  virtual int svc (void);
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

class TAO_Notify_Service
{
  // = TITLE
  //   Notify_Service
  //
  // = DESCRIPTION
  //   Implementation of the Notification Service front end.

 public:
  // = Initialization and termination methods.
  TAO_Notify_Service (void);
  // Constructor.

  virtual ~TAO_Notify_Service (void);
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

  int resolve_naming_service (ACE_ENV_SINGLE_ARG_DECL);
  // Resolve the naming service.

  int parse_args (int argc, char *argv []);
  // Parses the command line arguments.

  // = Data members

  int bootstrap_;
  // 1: this service is bootstrappable

  int use_name_svc_;
  // 1: register itself with the name service

  int register_ec_;
  // 1:

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  ACE_CString notify_factory_name_;
  // The Factory name.

  ACE_CString notify_channel_name_;
  // The Factory name.

  int register_event_channel_;
  // 1:  create an event channel and registers it with the Naming Service with
  //     the name <notify_channel_name_>

  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;
  // The Factory.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  PortableServer::POA_var poa_;
  // Reference to the root poa.

  CosNaming::NamingContextExt_var naming_;
  // A naming context.

  Worker worker_;
  // Worker for TP reactor mode.

  int nthreads_;
  // Number of worker threads.
};
#endif /* NOTIFY_SERVICE_H */
