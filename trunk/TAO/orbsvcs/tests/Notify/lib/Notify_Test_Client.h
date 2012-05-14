/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_Test_Client.h
 *
 *  $Id$
 *
 * Prototype Test Driver for all the Notify tests.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef NOTIFY_TEST_CLIENT_H
#define NOTIFY_TEST_CLIENT_H

#include "ace/Atomic_Op_T.h"
#include "ace/Thread_Mutex.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"
#include "notify_test_export.h"

class TAO_Notify_Tests_Peer;

/**
 * @class Notify_Test_Client
 *
 * @brief Notify_Test_Client
 *
 * Shows how consumers Notify_Test_Client for events.
 */
class TAO_NOTIFY_TEST_Export Notify_Test_Client
{

public:
  // = Initialization and Termination
  Notify_Test_Client (void);
  virtual ~Notify_Test_Client ();

  /// starts the orb and resolves the notify factory via a naming service.
  virtual int init (int argc, ACE_TCHAR *argv []);

  /// Allow the user to override this empty method
  virtual int parse_args (int argc, ACE_TCHAR* argv[]);

  /// Call ORB::run to accept requests.
  int ORB_run (void);
  int ORB_run (ACE_Time_Value& tv);

  /// How many clients will call consumer_done.
  void consumer_start (TAO_Notify_Tests_Peer*);

  /// Callback for clients to unregister themselves.
  void consumer_done (TAO_Notify_Tests_Peer*);

  /// Access the done boolean.
  bool is_done (void) const;

  /// Access the ORB.  This class retains ownership.
  CORBA::ORB_ptr orb (void);

  /// Access the Root POA.  This class retains ownership.
  PortableServer::POA_ptr root_poa (void);

  /// Access the Naming Context.  This class retains ownership.
  CosNaming::NamingContext_ptr naming_context (void);

  /// Access the Notify Factory.  This class retains ownership.
  CosNotifyChannelAdmin::EventChannelFactory_ptr notify_factory (void);

  /// Create an Event Channel.  Ownership is passed to the caller.
  CosNotifyChannelAdmin::EventChannel_ptr create_event_channel (
                                              const char* name,
                                              bool resolve);

protected:
  /// Initializes the ORB.
  int init_ORB (int argc,
                ACE_TCHAR *argv []);

  /// Try to get hold of a running naming service.
  void resolve_naming_service (void);

  /// Try to resolve the Notify factory from the Naming service.
  void resolve_Notify_factory (void);

  // = Data Members
  /// Reference to the root poa.
  PortableServer::POA_var root_poa_;

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// Handle to the name service.
  CosNaming::NamingContext_var naming_context_;

  /// Channel factory.
  CosNotifyChannelAdmin::EventChannelFactory_var notify_factory_;

  /// The group operator between admin-proxy's.
  CosNotifyChannelAdmin::InterFilterGroupOperator ifgop_;

  /// Initial qos specified to the factory when creating the EC.
  CosNotification::QoSProperties initial_qos_;

  /// Initial admin props specified to the factory when creating the EC.
  CosNotification::AdminProperties initial_admin_;

private:
  ACE_Atomic_Op< TAO_SYNCH_MUTEX, int > num_clients_;
  bool done_;
};

#endif /* NOTIFY_TEST_CLIENT_H */
