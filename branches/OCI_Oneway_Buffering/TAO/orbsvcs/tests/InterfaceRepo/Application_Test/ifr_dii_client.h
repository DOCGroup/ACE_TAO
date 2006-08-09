// -*- C++ -*-
// $Id$

#ifndef IFR_DII_CLIENT_H
#define IFR_DII_CLIENT_H

#include "tao/DynamicInterface/Request.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IFR_Client/IFR_BasicC.h"

class IFR_DII_Client
{
public:
  IFR_DII_Client (void);
  ~IFR_DII_Client (void);

  int init (int argc,
            char *argv[]
            ACE_ENV_ARG_DECL);
  // Initialize the client.

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Run the client.

private:
  int parse_args (int argc,
                  char *argv[]);
  // Process the command line arguments.

  int find_interface_def (ACE_ENV_SINGLE_ARG_DECL);
  // Query the object reference to get its InterfaceDef in the IFR.

  int lookup_interface_def (ACE_ENV_SINGLE_ARG_DECL);
  // Look up the InterfaceDef by name in the IFR.

  void get_operation_def (ACE_ENV_SINGLE_ARG_DECL);
  // Find the desired operation in the interface definition.

  void create_dii_request (ACE_ENV_SINGLE_ARG_DECL);
  // Query the interface definition to get the info needed
  // to construct a CORBA::Request.

  void invoke_and_display (ACE_ENV_SINGLE_ARG_DECL);
  // Do the invocation and display the results.

  CORBA::ORB_var orb_;
  // Reference to our ORB.

  CORBA::Repository_var repo_;
  // Reference to the Interface Repository.

  CORBA::Object_var target_;
  // Reference to the target object.

  CORBA::InterfaceDef_var target_def_;
  // Repository entry corresponding to target_.

  CORBA::OperationDef_var op_;
  // Reference to the discovered operation.

  CORBA::TypeCode_var result_;
  // Type of the return value, if any.

  CORBA::Request_var req_;
  // DII request holder.

  CORBA::String_var namespace_name;
  CORBA::String_var interface_name;
  CORBA::String_var op_name;
  // Things that we will be searching for in the repository.

  bool lookup_by_name_;
  // Are we looking up info on the target object by querying the
  // IFR directly with the target's name, or indirectly
  // by calling _get_interface() on the target object?
  
  bool debug_;
  // Display results and debugging info?
};

#endif /* IFR_DII_CLIENT_H */
