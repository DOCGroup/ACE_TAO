/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Consumer_Handler.h
//
// = DESCRIPTION
//    Subclass of Corba_Handler that sets up the Consumer handler
//    for use with the ACE ACE_Reactor.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu) and Pradeep Gore
//    (pradeep@cs.wustl.edu)
//
// ============================================================================

#ifndef _CONSUMER_HANDLER_H
#define _CONSUMER_HANDLER_H

#include "Event_Comm_i.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

class Consumer_Handler
{
  // = TITLE
  //    The Consumer_Handler class.
  //
  // = DESCRIPTION
  //     Starts up the ORB, registers the <Event_Comm::Consumer>
  //     servant with the orb and gets a reference to a <Notifier>
  //     from the Naming Service.
public:
  // = Initialization and termination methods.
  Consumer_Handler (void);
  // Constructor.

 virtual ~Consumer_Handler (void);
  // Destructor.

  int init (int argc, char *argv[], ShutdownCallback *_shutdowncallback);
  // Initializes the ORB, gets the Notifier reference from the Naming
  // Service, and starts the servant for the Consumer object.

  int run (void);
  // runs the ORB.

  void close (void);
  // shutdown the orb.

  void shutdown (void);
  // called to shutdown the consumer application.

  // = Accessors
  Event_Comm::Consumer *receiver (void);
  Event_Comm::Notifier *notifier (void);

  ACE_Reactor *reactor (void);
  // returns the ORB's reactor.

private:
  int get_notifier (void);
  // gets the notifier reference from the naming service.
  // returns 0 on success, -1 on error.

  CORBA::ORB_var orb_;
  // Remember our orb.

  Consumer_i receiver_i_;
  // The <Consumer> implementation.

  Event_Comm::Consumer_var receiver_;
  // Pointer to an IDL <Consumer> proxy object.

  Event_Comm::Notifier_var notifier_;
  // Pointer to an IDL <Notifier> proxy object.

  TAO_Naming_Client naming_services_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  ShutdownCallback* shutdowncallback;
  // The Shutdown callback used to shutdown the consumer application.
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _CONSUMER_HANDLER_H */
