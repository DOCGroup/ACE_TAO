/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Service.h
 *
 *  $Id$
 *
 * Notification Service front end.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *  @author Service options code by Wei Chiang <Wei.Chiang@nokia.com>.
 */
//=============================================================================


#ifndef NOTIFY_SERVICE_H
#define NOTIFY_SERVICE_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "../orbsvcs/Notify/CosNotify_Initializer.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_Notify_Service;
TAO_END_VERSIONED_NAMESPACE_DECL

#define NOTIFY_KEY "NotifyEventChannelFactory"
#define NOTIFY_CHANNEL_NAME "NotifyEventChannel"

/**
 * @class Worker
 *
 * @brief Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  /// ctor
  Worker (void);

  void orb (CORBA::ORB_ptr orb);

  /// The thread entry point.
  virtual int svc (void);

private:
  /// The orb
  CORBA::ORB_var orb_;
};

/**
 * @class TAO_Notify_Service_Driver
 *
 * @brief Notify_Service
 *
 * Implementation of the Notification Service front end.
 */
class TAO_Notify_Service_Driver
{

 public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Notify_Service_Driver (void);

  /// Destructor.
  virtual ~TAO_Notify_Service_Driver (void);

  /// Initializes the Service.
  /// Returns 0 on success, -1 on error.
  int init (int argc, ACE_TCHAR *argv[]);

  /// run the Service.
  /// Returns 0 on success, -1 on error.
  int run (void);

  /// Shutdown the Service.
  /// Returns 0 on success, -1 on error.
  void shutdown (void);


  // CosNotifyChannelAdmin::EventChannelFactory_var obj;
  //
protected:
  /// initialize the ORB.
  /// initialize the dispatching ORB.
  int init_ORB (int& argc, ACE_TCHAR *argv []);
  int init_dispatching_ORB (int& argc, ACE_TCHAR *argv []);

  TAO_Notify_Service* notify_service_;

  /// Resolve the naming service.
  int resolve_naming_service (void);

  /// Parses the command line arguments.
  int parse_args (int& argc, ACE_TCHAR *argv []);

  // = Data members

  /// 1: this service is bootstrappable
  int bootstrap_;

  /// 1: register itself with the name service
  int use_name_svc_;

  /// 1:
  int register_ec_;

  /// File where the IOR of the server object is stored.
  FILE *ior_output_file_;

  /// The Factory name.
  ACE_CString notify_factory_name_;

  /// The Factory name.
  ACE_CString notify_channel_name_;

  /// 1:  create an event channel and registers it with the Naming Service with
  ///     the name <notify_channel_name_>
  int register_event_channel_;

  /// The Factory.
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// separate dispatching orb if needed.
  CORBA::ORB_var dispatching_orb_;

  /// Reference to the root poa.
  PortableServer::POA_var poa_;

  /// A naming context.
  CosNaming::NamingContextExt_var naming_;

  /// Worker for TP reactor mode.
  Worker worker_;

  /// Number of worker threads.
  int nthreads_;

  /// indicate that a separate ORB is used for dispatching events.
  bool separate_dispatching_orb_;
};

#include /**/ "ace/post.h"
#endif /* NOTIFY_SERVICE_H */
