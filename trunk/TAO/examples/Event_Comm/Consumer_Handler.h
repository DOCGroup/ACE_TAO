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
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
//
// ============================================================================

#if !defined (_CONSUMER_HANDLER_H)
#define _CONSUMER_HANDLER_H

//#include "ace/CORBA_Handler.h"
#include "Event_Comm_i.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

class Consumer_Handler
{
  // = TITLE
  //    The Consumer_Handler class.
  //
  // = DESCRIPTION
  //     Starts up the ORB, registers the <Event_Comm::Consumer> servant with the orb
  //      and gets a reference to a <Notifier> from the Naming Service.
  //

public:
  // = Initialization and termination methods.
  Consumer_Handler (void);
 ~Consumer_Handler (void);

  int init (int argc, char *argv[]);
  // Initializes the ORB,
  // gets the Notifier reference from the Naming Service,
  // and starts the servant for the Consumer object.

  int run (void);
  // runs the ORB.

  void close (void);
  // shutdown the orb.

  // = Accessors
  Event_Comm::Consumer *receiver (void);
  Event_Comm::Notifier *notifier (void);

  ACE_Reactor *reactor (void);

  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
			    ACE_Reactor_Mask = ACE_Event_Handler::NULL_MASK);
  // Close down the handler.

private:
  int get_notifier(void);

  Consumer_i receiver_i_;
  // The <Consumer> implementation.

  Event_Comm::Consumer_var receiver_;
  // Pointer to an IDL <Consumer> proxy object.

  Event_Comm::Notifier_var notifier_;
  // Pointer to an IDL <Notifier> proxy object.

  TAO_Naming_Client naming_services_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  CORBA::ORB_var orb_;
  // Remember our orb.
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _CONSUMER_HANDLER_H */
