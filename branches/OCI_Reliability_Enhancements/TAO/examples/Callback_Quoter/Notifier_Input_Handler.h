/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    Notifier_Input_Handler.h
 *
 *  $Id$
 *
 *  Definition of the Callback_Quoter Notifier_Input_Handler class.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef SUPPLIER_INPUT_HANDLER_H
#define SUPPLIER_INPUT_HANDLER_H

#include "Notifier_i.h"
#include "NotifierS.h"
#include "ace/Event_Handler.h"
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Client.h"

/**
 * @class Notifier_Input_Handler
 *
 * @brief The class defines the callback quoter Notifier initialization
 * and run methods.
 *
 * This class handles initialization tasks, as well, such as
 * setting up the Orb manager and registers the Notifier servant
 * object.
 */
class Notifier_Input_Handler : public ACE_Event_Handler
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Notifier_Input_Handler (void);

  /// Destructor.
  ~Notifier_Input_Handler (void);

  /// Initialize the Notifier who plays the role of the server here.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the ORB.
  int run (void);

  /// Handle the user input.
   virtual int handle_input (ACE_HANDLE);

private:

  /// The tao orb manager object.
  TAO_ORB_Manager orb_manager_;

  /// Parses the command line arguments.
  int parse_args (void);

  /// Initialises the name server and registers the Notifier object
  /// name with the name server.
  int init_naming_service (void);

  /// File where the IOR of the Notifier object is stored.
  FILE *ior_output_file_;

  /// Number of command line arguments.
  int argc_;

  /// The command line arguments.
  ACE_TCHAR **argv_;

  /// Naming context for the naming service.
  CosNaming::NamingContext_var naming_context_;

  /// helper class for getting access to Naming Service.
  TAO_Naming_Client naming_server_;

  /// The servant object registered with the orb.
  Notifier_i notifier_i_;

  /// This specifies whether the naming service is to be used.
  int using_naming_service_;
};

#endif /* NOTIFIER_INPUT_HANDLER_H */
