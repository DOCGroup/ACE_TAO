// -*- C++ -*-

//=============================================================================
/**
 *  @file   Locator_Options.h
 *
 *  $Id$
 *
 *  @brief  Definition of the Options class for the Implementation Repository.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef LOCATOR_OPTIONS_H
#define LOCATOR_OPTIONS_H

#include "locator_export.h"

#include "ace/SString.h"
#include "ace/Time_Value.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class LiveCheck;

/**
 * @class Options
 *
 * @brief Maintains the global options.
 *
 * This is where the settings for TAO's Implementation Repository are stored.
 */
class Locator_Export Options
{
public:

  enum SERVICE_COMMAND {
    SC_NONE,
    SC_INSTALL,
    SC_REMOVE
  };

  Options ();

  /// Parse the command-line arguments and initialize the options.
  int init (int argc, ACE_TCHAR *argv[]);
  /// This version should only be used when run as an nt service.
  int init_from_registry();

  /// Service Mode
  bool service (void) const;

  /// Debug level for the Implementation Repository.
  unsigned int debug (void) const;

  /// Returns the file where the IOR should be stored.
  const ACE_TString& ior_filename (void) const;

  /// Will we listen for multicast location requests?
  bool multicast (void) const;

  /// The nt service command to run (install/remove)
  SERVICE_COMMAND service_command(void) const;

  int save_registry_options();

  const char* cmdline(void) const;

  /// File that contains the activator related information
  /// that the persistent locator has to save.
  const ACE_TString& persist_file_name(void) const;

  /// Do we allow modifications to the servers?
  bool readonly (void) const;

  /// Which type of repository is to be used?
  enum RepoMode {
    REPO_NONE,
    REPO_XML_FILE,
    REPO_SHARED_FILES,
    REPO_HEAP_FILE,
    REPO_REGISTRY
  };
  RepoMode repository_mode (void) const;

  /// Do we wish to clear out the repository
  bool repository_erase (void) const;

  /// Returns the timeout value for program starting.
  ACE_Time_Value startup_timeout (void) const;

  /// Servers may be started externally to the ImR but register with it
  /// so that clients may still be forwarded to it. Traditionally, such
  /// servers are not pinged by the ImR in the spirit that since its own
  /// means were used to activate the service, the ImR should let the client
  /// deal with the server no matter the consequences. However, the ImR is
  /// in a position to give more information to the client or lists of
  /// active servers, so enabling the ping_external_ option will override
  /// the assumption of liveness and actively ping, based on the existing
  /// rules, all registered servers.
  bool ping_external (void) const;

  /// If the server hasn't been verified for a while, then we'll
  /// ping it. Note : No timers are currently used. We simply ping()
  /// during indirect invocations, if this interval has elapsed.
  ACE_Time_Value ping_interval (void) const;

  /// When pinging, this is the timeout
  ACE_Time_Value ping_timeout (void) const;

  LiveCheck *pinger (void) const;
  void pinger (LiveCheck *);

  bool unregister_if_address_reused (void) const;

  bool lockout (void) const;

  bool throw_shutdown_exceptions (void) const;

  /// Indicate what type of ImR Locator this is.
  enum ImrType { BACKUP_IMR, PRIMARY_IMR, STANDALONE_IMR };
  ImrType imr_type(void) const;

private:
  /// Parses and pulls out arguments for the ImR
  int parse_args (int &argc, ACE_TCHAR *argv[]);

  /// Print the usage information.
  void print_usage (void) const;

  /// Run a service command.
  int run_service_command (const ACE_TString& cmdline);

  int load_registry_options();
private:

  /// xml, heap, or registry
  RepoMode repo_mode_;

  /// Do we clear out the repository on load
  bool erase_repo_;

  /// Debug level.
  unsigned int debug_;

  /// File where the IOR of the server object is stored.
  ACE_TString ior_output_file_;

  /// Will we listen for multicast location requests?
  bool multicast_;

  /// Are we running as a service?
  bool service_;

  /// Should the ImR ping servers not started using the ImR?
  bool ping_external_;

  /// The amount of time between successive "are you started yet?" pings.
  ACE_Time_Value ping_interval_;

  /// The amount of time to wait for a "are you started yet?" ping reply.
  ACE_Time_Value ping_timeout_;

  /// The amount of time to wait for a server to response after starting it.
  ACE_Time_Value startup_timeout_;

  /// Can the server_repository be modified?
  bool readonly_;

  /// SC_NONE, SC_INSTALL, SC_REMOVE, ...
  SERVICE_COMMAND service_command_;

  /// Our extra command line arguments
  ACE_CString cmdline_;

  /// The persistent XML file name.
  ACE_TString persist_file_name_;

  /// Should check the server address and remove previous server if
  /// the address is reused.
  bool unregister_if_address_reused_;

  /// If enabled, use a global start count and lock access to the server without
  /// user intervention. Otherwise start count is reset for every start request
  bool lockout_;

  /// The type of ImR Locator this is.
  ImrType imr_type_;

  /// Have the "shutdown_server" command forward any exceptions (such as transient)
  /// back to the caller. Default behavior is to not do that since doing so may break
  /// existing installations. There is no command line option for this yet, but I want
  /// to preserve the framework for future use.
  bool throw_shutdown_exceptions_;

  LiveCheck *pinger_;
};

#endif
