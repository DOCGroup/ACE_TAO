/* -*- C++ -*- */
// $Id$
// ===========================================================
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Notifier_Input_Handler.h
//
// = DESCRIPTION
//    Definition of the Callback_Quoter Notifier_Input_Handler class.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef SUPPLIER_INPUT_HANDLER_H
#define SUPPLIER_INPUT_HANDLER_H

#include "Notifier_i.h"
#include "NotifierS.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class Notifier_Input_Handler : public ACE_Event_Handler
{
  // = TITLE
  //   The class defines the callback quoter Notifier initialization
  //   and run methods. 
  //
  // = DESCRIPTION
  //   This class handles initialization tasks, as well, such as
  //   setting up the Orb manager and registers the Notifier servant
  //   object. 
public:
  // = Initialization and termination methods.
  Notifier_Input_Handler (void);
  // Constructor.

  ~Notifier_Input_Handler (void);
  // Destructor.

  int init (int argc,
            char *argv[],
            CORBA::Environment &env);
  // Initialize the Notifier who plays the role of the server here.

  int run (CORBA::Environment &env);
  // Run the ORB.

   virtual int handle_input (ACE_HANDLE);
  // Handle the user input.

private:
  int parse_args (void);
  // Parses the command line arguments.

  int init_naming_service (CORBA::Environment &env);
  // Initialises the name server and registers the Notifier object
  // name with the name server.

  FILE *ior_output_file_;
  // File where the IOR of the Notifier object is stored.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the naming service.

  TAO_Naming_Server naming_server_;
  // An instance of the name server, wherein the naming context
  // containg the factory of objects will be registered.

  TAO_ORB_Manager orb_manager_;
  // The tao orb manager object.

  Notifier_i notifier_i_;
  // The servant object registered with the orb.

  int using_naming_service_;
  // This specifies whether the naming service is to be used.
};

#endif /* NOTIFIER_INPUT_HANDLER_H */
