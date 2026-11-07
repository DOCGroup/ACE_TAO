/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Notifier_Handler.h
 *
 *  @author Douglas C. Schmidt (d.schmidt@vanderbilt.edu) and Pradeep Gore (pradeep@cs.wustl.edu)
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
  /// This constructor gets the notifier object from the Naming
  /// Service.
  Notifier_Handler () = default;

  /// Destructor.
  ~Notifier_Handler () = default;

  Notifier_Handler (Notifier_Handler const &) = delete;
  Notifier_Handler &operator= (Notifier_Handler const &) = delete;
  Notifier_Handler (Notifier_Handler &&) = delete;
  Notifier_Handler &operator= (Notifier_Handler &&) = delete;

  /// Initialize the client communication endpoint with server.
  int init (int argc, ACE_TCHAR *argv[], ShutdownCallback *shutdown_callback);

  // = Accessors.
  Event_Comm::Notifier *notifier () const;
  void notifier (Event_Comm::Notifier *);

  /// runs the ORB.
  int run ();

  /// Close down the handler.
  int close ();

  /// called to request application shutdown.
  void shutdown ();

  /// returns the ORB's reactor.
  ACE_Reactor *reactor () const;

private:
  /// Remember our orb.
  CORBA::ORB_var orb_ {};

  /// Pointer to an <Event_Comm::Notifier> object.
  Event_Comm::Notifier *notifier_ {};

  /// An instance of the name client used for resolving the factory
  /// objects.
 TAO_Naming_Client naming_client_;

  /// The handler to shutdown the app.
  ShutdownCallback *shutdowncallback_ {};
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _NOTIFIER_HANDLER_H */
