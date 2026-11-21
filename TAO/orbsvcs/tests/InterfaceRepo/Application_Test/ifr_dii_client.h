// -*- C++ -*-
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
  IFR_DII_Client () = default;

  IFR_DII_Client (IFR_DII_Client const&) = delete;
  IFR_DII_Client (IFR_DII_Client&&) = delete;
  IFR_DII_Client& operator= (IFR_DII_Client const&) = delete;
  IFR_DII_Client& operator= (IFR_DII_Client&&) = delete;

  /// Initialize the client.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the client.
  int run ();

private:
  /// Process the command line arguments.
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  /// Query the object reference to get its InterfaceDef in the IFR.
  int find_interface_def ();

  /// Look up the InterfaceDef by name in the IFR.
  int lookup_interface_def ();

  /// Find the desired operation in the interface definition.
  void get_operation_def ();

  /// Query the interface definition to get the info needed
  /// to construct a CORBA::Request.
  void create_dii_request ();

  /// Do the invocation and display the results.
  void invoke_and_display ();

  /// Reference to our ORB.
  CORBA::ORB_var orb_ {};

  /// Reference to the Interface Repository.
  CORBA::Repository_var repo_ {};

  /// Reference to the target object.
  CORBA::Object_var target_ {};

  /// Repository entry corresponding to target_.
  CORBA::InterfaceDef_var target_def_ {};

  /// Reference to the discovered operation.
  CORBA::OperationDef_var op_ {};

  /// Type of the return value, if any.
  CORBA::TypeCode_var result_ {};

  /// DII request holder.
  CORBA::Request_var req_ {};

  /// Things that we will be searching for in the repository.
  CORBA::String_var namespace_name {CORBA::string_dup ("warehouse")};
  CORBA::String_var interface_name {CORBA::string_dup ("inventory")};
  CORBA::String_var op_name {CORBA::string_dup ("getCDinfo")};

  /// Are we looking up info on the target object by querying the
  /// IFR directly with the target's name, or indirectly
  /// by calling _get_interface() on the target object?
  bool lookup_by_name_ {};

  /// Display results and debugging info?
  bool debug_ {};

  ACE_TCHAR const *ior_output_file_ {ACE_TEXT ("file://iorfile")};
};

#endif /* IFR_DII_CLIENT_H */
