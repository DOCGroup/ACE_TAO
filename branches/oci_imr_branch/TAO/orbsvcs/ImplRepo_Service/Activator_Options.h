// -*- C++ -*-

//=============================================================================
/**
 *  @file   Activator_Options.h
 *
 *  $Id$
 *
 *  @brief  Definition of the Options class for the Implementation Repository.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACTIVATOR_OPTIONS_H
#define ACTIVATOR_OPTIONS_H

#include "activator_export.h"

#include "ace/SString.h"

/**
 * @class Options
 *
 * @brief Maintains the global options.
 *
 * This is where all the settings for TAO's Implementation Repository are
 * stored.
 */
class Activator_Export Options
{
public:
  enum SERVICE_COMMAND {
    SC_NONE,
    SC_INSTALL,
    SC_REMOVE,
    SC_INSTALL_NO_LOCATOR
  };

  enum RepoMode {
    REPO_NONE,
    REPO_XML_FILE,
    REPO_HEAP_FILE,
    REPO_REGISTRY
  };

  /// Default Constructor
  Options ();

  /// Parse the command-line arguments and initialize the options.
  int init (int argc, char *argv[]);
  /// This version should only be used when run as an nt service.
  int init_from_registry();

  /// Service Mode
  bool service (void) const;

  /// Debug level for the Implementation Repository.
  unsigned int debug (void) const;

  /// Returns the file where the IOR should be stored.
  const ACE_CString& ior_filename (void) const;

  RepoMode repository_mode (void) const;

  /// Returns the file name.
  const ACE_CString& file_name (void) const;

  /// Returns the timeout value for program starting.
  ACE_Time_Value startup_timeout (void) const;

  /// Returns the timeout value for program starting.
  ACE_Time_Value ping_interval (void) const;

  /// Will we listen for multicast location requests?
  bool multicast (void) const;

  /// Do we allow modifications to the servers?
  bool readonly (void) const;

  /// The nt service command to run (install/remove)
  SERVICE_COMMAND service_command(void) const;

  /// Save the command line arguments as registry settings. (Windows only)
  int save_registry_options ();

  const ACE_CString& cmdline(void) const;

private:
  /// Parses and pulls out arguments for the ImR
  int parse_args (int &argc, char *argv[]);

  /// Print the usage information.
  void print_usage (void) const;

  /// Loads options from the registry
  int load_registry_options ();

private:

  /// Our extra command line arguments
  ACE_CString cmdline_;

  // xml, heap, or registry
  RepoMode repo_mode_;

  /// The persistent file option.
  ACE_CString persist_file_name_;

  /// Debug level.
  unsigned int debug_;

  /// File where the IOR of the server object is stored.
  ACE_CString ior_output_file_;

  /// Will we listen for multicast location requests?
  bool multicast_;

  /// The amount of time between successive "are you started yet?" pings.
  ACE_Time_Value ping_interval_;

  /// Should we run as a service?
  bool service_;

  /// The amount of time to wait for a server to response after starting it.
  ACE_Time_Value startup_timeout_;

  /// Can the server_repository be modified?
  bool readonly_;

  /// SC_NONE, SC_INSTALL, SC_REMOVE, ...
  SERVICE_COMMAND service_command_;
};

#endif
