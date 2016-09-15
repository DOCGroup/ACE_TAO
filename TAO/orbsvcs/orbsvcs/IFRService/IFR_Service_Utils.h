// -*- C++ -*-


//=============================================================================
/**
 *  @file    IFR_Service_Utils.h
 *
 *    Implement wrappers useful to IFR Service clients and servers.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu> Jaiganesh Balasubramanian <jai@doc.ece.uci.edu> Priyanka Gontla <pgontla@doc.ece.uci.edu>
 */
//=============================================================================


#ifndef TAO_IFR_SERVICE_UTILS_H
#define TAO_IFR_SERVICE_UTILS_H

#include /**/ "ace/pre.h"

#include "orbsvcs/IFRService/ifr_service_export.h"
#include "tao/IFR_Client/IFR_BasicC.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/ORB.h"

#include "ace/Configuration.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Configuration;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IOR_Multicast;
class TAO_Repository_i;
class TAO_IDLType_i;
class TAO_Container_i;
class TAO_Contained_i;

class TAO_IFRService_Export TAO_IFR_Server
{
public:
  TAO_IFR_Server (void);

  /// Initialize the IFR Service with the command line arguments and
  /// the ORB.
  int init_with_orb (int argc,
                     ACE_TCHAR *argv [],
                     CORBA::ORB_ptr orb,
                     int use_multicast_server = 0);

  /// Initialize with a provided poa to serve as the alternate root POA.
  int init_with_poa (int argc,
                     ACE_TCHAR *argv [],
                     CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr rp,
                     int use_multicast_server = 0);

  /// Destroy the child POA created in init_with_poa().
  int fini (void);

  /// Destructor.
  ~TAO_IFR_Server (void);

protected:

  /// Two persistent POAs, one using a servant locator.
  int create_poa (void);

  /// Open an ACE_Configuration of the appropriate type.
  int open_config (void);

  /// Create and initialize the repository.
  int create_repository (void);

  /// Enable the Interface Repository to answer multicast requests
  /// for its IOR.
  int init_multicast_server (void);

  /// Reference to our ORB.
  CORBA::ORB_var orb_;

  /// Root POA reference.
  PortableServer::POA_ptr root_poa_;

  /// The Repository's POA reference.
  PortableServer::POA_ptr repo_poa_;

  /// Event handler that responds to multicast requests.
  TAO_IOR_Multicast *ior_multicast_;

  /// Database for the IFR.
  ACE_Configuration *config_;

  /// Interface Repository's IOR.
  CORBA::String_var ifr_ior_;
};

class TAO_IFR_Service_Utils
{
public:
  TAO_IFR_Service_Utils (void);

  ~TAO_IFR_Service_Utils (void);

  typedef int (*name_clash_checker)(const char *);

  /// Convert the hex form of an 4-byte unsigned int to a string.
  static char *int_to_string (CORBA::ULong number);

  /// The error-checking methods below all return void because
  /// they throw a spec-defined exception for every error case
  /// (specifically one of the BAD_PARAM minor versions).

  /// Checks for illegal member type in given container type.
  static void valid_container (CORBA::DefinitionKind container_kind,
                               CORBA::DefinitionKind contained_kind);

  /// Wrapper for id_exists() and name_exists().
  static void pre_exist (const char *id,
                         name_clash_checker checker,
                         ACE_Configuration_Section_Key &key,
                         TAO_Repository_i *repo,
                         CORBA::DefinitionKind kind);

  /// Checks for a global repository id clash.
  static void id_exists (const char *id,
                         TAO_Repository_i *repo);

  /// Checks for a local name clash.
  static void name_exists (name_clash_checker checker,
                           ACE_Configuration_Section_Key &key,
                           TAO_Repository_i *repo,
                           CORBA::DefinitionKind kind);

  /// Used by interfaces and components to check for name clashes
  /// in the attributes, operations, or component ports subsections.
  static void check_subsection (name_clash_checker checker,
                                const char *sub_section,
                                ACE_Configuration *config,
                                ACE_Configuration_Section_Key &key);

  /// Top-level method which calls all the above error-checking methods.
  static void valid_creation (CORBA::DefinitionKind container_kind,
                              CORBA::DefinitionKind contained_kind,
                              const char *id,
                              name_clash_checker checker,
                              ACE_Configuration_Section_Key &key,
                              TAO_Repository_i *repo);

  /// Code common to the creation of an entry for any non-anonymous type.
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
    );

  /// Common to Container::create_value() and ValueDef::supported_interfaces(),
  /// Container::create_component() and Component::supported_interfaces().
  static void set_supported_interfaces (
      const CORBA::InterfaceDefSeq &supported_interfaces,
      ACE_Configuration *config,
      ACE_Configuration_Section_Key &key
    );

  /// Common code for operations, attributes, and valuetype initializers.
  static void set_exceptions (ACE_Configuration *config,
                              ACE_Configuration_Section_Key &key,
                              const char *sub_section,
                              const CORBA::ExceptionDefSeq &exceptions);

  /// Implemented here to avoid recursion in the servant classes.
  static CORBA::TypeCode_ptr gen_valuetype_tc_r (
      ACE_Configuration_Section_Key &,
      TAO_Repository_i *
    );

  /// Called from the above function.
  static void fill_valuemember_seq (CORBA::ValueMemberSeq &,
                                    ACE_Configuration_Section_Key &,
                                    TAO_Repository_i *);

  /// Converison from IR Object reference to key location path.
  static char *reference_to_path (CORBA::IRObject_ptr obj);

  /// Conversion from key location path to CORBA::DefinitionKind.
  static CORBA::DefinitionKind path_to_def_kind (ACE_TString &path,
                                                 TAO_Repository_i *repo);

  /// Conversion from IR Object reference to CORBA::DefinitionKind.
  static CORBA::DefinitionKind reference_to_def_kind (
      CORBA::IRObject_ptr obj,
      TAO_Repository_i *repo
    );

  /// Converion from key location path to IDLType.
  static TAO_IDLType_i *path_to_idltype (ACE_TString &path,
                                         TAO_Repository_i *repo);

  /// Converion from key location path to IDLType.
  static TAO_Contained_i *path_to_contained (ACE_TString &path,
                                             TAO_Repository_i *repo);

  /// Converion from key location path to IDLType.
  static TAO_Container_i *path_to_container (ACE_TString &path,
                                             TAO_Repository_i *repo);

  /// Conversion from key location path to IR Object reference.
  static CORBA::Object_ptr path_to_ir_object (ACE_TString &path,
                                              TAO_Repository_i *repo);

  /// Can be called from the above method or directly from a derived class.
  static CORBA::Object_ptr create_objref (CORBA::DefinitionKind def_kind,
                                          const char *obj_id,
                                          TAO_Repository_i *repo);

  /// Copy and octet sequence to a string..
  static char *oid_to_string (PortableServer::ObjectId &oid);

public:
  /// To hold a key created by one method for another method to use.
  /// Calls to this class will be made after an IFR lock (if any)
  /// has been acquired, so thread safety is not an issue with this
  /// static variable.
  static ACE_Configuration_Section_Key tmp_key_;

  /// Needed by TAO_InterfaceDef_i::name_clash, probably can be used
  /// all over the place now that it's here.
  static TAO_Repository_i *repo_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_IFR_SERVICE_UTILS_H */
