/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    IFR_Service_Utils.h
//
// = DESCRIPTION
//      Implement wrappers useful to IFR Service clients and servers.
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu>
//    Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
//    Priyanka Gontla <pgontla@doc.ece.uci.edu>
//
// ============================================================================

#ifndef TAO_IFR_SERVICE_UTILS_H
#define TAO_IFR_SERVICE_UTILS_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/PortableServerC.h"
#include "tao/ORB.h"
#include "tao/IFR_Client/IFR_BaseC.h"
#include "ifr_service_export.h"

#include "ace/Configuration.h"

// Forward decl;
class IFR_ServantLocator;
class TAO_IOR_Multicast;
class ACE_Configuration;
class TAO_Repository_i;
class TAO_IDLType_i;
class TAO_Container_i;
class TAO_Contained_i;

class TAO_IFRService_Export TAO_IFR_Server
{
public:
  TAO_IFR_Server (void);

  int init_with_orb (int argc,
                     ACE_TCHAR *argv [],
                     CORBA::ORB_ptr orb,
                     int use_multicast_server = 0);
  /// Initialize the IFR Service with the command line arguments and
  /// the ORB.

  int fini (void);
  /// Destroy the child POA created in <init_with_orb>.

  ~TAO_IFR_Server (void);
  /// Destructor.

protected:
  
  int create_poa (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  /// Two persistent POAs, one using a servant locator.

  int open_config (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  /// Open an ACE_Configuration of the appropriate type.

  int create_repository (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  /// Create and initialize the repository.

  int init_multicast_server (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  /// Enable the Interface Repository to answer multicast requests
  /// for its IOR.

  CORBA::ORB_var orb_;
  /// Reference to our ORB.

  PortableServer::POA_ptr root_poa_;
  /// Root POA reference.

  PortableServer::POA_ptr repo_poa_;
  /// The Repository's POA reference.

  TAO_IOR_Multicast *ior_multicast_;
  /// Event handler that responds to multicast requests.

  ACE_Configuration *config_;
  /// Database for the IFR.

  CORBA::String_var ifr_ior_;
  /// Interface Repository's IOR.
};

class TAO_IFR_Service_Utils
{
public:
  TAO_IFR_Service_Utils (void);

  ~TAO_IFR_Service_Utils (void);
  
  typedef int (*name_clash_checker)(const char *);

  static char *int_to_string (CORBA::ULong number);
  /// Convert the hex form of an 4-byte unsigned int to a string.

  /// The error-checking methods below all return void because
  /// they throw a spec-defined exception for every error case
  /// (specifically one of the BAD_PARAM minor versions).

  static void valid_container (CORBA::DefinitionKind container_kind,
                               CORBA::DefinitionKind contained_kind
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  /// Checks for illegal member type in given container type.

  static void pre_exist (const char *id,
                         name_clash_checker checker,
                         ACE_Configuration_Section_Key &key,
                         TAO_Repository_i *repo,
                         CORBA::DefinitionKind kind
                         ACE_ENV_ARG_DECL);
  /// Wrapper for id_exists() and name_exists().

  static void id_exists (const char *id,
                         TAO_Repository_i *repo
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  /// Checks for a global repository id clash.

  static void name_exists (name_clash_checker checker,
                           ACE_Configuration_Section_Key &key,
                           TAO_Repository_i *repo,
                           CORBA::DefinitionKind kind
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  /// Checks for a local name clash.

  static void check_subsection (name_clash_checker checker,
                                const char *sub_section,
                                ACE_Configuration *config,
                                ACE_Configuration_Section_Key &key
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  /// Used by interfaces and components to check for name clashes
  /// in the attributes, operations, or component ports subsections.

  static void valid_creation (CORBA::DefinitionKind container_kind,
                              CORBA::DefinitionKind contained_kind,
                              const char *id,
                              name_clash_checker checker,
                              ACE_Configuration_Section_Key &key,
                              TAO_Repository_i *repo
                              ACE_ENV_ARG_DECL);
  /// Top-level method which calls all the above error-checking methods.

  static ACE_TString create_common (
    CORBA::DefinitionKind container_kind,
    CORBA::DefinitionKind contained_kind,
    ACE_Configuration_Section_Key container_key,
    ACE_Configuration_Section_Key &new_key,
    TAO_Repository_i *repo,
    const char *id,
    const char *name,
    name_clash_checker checker,
    const char *version,
    const char *sub_section_name
    ACE_ENV_ARG_DECL
  );
  /// Code common to the creation of an entry for any non-anonymous type.
  
  static void set_initializers (const CORBA::InitializerSeq &initializers,
                                ACE_Configuration *config,
                                ACE_Configuration_Section_Key &key);
  /// Common to Container::create_value() and ValueDef::initializers().

  static void set_supported_interfaces (
      const CORBA::InterfaceDefSeq &supported_interfaces,
      ACE_Configuration *config,
      ACE_Configuration_Section_Key &key
    );
  /// Common to Container::create_value() and ValueDef::supported_interfaces(),
  /// Container::create_component() and Component::supported_interfaces().

  static char *reference_to_path (CORBA::IRObject_ptr obj);
  /// Converison from IR Object reference to key location path.

  static CORBA::DefinitionKind path_to_def_kind (ACE_TString &path,
                                                 TAO_Repository_i *repo);
  /// Conversion from key location path to CORBA::DefinitionKind.

  static CORBA::DefinitionKind reference_to_def_kind (
      CORBA::IRObject_ptr obj,
      TAO_Repository_i *repo
    );
  /// Conversion from IR Object reference to CORBA::DefinitionKind.

  static TAO_IDLType_i *path_to_idltype (ACE_TString &path,
                                         TAO_Repository_i *repo);
  /// Converion from key location path to IDLType.

  static TAO_Contained_i *path_to_contained (ACE_TString &path,
                                             TAO_Repository_i *repo);
  /// Converion from key location path to IDLType.

  static TAO_Container_i *path_to_container (ACE_TString &path,
                                             TAO_Repository_i *repo);
  /// Converion from key location path to IDLType.

  static CORBA::Object_ptr path_to_ir_object (ACE_TString &path,
                                              TAO_Repository_i *repo
                                              ACE_ENV_ARG_DECL);
  /// Conversion from key location path to IR Object reference.

  static CORBA::Object_ptr create_objref (CORBA::DefinitionKind def_kind,
                                          const char *obj_id,
                                          TAO_Repository_i *repo
                                          ACE_ENV_ARG_DECL);
  /// Can be called from the above method or directly from a derived class.

  static char *oid_to_string (PortableServer::ObjectId &oid);
  /// Copy and octet sequence to a string..

public:
  static ACE_Configuration_Section_Key tmp_key_;
  /// To hold a key created by one method for another method to use.
  /// Calls to this class will be made after an IFR lock (if any)
  /// has been acquired, so thread safety is not an issue with this
  /// static variable.
  
  static TAO_Repository_i *repo_;
  /// Needed by TAO_InterfaceDef_i::name_clash, probably can be used 
  /// all over the place now that it's here.
};

#include /**/ "ace/post.h"
#endif /* TAO_IFR_SERVICE_UTILS_H */
