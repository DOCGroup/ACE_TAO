// -*- C++ -*-
//=============================================================================
/**
 *  @file   ImR_Activator_i.h
 *
 *  $Id$
 *
 *  @brief  This class implements the Implementation Repository.
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef IMR_ACTIVATOR_I_H
#define IMR_ACTIVATOR_I_H

#include "Server_Repository.h"
#include "activator_export.h"

#include "orbsvcs/orbsvcs/IOR_Multicast.h"

#include "tao/PortableServer/ImplRepoS.h"

#include "ace/Process_Manager.h"

class Options;

/**
 * @class ImR_Activator_i
 *
 * @brief IMR Activator Interface.
 *
 * This class provides the interface for the various activities
 * that can be done by the ImR_Activator.
 *
 */
class Activator_Export ImR_Activator_i : public POA_ImplementationRepository::Administration
{
public:
  ImR_Activator_i (void);

  /// IOR_LookupTable_Callback method.  Will return an IOR
  virtual char *find_ior (const char* object_name  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // = Interface methods

  /// Starts up the server <server> if not already running.
  virtual void activate_server (
      const char *server
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((
          CORBA::SystemException,
          ImplementationRepository::NotFound,
          ImplementationRepository::CannotActivate
        ));

  /// Implementation of activate_server.  <check_startup> is a flag to check
  /// the activation mode before attempting to start it.
  virtual char *activate_server_with_startup (const char *server,
                                              int check_startup
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((
          CORBA::SystemException,
          ImplementationRepository::NotFound,
          ImplementationRepository::CannotActivate
        ));

  /// Adds the server to the repository and registers the startup information
  /// about the server <server>.
  virtual void register_server (
      const char *server,
      const ImplementationRepository::StartupOptions &options
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((
          CORBA::SystemException,
          ImplementationRepository::AlreadyRegistered,
          ImplementationRepository::NotFound
        ));

  /// Updates the startup information about the server <server>.
  virtual void reregister_server (
      const char *server,
      const ImplementationRepository::StartupOptions &options
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
    ImplementationRepository::AlreadyRegistered,
    ImplementationRepository::NotFound
    ));

  /// Removes the server <server> from the repository.
  virtual void remove_server (
      const char *server
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::NotFound));

  /// Attempts to gracefully shut down the server,
  virtual void shutdown_server (
      const char *server
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::NotFound));

  /// Called by the server to update transient information such as current
  /// partial_ior of the <server> and its ServerObject.
  virtual void server_is_running (
      const char *server,
      const char * partial_ior,
      ImplementationRepository::ServerObject_ptr server_object
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::NotFound));

  /// What the server should call before it shuts down.
  virtual void server_is_shutting_down (
      const char *server
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::NotFound));

  /// Returns the startup information for a server
  virtual void find (
      const char *server,
      ImplementationRepository::ServerInformation_out info
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException,
                       ImplementationRepository::NotFound));

  /// Used to access the list of servers registered.  May also return an
  /// iterator which can be used to access more than <how_many> of them.
  virtual void list (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      ImplementationRepository::ServerInformationIterator_out server_iterator
      ACE_ENV_ARG_DECL_WITH_DEFAULTS
    ) ACE_THROW_SPEC ((CORBA::SystemException));

  /// Initialize the Server state - parsing arguments and waiting.
  int init (Options& opts ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Cleans up any state created by init*.
  int fini (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Runs the orb.
  int run (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

private:

  int init_with_orb (CORBA::ORB_ptr orb, const Options& opts ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// This method starts the server process.
  void start_server_i (Server_Info& info ACE_ENV_ARG_DECL_WITH_DEFAULTS) 
    ACE_THROW_SPEC ((CORBA::SystemException,
          ImplementationRepository::NotFound,
    ImplementationRepository::CannotActivate));

  void auto_start_servers(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

private:
  ACE_Process_Manager process_mgr_;

  /// Repository containing information about each server.
  Server_Repository repository_;

  /// The Root POA for this ORB.
  PortableServer::POA_var root_poa_;

  /// Implementation Repository's POA.
  PortableServer::POA_var imr_poa_;

  /// We're given a token when registering with the locator, which
  /// we must use when unregistering.
  CORBA::Long registration_token_;

  CORBA::ORB_var orb_;

  unsigned int debug_;
  bool read_only_;
  ACE_Time_Value startup_timeout_;
};

#endif /* IMR_ACTIVATOR_I_H */
