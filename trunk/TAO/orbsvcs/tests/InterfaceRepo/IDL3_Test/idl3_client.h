// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/InterfaceRepo
//
// = FILENAME
//    idl3_client.h
//
// = DESCRIPTION
//    This class tests the IFR support for CCM-related types.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef IDL3_CLIENT_H
#define IDL3_CLIENT_H

#include "tao/IFR_Client/IFR_ComponentsC.h"

class IDL3_Client
{
  // = TITLE
  //     Component-aware IFR Client Implementation
  //
  // = DESCRIPTION
  //     Class wrapper for a client which tests the Interface Repository
  //     support for the OMG IDL3 extensions.
public:
  IDL3_Client (void);
  // Constructor

  ~IDL3_Client (void);
  // Destructor

  int init (int argc,
            char *argv[]
            ACE_ENV_ARG_DECL);
  // Initialize the ORB and get the IFR object reference.

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Execute test code.

private:
  int parse_args (int argc,
                  char *argv[]);
  // Process the command line arguments.

  int component_test (ACE_ENV_SINGLE_ARG_DECL);
  int home_test (ACE_ENV_SINGLE_ARG_DECL);

  // Also tests eventtype.
  int valuetype_test (const char *repo_id,
                      const char *prefix
                      ACE_ENV_ARG_DECL);

  int component_attribute_test (
      CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription_var &
      ACE_ENV_ARG_DECL
    );

  int component_inheritance_test (CORBA::ComponentIR::ComponentDef_var &
                                  ACE_ENV_ARG_DECL);

  int component_port_test (CORBA::ComponentIR::ComponentDef_var &
                           ACE_ENV_ARG_DECL);

  int provides_test (CORBA::ComponentIR::ProvidesDescriptionSeq &
                     ACE_ENV_ARG_DECL);

  int uses_test (CORBA::ComponentIR::UsesDescriptionSeq &
                 ACE_ENV_ARG_DECL);

  int event_port_test (CORBA::ComponentIR::EventPortDescriptionSeq &,
                       CORBA::ULong seq_length,
                       const char *port_type,
                       const char **names,
                       const char **ids
                       ACE_ENV_ARG_DECL);

  int valuetype_inheritance_test (CORBA::ExtValueDef_var &,
                                  const char *prefix
                                  ACE_ENV_ARG_DECL);

  int valuetype_attribute_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var &,
      const char *prefix
      ACE_ENV_ARG_DECL
    );

  int valuetype_operation_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var &,
      const char *prefix
      ACE_ENV_ARG_DECL
    );

  int valuetype_member_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var &,
      const char *prefix
      ACE_ENV_ARG_DECL
    );

  int valuetype_factory_test (
      CORBA::ExtValueDef::ExtFullValueDescription_var &,
      const char *prefix
      ACE_ENV_ARG_DECL
    );

  int home_inheritance_test (CORBA::ComponentIR::HomeDef_var &
                             ACE_ENV_ARG_DECL);

  int home_factory_test (CORBA::ComponentIR::HomeDescription *
                         ACE_ENV_ARG_DECL);

  int home_finder_test (CORBA::ComponentIR::HomeDescription *
                        ACE_ENV_ARG_DECL);

private:
  CORBA::Boolean debug_;
  // Flag to output detailed error messages.

  CORBA::ORB_var orb_;
  // Storage of the ORB reference.

  CORBA::Repository_var repo_;
  // Storage of the IFR reference.
};

#endif /* IDL3_CLIENT_H */
