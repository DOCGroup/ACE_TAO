/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Repository.h
 *
 *  $Id$
 *
 *  This class implements the Repository for the Implementation Repository.
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================



#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "tao/PortableServer/ImplRepoC.h"
#include "tao/corba.h"

#include "ace/Functor.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "ace/Configuration.h"

/**
 * @class Server_Info
 *
 * @brief Information about IMR registered servers.
 *
 * Contains all the necessary information about the server including
 * Information on how to start it up and where it is running.
 */
class Server_Info
{
public:
  // = Constructors

  /// Initialize the command_line and working_dir.
  Server_Info (const ACE_TString POA_name,
               const ACE_TString logical_server_name,
               const ACE_TString startup_command,
               const ImplementationRepository::EnvironmentList
                     environment_vars,
               const ACE_TString working_dir,
               const ImplementationRepository::ActivationMode activation);

  // = Destructors

  /// The only destructor there is.
  ~Server_Info ();

  /// Updates information that is relevant only when an instance
  /// of the server is running.
  void update_running_info (const ACE_TString location,
                            const ACE_TString server_object_ior);

  /// Returns startup information.
  void get_startup_info (ACE_TString &logical_server_name,
                         ACE_TString &startup_command,
                         ImplementationRepository::EnvironmentList
                            &environment_vars,
                         ACE_TString &working_dir,
                         ImplementationRepository::ActivationMode &activation);

  /// Returns information about a running instance.
  void get_running_info (ACE_TString &location,
                         ACE_TString &server_object_ior);

  // ActivationMode get_activation (void);
  // Get the activation mode.

  // @@ Does this belong here?
  //  int startup ();
  // Starts up the server based on the information.
  // Returns:  0  if successful
  //           -1 if there is no registration command (it has to be manually
  //              restarted)

  /// This is a flag to determine if the process has already been spawned
  /// and we are just waiting for it to start up.
  int starting_up_;

private:
  /// Which server process this poa is grouped in.
  ACE_TString logical_server_name_;

  /// The name of the POA.
  ACE_TString POA_name_;

  /// The command line startup command (program and arguments).
  ACE_TString startup_command_;

  /// Environment Variables.
  ImplementationRepository::EnvironmentList environment_vars_;

  /// The working directory.
  ACE_TString working_dir_;

  /// Current endpoint used by the server.
  ACE_TString location_;

  /// IOR of the server object in the server.
  ACE_TString server_object_ior_;

  /// The type of activation this supports.
  ImplementationRepository::ActivationMode activation_;

  // No copying allowed.
  void operator= (Server_Info &);
  Server_Info (Server_Info &);
};




/**
 * @class Server_Repository
 *
 * @brief Repository of Server_Infos.
 *
 * Handles the storage, updating, and startup of servers.
 */
class Server_Repository
{
public:
  /// Default Constructor
  Server_Repository ();

  /// Destructor
  ~Server_Repository ();

  typedef ACE_Hash_Map_Entry<ACE_TString,
                             Server_Info *> HASH_IMR_ENTRY;

  typedef ACE_Hash_Map_Manager_Ex<ACE_TString,
                                  Server_Info *,
                                  ACE_Hash<ACE_TString>,
                                  ACE_Equal_To<ACE_TString>,
                                  ACE_Null_Mutex> HASH_IMR_MAP;

  typedef ACE_Hash_Map_Iterator_Ex<ACE_TString,
                                   Server_Info *,
                                   ACE_Hash<ACE_TString>,
                                   ACE_Equal_To<ACE_TString>,
                                   ACE_Null_Mutex> HASH_IMR_ITER;

  /// Initializes the Server Repository
  int init ();

  /// Add a new server to the Repository
  int add (const ACE_TString POA_name,
           const ACE_TString logical_server_name,
           const ACE_TString startup_command,
           const ImplementationRepository::EnvironmentList
                 environment_vars,
           const ACE_TString working_dir,
           const ImplementationRepository::ActivationMode activation);

  /// Update the associated process information.
  int update (const ACE_TString POA_name,
              const ACE_TString location,
              const ACE_TString server_object_ior);

  /// Returns information related to startup.
  int get_startup_info (const ACE_TString POA_name,
                        ACE_TString &logical_server_name,
                        ACE_TString &startup_command,
                        ImplementationRepository::EnvironmentList
                           &environment_vars,
                        ACE_TString &working_dir,
                        ImplementationRepository::ActivationMode &activation);

  /// Returns information related to a running copy.
  int get_running_info (const ACE_TString POA_name,
                        ACE_TString &location,
                        ACE_TString &server_object_ior);

  /// Checks the starting_up_ variable in the Server_Info and
  /// returns the previous value or -1 if the POA_name wasn't found
  int starting_up (const ACE_TString POA_name, int new_value);

  /// Same as above but does not alter the value.
  int starting_up (const ACE_TString POA_name);

  /// Removes the server from the Repository.
  int remove (const ACE_TString POA_name);

  /// Returns a new iterator that travels over the repository.
  HASH_IMR_ITER *new_iterator ();

  /// Returns the number of entries in the repository.
  size_t get_repository_size ();

private:
  HASH_IMR_MAP repository_;
  ACE_Configuration_Section_Key servers_;
};


#endif /* REPOSITORY_H */
