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

#ifndef OPTIONS_H
#define OPTIONS_H

#include "Repository_Configuration.h"

#include "tao/ORB.h"
#include "tao/PortableServer/ImplRepoC.h"

#include "ace/Singleton.h"


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
  /// Default Constructor
  Options ();

  /// Destructor
  ~Options();

  /// Parse the command-line arguments and initialize the options.
  int init (int argc, char *argv[]);

  /// Service Mode
  int service (void) const;

  /// Debug level for the Implementation Repository.
  unsigned int debug (void) const;

  /// Returns the file where the IOR should be stored.
  FILE *output_file (void) const;

  /// Returns the configuration  object.
  Repository_Configuration* config (void) const;

  char *repository_mode (void);

  /// Returns the file addr.
  char *file_name (void) const;

  /// Converts the activation mode to a const char *. Needed to put
  /// the activation mode into the XML file or print it out.
  const char *convert_str (ImplementationRepository::ActivationMode mode);

  ImplementationRepository::ActivationMode convert_mode (const char *mode);

  /// Returns the timeout value for program starting.
  const ACE_Time_Value &startup_timeout (void) const;

  /// Returns the timeout value for program starting.
  const ACE_Time_Value &ping_interval (void) const;

  /// Returns a pointer to the ORB.
  CORBA::ORB_ptr orb (void) const;

  /// Will we listen for multicast location requests?
  int multicast (void) const;

  /// Do we allow modifications to the servers?
  int readonly (void) const;

private:
  /// Parses and pulls out arguments for the ImR
  int parse_args (int &argc, char *argv[]);

  /// Print the usage information.
  void print_usage (void) const;

  /// Initialize file persistence.
  int initialize_file_persistence (const char  *filename);

  /// Initialize Registry persistence.
  int initialize_registry_persistence (void);

  /// Initialize default heap for no persistence.
  int initialize_non_persistence (void);

  /// Initialize XML file persistence
  int initialize_xml_persistence (const char *file_name);

  /// Run a service command.
  int run_service_command (const char *command);

  /// Loads ORB options from the registry
  int load_registry_options (ACE_ARGV &orb_options);

  /// The persistent configuration object.
  Repository_Configuration* repo_config_;

  /// Mode of the Server Repository: if XML (x) or non-XML (n)
  char repo_mode_;

  /// The persistent file option.
  char *file_name_;

  /// Debug level.
  unsigned int debug_;

  /// File where the IOR of the server object is stored.
  FILE *ior_output_file_;

  /// Will we listen for multicast location requests?
  int multicast_;

  /// The ORB for the Implementation Repository.
  CORBA::ORB_var orb_;

  /// The amount of time between successive "are you started yet?" pings.
  ACE_Time_Value ping_interval_;

  /// Are we running as a service?
  int service_;

  /// The amount of time to wait for a server to response after starting it.
  ACE_Time_Value startup_timeout_;

  /// Can the server_repository be modified?
  int readonly_;
};

typedef ACE_Singleton <Options, ACE_Null_Mutex> OPTIONS;

#endif /* OPTIONS_H */
