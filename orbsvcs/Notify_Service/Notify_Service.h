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
#include "orbsvcs/Notify/CosNotify_Initializer.h"
#include "ace/SString.h"
#include "ace/Reactor.h"
#include "ace/Service_Object.h"
#include "Notify_Service_Export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_Notify_Service;
class TAO_Notify_Service_Driver;

#define NOTIFY_KEY "NotifyEventChannelFactory"
#define NOTIFY_CHANNEL_NAME "NotifyEventChannel"

class LoggingWorker : public ACE_Task_Base
{
  public:
    LoggingWorker (TAO_Notify_Service_Driver* ns);
    virtual int svc (void);
    void start ();
    void end ();
  private:
    ACE_Reactor logging_reactor_;
    TAO_Notify_Service_Driver* ns_;
    bool started_;
    long timer_id_;
};

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
class TAO_Notify_Service_Export TAO_Notify_Service_Driver : public ACE_Service_Object
{
 friend class LoggingWorker;

 public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Notify_Service_Driver (void);

  /// Destructor.
  virtual ~TAO_Notify_Service_Driver (void);

  /// Initializes the Service.
  /// Returns 0 on success, -1 on error.
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// run the Service.
  /// Returns 0 on success, -1 on error.
  int run (void);

  /// Shutdown the Service.
  /// Returns 0 on success, -1 on error.
  virtual int fini (void);

protected:
  /// initialize the ORB.
  int init_ORB (int& argc, ACE_TCHAR *argv []);

  /// initialize the dispatching ORB.
  int init_dispatching_ORB (int& argc, ACE_TCHAR *argv []);

  /// Apply a relative round-trip timeout to the ORB
  void apply_timeout (CORBA::ORB_ptr orb);

  TAO_Notify_Service* notify_service_;

  /// Resolve the naming service.
  int resolve_naming_service (void);

  /// Parses the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv []);

  // = Data members

  /// true: this service is bootstrappable
  bool bootstrap_;

  /// true: register itself with the name service
  bool use_name_svc_;

  /// File name where the IOR of the server object is stored.
  const ACE_TCHAR *ior_output_file_name_;

  /// The Factory name.
  ACE_CString notify_factory_name_;

  /// The event channel names.
  ACE_Unbounded_Set <ACE_CString> notify_channel_name_;

  /// true: create an event channel and registers it with the Naming Service
  /// with the name <notify_channel_name_>
  bool register_event_channel_;

  /// The Factory.
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// Separate dispatching orb if needed.
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

  /// The relative round-trip timeout
  suseconds_t timeout_;

  /// Logging interval
  ACE_Time_Value logging_interval_;

  /// Logging worker
  LoggingWorker logging_worker_;

  /// Shutdown the ORB?
  bool shutdown_orb_;

  /// Shutdown the dispatching ORB?
  bool shutdown_dispatching_orb_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_Notify_Service_Driver)
ACE_FACTORY_DECLARE (TAO_Notify_Service, TAO_Notify_Service_Driver)

#include /**/ "ace/post.h"
#endif /* NOTIFY_SERVICE_H */
