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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Options
 *
 * This is where all the settings for TAO's Implementation Repository are
 * stored.
 */
class Activator_Export Activator_Options
{
public:

  enum SERVICE_COMMAND {
    SC_NONE,
    SC_INSTALL,
    SC_REMOVE,
    SC_INSTALL_NO_LOCATOR
  };

  // This default is based on the value from ACE_Process_Options
  // however, the real value is protected -- CAE 4/16/2007
  enum ACTIVATOR_PROCESS
  {
    ENVIRONMENT_BUFFER = 16 * 1024,
    ENVIRONMENT_MAX_VARS = 512
  };

  Activator_Options ();

  /// Parse the command-line arguments and initialize the options.
  int init (int argc, ACE_TCHAR *argv[]);
  /// This version should only be used when run as an nt service.
  int init_from_registry();

  /// Service Mode
  bool service (void) const;

  /// Notify the ImR when server processes die.
  /// Note : Currently this only works on Unix.
  bool notify_imr (void) const;

  /// When notifying of child death, pause this number of milliseconds
  /// to simulate a heavily loaded server.
  unsigned int induce_delay (void) const;

  /// Debug level for the Implementation Repository.
  unsigned int debug (void) const;

  /// Returns the file where the IOR should be stored.
  const ACE_TString& ior_filename (void) const;

  /// The nt service command to run (install/remove)
  SERVICE_COMMAND service_command(void) const;

  /// Save the command line arguments as registry settings. (Windows only)
  int save_registry_options ();

  const char* cmdline(void) const;

  const ACE_CString& name(void) const;

  int env_buf_len (void) const;

  int max_env_vars (void) const;

private:
  /// Parses and pulls out arguments for the ImR
  int parse_args (int &argc, ACE_TCHAR *argv[]);

  /// Print the usage information.
  void print_usage (void) const;

  /// Loads options from the registry
  int load_registry_options ();

private:

  /// Our extra command line arguments
  ACE_CString cmdline_;

  /// Debug level.
  unsigned int debug_;

  /// File where the IOR of the server object is stored.
  ACE_TString ior_output_file_;

  /// Should we run as a service?
  bool service_;

  bool notify_imr_;

  unsigned int induce_delay_;

  /// SC_NONE, SC_INSTALL, SC_REMOVE, ...
  SERVICE_COMMAND service_command_;

  ACE_CString name_;

  /// The default environment buffer length
  int env_buf_len_;

  /// Maximum number of environment variables
  int max_env_vars_;
};

#endif
