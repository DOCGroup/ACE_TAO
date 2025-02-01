/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Notifier_Server.h
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef _NOTIFIER_SERVER_H
#define _NOTIFIER_SERVER_H

#include "Event_Comm_i.h"
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Client.h"

/**
 * @class Notifier_Server
 *
 * @brief The Notifier Server that handles CORBA operations from
 * Consumers and Suppliers.
 */
class Notifier_Server
{
public:
  /// Constructor.
  Notifier_Server ();

  /// Destructor.
  ~Notifier_Server ();

  /// Initialize the Server state.
 int init (int argc, ACE_TCHAR *argv[]);

  /// Run the orb.
  int run ();

  /// Close the naming service.
  int close ();

  /// Reactor accessor.
  ACE_Reactor *reactor();

private:
  /// Initialises the name server and registers the Notifier server
  /// object name with the name server.
  int init_naming_service ();

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// Servant for the Notifier interface.
  Notifier_i servant_;

  /// Naming context for the naming service.
  CosNaming::NamingContext_var naming_context_;

  /// helper class for getting access to Naming Service.
  TAO_Naming_Client naming_server_;
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _NOTIFIER_SERVER_H */
