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

  /// Will we listen for multicast location requests?
  bool multicast (void) const;

  /// The nt service command to run (install/remove)
  SERVICE_COMMAND service_command(void) const;

  int save_registry_options();

  const ACE_CString& cmdline(void) const;

private:
  /// Parses and pulls out arguments for the ImR
  int parse_args (int &argc, char *argv[]);

  /// Print the usage information.
  void print_usage (void) const;

  /// Run a service command.
  int run_service_command (const ACE_CString& cmdline);

  int load_registry_options();
private:

  /// Debug level.
  unsigned int debug_;

  /// File where the IOR of the server object is stored.
  ACE_CString ior_output_file_;

  /// Will we listen for multicast location requests?
  bool multicast_;

  /// Are we running as a service?
  bool service_;

  /// SC_NONE, SC_INSTALL, SC_REMOVE, ...
  SERVICE_COMMAND service_command_;

  /// Our extra command line arguments
  ACE_CString cmdline_;
};

#endif 
