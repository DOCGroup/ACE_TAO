/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Consumer_Handler.h
 *
 *  $Id$
 *
 *  Subclass of Corba_Handler that sets up the Consumer handler
 *  for use with the ACE ACE_Reactor.
 *
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu) and Pradeep Gore (pradeep@cs.wustl.edu)
 */
//=============================================================================


#ifndef _CONSUMER_HANDLER_H
#define _CONSUMER_HANDLER_H

#include "Event_Comm_i.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/CosNamingC.h"

/**
 * @class Consumer_Handler
 *
 * @brief The Consumer_Handler class.
 *
 * Starts up the ORB, registers the <Event_Comm::Consumer>
 * servant with the orb and gets a reference to a <Notifier>
 * from the Naming Service.
 */
class Consumer_Handler
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Consumer_Handler (void);

  /// Destructor.
 virtual ~Consumer_Handler (void);

  /// Initializes the ORB, gets the Notifier reference from the Naming
  /// Service, and starts the servant for the Consumer object.
  int init (int argc, ACE_TCHAR *argv[], ShutdownCallback *_shutdowncallback);

  /// runs the ORB.
  int run (void);

  /// shutdown the orb.
  void close (void);

  /// called to shutdown the consumer application.
  void shutdown (void);

  // = Accessors
  Event_Comm::Consumer *receiver (void);
  Event_Comm::Notifier *notifier (void);

  /// returns the ORB's reactor.
  ACE_Reactor *reactor (void);

private:
  /// gets the notifier reference from the naming service.
  /// returns 0 on success, -1 on error.
  int get_notifier (void);

  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// The <Consumer> implementation.
  Consumer_i receiver_i_;

  /// Pointer to an IDL <Consumer> proxy object.
  Event_Comm::Consumer_var receiver_;

  /// Pointer to an IDL <Notifier> proxy object.
  Event_Comm::Notifier_var notifier_;

  /// An instance of the name client used for resolving the factory
  /// objects.
  TAO_Naming_Client naming_services_client_;

  /// The Shutdown callback used to shutdown the consumer application.
  ShutdownCallback* shutdowncallback;
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _CONSUMER_HANDLER_H */
