/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Notifier_Handler.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu) and Pradeep Gore (pradeep@cs.wustl.edu)
 */
//=============================================================================


#ifndef _NOTIFIER_HANDLER_H
#define _NOTIFIER_HANDLER_H

#include "Event_Comm_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/CosNamingC.h"

/**
 * @class Notifier_Handler
 *
 * @brief Holds the <Event_Comm::Notifier> object which is obtained from
 * the naming service.
 */
class Notifier_Handler
{
public:
  // = Initialization and termination methods.
  /// This constructor gets the notifier object from the Naming
  /// Service.
  Notifier_Handler (void);

  /// Destructor.
  virtual ~Notifier_Handler (void);

  /// Initialize the client communication endpoint with server.
  int init (int argc, ACE_TCHAR *argv[], ShutdownCallback* _shutdowncallback);

  // = Accessors.
  Event_Comm::Notifier *notifier (void);
  void notifier (Event_Comm::Notifier *);

  /// runs the ORB.
  int run (void);

  /// Close down the handler.
  int close (void);

  /// called to request application shutdown.
  void shutdown (void);

  /// returns the ORB's reactor.
  ACE_Reactor *reactor (void);

private:
  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// Pointer to an <Event_Comm::Notifier> object.
  Event_Comm::Notifier *notifier_;

  /// An instance of the name client used for resolving the factory
  /// objects.
 TAO_Naming_Client naming_client_;

  /// The handler to shutdown the app.
  ShutdownCallback *shutdowncallback;
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _NOTIFIER_HANDLER_H */
