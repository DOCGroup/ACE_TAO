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

#include "ace/CORBA_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Event_Comm_i.h"
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

  int init (int argc, char *argv[]);
  // Initialize the client communication endpoint with server.

  // = Accessors.
  Event_Comm::Notifier *notifier (void);
  void notifier (Event_Comm::Notifier *);

  virtual int handle_close (void);
  // Close down the handler.

private:
  Event_Comm::Notifier *notifier_;
  // Pointer to an <Event_Comm::Notifier> object.

 TAO_Naming_Client naming_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  CORBA::ORB_var orb_;
  // Remember our orb.
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _NOTIFIER_HANDLER_H */
