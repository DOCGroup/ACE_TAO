/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Notifier_Handler.h
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu) and
//    Pradeep Gore (pradeep@cs.wustl.edu)
//
// ============================================================================

#ifndef _NOTIFIER_HANDLER_H
#define _NOTIFIER_HANDLER_H

#include "Event_Comm_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

class Notifier_Handler
{
  // = TITLE
  //  Holds the <Event_Comm::Notifier> object which is obtained from
  //  the naming service.
public:
  // = Initialization and termination methods.
  Notifier_Handler (void);
  // This constructor gets the notifier object from the Naming
  // Service.

  virtual ~Notifier_Handler (void);
  // Destructor.

  int init (int argc, char *argv[], ShutdownCallback* _shutdowncallback);
  // Initialize the client communication endpoint with server.

  // = Accessors.
  Event_Comm::Notifier *notifier (void);
  void notifier (Event_Comm::Notifier *);

  int run (void);
  // runs the ORB.

  int close (void);
  // Close down the handler.

  void shutdown (void);
  // called to request application shutdown.

  ACE_Reactor *reactor (void);
  // returns the ORB's reactor.

private:
  CORBA::ORB_var orb_;
  // Remember our orb.

  Event_Comm::Notifier *notifier_;
  // Pointer to an <Event_Comm::Notifier> object.

 TAO_Naming_Client naming_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  ShutdownCallback *shutdowncallback;
  // The handler to shutdown the app.
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _NOTIFIER_HANDLER_H */
