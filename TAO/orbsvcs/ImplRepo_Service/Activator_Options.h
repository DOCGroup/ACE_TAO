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

#ifndef ACTIVATOR_OPTIONS_H
#define ACTIVATOR_OPTIONS_H

#include "Repository_Configuration.h"

#include "tao/ORB.h"
#include "tao/PortableServer/ImplRepoC.h"

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
 * This is where all the settings for TAO's Implementation Repository is
 * stored.
 */
class Options
{
public:
  enum REPO_MODE {
    REPO_NONE,
    REPO_XML_FILE,
    REPO_HEAP_FILE,
    REPO_REGISTRY
  };

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

  /// Returns the configuration  object.
  Repository_Configuration* config (void) const;

  int repository_mode (void);

  /// Returns the file name.
  ACE_CString file_name (void) const;

  /// Converts the activation mode to a const char *. Needed to put
  /// the activation mode into the XML file or print it out.
  const char *convert_str (ImplementationRepository::ActivationMode mode);

  ImplementationRepository::ActivationMode convert_mode (const char *mode);

  /// Returns the timeout value for program starting.
  ACE_Time_Value startup_timeout (void) const;

  /// Returns the timeout value for program starting.
  ACE_Time_Value ping_interval (void) const;

  /// Returns a pointer to the ORB.
  CORBA::ORB_ptr orb (void) const;

  /// Will we listen for multicast location requests?
  bool multicast (void) const;

  /// Do we allow modifications to the servers?
  bool readonly (void) const;

private:
  /// Parses and pulls out arguments for the ImR
  int parse_args (int &argc, char *argv[]);

  /// Print the usage information.
  void print_usage (void) const;

  /// Wrapper for the other initialize_persistence() methods
  int initialize_persistence(void);

  /// Initialize heap file persistence.
  int initialize_heap_persistence (void);

  /// Initialize Registry persistence.
  int initialize_registry_persistence (void);

  /// Initialize default heap for no persistence.
  int initialize_non_persistence (void);

  /// Initialize XML file persistence
  int initialize_xml_persistence (void);

  /// Run a service command.
  int run_service_command (const ACE_CString& cmdline);

  /// Loads ORB options from the registry
  int load_registry_options (char*& cmdline, ACE_ARGV& argv);
  int save_registry_options (const ACE_CString& cmdline);

  /// The persistent configuration object.
  auto_ptr<Repository_Configuration> repo_config_;

  /// Mode of the Server Repository: if XML (x) or non-XML (n)
  int repo_mode_;

  /// The persistent file option.
  ACE_CString file_name_;

  /// Debug level.
  unsigned int debug_;

  /// File where the IOR of the server object is stored.
  ACE_CString ior_output_file_;

  /// Will we listen for multicast location requests?
  bool multicast_;

  /// The ORB for the Implementation Repository.
  CORBA::ORB_var orb_;

  /// The amount of time between successive "are you started yet?" pings.
  ACE_Time_Value ping_interval_;

  /// Are we running as a service?
  bool service_;

  /// The amount of time to wait for a server to response after starting it.
  ACE_Time_Value startup_timeout_;

  /// Can the server_repository be modified?
  bool readonly_;

  /// SC_NONE, SC_INSTALL, SC_REMOVE, ...
  int service_command_;
};

typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

#endif
