// -*- C++ -*-
//=============================================================================
/**
 *  @file   ImplRepo_i.h
 *
 *  $Id$
 *
 *  @brief  This class implements the Implementation Repository.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef IMPLREPO_I_H
#define IMPLREPO_I_H

#include "Repository.h"
#include "orbsvcs/IOR_Multicast.h"
#include "tao/PortableServer/ImplRepoS.h"
#include "tao/IORTable/IORTable.h"
#include "ace/Process_Manager.h"

class ImR_Adapter_Activator;
class ImR_Forwarder;

/**
 * @class ImplRepo_i
 *
 * @brief Implementation Repository
 *
 * This provides the interface to Administer the Implementation
 * Repository.
 */
class ImplRepo_i : public POA_ImplementationRepository::Administration
{
public:
  // = Constructor and destructor
  ImplRepo_i (void);
  ~ImplRepo_i (void);

  /// IOR_LookupTable_Callback method.  Will return an IOR
  char *find_ior (const ACE_CString &object_name
                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, IORTable::NotFound));

  // = Interface methods

  /// Starts up the server <server> if not already running.
  virtual void activate_server (
      const char *server
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((
          CORBA::SystemException,
          ImplementationRepository::Administration::NotFound,
          ImplementationRepository::Administration::CannotActivate
        ));

  /// Adds the server to the repository and registers the startup information
  /// about the server <server>.
  virtual void register_server (
      const char *server,
      const ImplementationRepository::StartupOptions &options
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((
          CORBA::SystemException,
          ImplementationRepository::Administration::AlreadyRegistered
        ));

  /// Updates the startup information about the server <server>.
  virtual void reregister_server (
      const char *server,
      const ImplementationRepository::StartupOptions &options
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException));

  /// Removes the server <server> from the repository.
  virtual void remove_server (
      const char *server
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::Administration::NotFound));

  /// Attempts to gracefully shut down the server,
  virtual void shutdown_server (
      const char *server
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::Administration::NotFound));

  /// Called by the server to update transient information such as current
  /// location of the <server> and its ServerObject.
  virtual char *server_is_running (
      const char *server,
      const char *location,
      ImplementationRepository::ServerObject_ptr server_object
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::Administration::NotFound));

  /// What the server should call before it shuts down.
  virtual void server_is_shutting_down (
      const char *server
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::Administration::NotFound));

  /// Returns the startup information for a server
  virtual void find (
      const char *server,
      ImplementationRepository::ServerInformation_out info
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::Administration::NotFound));

  /// Used to access the list of servers registered.  May also return an
  /// iterator which can be used to access more than <how_many> of them.
  virtual void list (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      ImplementationRepository::ServerInformationIterator_out server_iterator
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException));

  /// Initialize the Server state - parsing arguments and waiting.
  int init (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Cleans up any state created by init ().
  int fini (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Runs the orb.
  int run (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

private:
  /// Implementation of activate_server.  <check_startup> is a flag to check
  /// the activation mode before attempting to start it.
  ACE_TString activate_server_i (
      const char *server,
      const int check_startup
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((
          CORBA::SystemException,
          ImplementationRepository::Administration::NotFound,
          ImplementationRepository::Administration::CannotActivate
        ));

  /// This method starts the server process.
  void start_server_i (
      const char *server
      TAO_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((
          CORBA::SystemException,
          ImplementationRepository::Administration::NotFound,
          ImplementationRepository::Administration::CannotActivate
        ));

  /// This method will continuously ping a server and either return when it
  /// responds to the ping or return -1 if it times out.
  int ready_check (const char *server)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound));

  /// Set up multicast handler, if multicast is enabled
  int setup_multicast (ACE_Reactor *reactor);

  /// The locator interface for the IORTable
  IORTable::Locator_var locator_;

  /// The Process Manager.
  ACE_Process_Manager process_mgr_;

  /// The class that handles the forwarding.
  ImR_Forwarder *forwarder_impl_;

  /// Used for the forwarding of any type of POA.
  ImR_Adapter_Activator *activator_;

  /// Repository containing information about each server.
  Server_Repository repository_;

  /// The Root POA for this ORB.
  PortableServer::POA_var root_poa_;

  /// Implementation Repository's POA.
  PortableServer::POA_var imr_poa_;

  /// Key of the obj ref of the server.
  char *server_key_;

  /// Copy of the filename for the server output file.
  char *server_input_file_;

  /// The ior_multicast event handler.
  TAO_IOR_Multicast *ior_multicast_;

  /// Implementation Repository's IOR.  Why do we store it here?  Multicast
  /// doesn't work otherwise.
  CORBA::String_var imr_ior_;

  /// Number of command line arguments.
  int argc_;

  /// The command line arguments.
  char **argv_;

  friend class ImR_Forwarder;
};

#endif /* IMPLREPO_I_H */
