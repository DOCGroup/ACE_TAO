// -*- C++ -*-
// $Id$

#ifndef IFR_DII_CLIENT_H
#define IFR_DII_CLIENT_H

#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/NVList.h"
#include "tao/IFR_Client/InterfaceC.h"
#include "tao/DynamicInterface/Request.h"

class IFR_DII_Client
{
public:
  IFR_DII_Client (void);
  ~IFR_DII_Client (void);

  int init (int argc,
            char *argv[],
            CORBA::Environment &ACE_TRY_ENV);
  // Initialize the client.

  int run (CORBA::Environment &ACE_TRY_ENV);
  // Run the client.

private:
  void find_interface_def (CORBA::Environment &ACE_TRY_ENV);
  // Search the repository to find the desired interface
  // definition.

  void get_operation_def (CORBA::Environment &ACE_TRY_ENV);
  // Find the desired operation in the interface definition.

  void create_dii_request (CORBA::Environment &ACE_TRY_ENV);
  // Query the interface definition to get the info needed
  // to construct a CORBA::Request.

  void invoke_and_display (CORBA::Environment &ACE_TRY_ENV);
  // Do the invocation and display the results.

  CORBA::ORB_var orb_;
  // Reference to our ORB.

  IR_Repository_var repo_;
  // Reference to the Interface Repository.

  CORBA::Object_var target_;
  // Reference to the target of the DII request.

  IR_InterfaceDef_var target_def_;
  // Repository entry corresponding to target_.

  IR_OperationDef_var op_;
  // Reference to the discovered operation.

  CORBA::TypeCode_var result_;
  // Type of the return value, if any.

  CORBA::Request_var req_;
  // DII request holder.

  CORBA::String_var namespace_name;
  CORBA::String_var interface_name;
  CORBA::String_var op_name;
  // Things that we will be searching for in the repository.
};

#endif /* IFR_DII_CLIENT_H */

