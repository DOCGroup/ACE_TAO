// -*- C++ -*-

//=============================================================================
/**
 *  @file    idl3_client.h
 *
 *  This class tests the IFR support for CCM-related types.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef IDL3_CLIENT_H
#define IDL3_CLIENT_H

#include "tao/IFR_Client/IFR_ComponentsC.h"

/**
 * @class IDL3_Client
 *
 * @brief Component-aware IFR Client Implementation
 *
 * Class wrapper for a client which tests the Interface Repository
 * support for the OMG IDL3 extensions.
 */
class IDL3_Client
{
public:
  /// Initialize the ORB and get the IFR object reference.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Execute test code.
  int run ();

private:
  /// Process the command line arguments.
  int parse_args (int argc,
                  ACE_TCHAR *argv[]);

  int component_test ();
  int home_test ();

  // Also tests eventtype.
  int valuetype_test (char const* repo_id,
                      char const* prefix);

  int component_attribute_test (
      CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var const&);

  int component_inheritance_test (CORBA::ComponentIR::ComponentDef_var const&);

  int component_port_test (CORBA::ComponentIR::ComponentDef_var const&);

  int provides_test (CORBA::ComponentIR::ProvidesDescriptionSeq const&);

  int uses_test (CORBA::ComponentIR::UsesDescriptionSeq const&);

  int event_port_test (CORBA::ComponentIR::EventPortDescriptionSeq const&,
                       CORBA::ULong seq_length,
                       char const* port_type,
                       char const* const* names,
                       char const* const* ids);

  int valuetype_inheritance_test (CORBA::ExtValueDef_var const&,
                                  char const* prefix);

  int valuetype_attribute_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var const&,
      char const* prefix);

  int valuetype_operation_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var const&,
      char const* prefix);

  int valuetype_member_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var const&,
      char const* prefix);

  int valuetype_factory_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var const&,
      char const* prefix);

  int home_inheritance_test (CORBA::ComponentIR::HomeDef_var const&);

  int home_factory_test (CORBA::ComponentIR::HomeDescription const*);

  int home_finder_test (CORBA::ComponentIR::HomeDescription const*);

private:
  /// Flag to output detailed error messages.
  bool debug_ {};

  /// Storage of the ORB reference.
  CORBA::ORB_var orb_ {};

  /// Storage of the IFR reference.
  CORBA::Repository_var repo_ {};
};

#endif /* IDL3_CLIENT_H */
