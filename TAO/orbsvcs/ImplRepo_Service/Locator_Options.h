// -*- C++ -*-

//=============================================================================
/**
 *  @file   Options.h
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

#include "Repository_Configuration.h"

#include "tao/ORB.h"

#include "ace/Singleton.h"
#include "ace/SString.h"
#include "ace/Auto_Ptr.h"


// Forward declarations
class ACE_ARGV;

/**
 * @class Options
 *
 * @brief Maintains the global options.
 *
 * This is where the settings for TAO's Implementation Repository are stored.
 */
class Options
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

  /// Service Mode
  bool service (void) const;

  /// Debug level for the Implementation Repository.
  unsigned int debug (void) const;

  /// Returns the file where the IOR should be stored.
  ACE_CString output_filename (void) const;

  /// Returns a pointer to the ORB.
  CORBA::ORB_ptr orb (void) const;

  /// Will we listen for multicast location requests?
  bool multicast (void) const;

private:
  /// Parses and pulls out arguments for the ImR
  int parse_args (int &argc, char *argv[]);

  /// Print the usage information.
  void print_usage (void) const;

  /// Run a service command.
  int run_service_command (const ACE_CString& cmdline);

  /// Loads ORB options from the registry
  int load_registry_options (char*& cmdline, ACE_ARGV& argv);
  int save_registry_options (const ACE_CString& cmdline);

  /// Debug level.
  unsigned int debug_;

  /// File where the IOR of the server object is stored.
  ACE_CString ior_output_file_;

  /// Will we listen for multicast location requests?
  bool multicast_;

  /// The ORB for the Implementation Repository.
  CORBA::ORB_var orb_;

  /// Are we running as a service?
  bool service_;

  /// SC_NONE, SC_INSTALL, SC_REMOVE, ...
  int service_command_;
};

typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

#endif 
