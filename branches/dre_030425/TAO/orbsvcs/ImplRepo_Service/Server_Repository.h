/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Server_Repository.h
 *
 *  $Id$
 *
 *  This class implements the Repository for the Implementation Repository.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "Server_Info.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Configuration.h"


class XML_ContentHandler;


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
  Server_Repository (void);

  /// Destructor
  ~Server_Repository (void);

  typedef ACE_Hash_Map_Entry<ACE_CString,
                             Server_Info *> HASH_IMR_ENTRY;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  Server_Info *,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> HASH_IMR_MAP;

  typedef ACE_Hash_Map_Iterator_Ex<ACE_CString,
                                   Server_Info *,
                                   ACE_Hash<ACE_CString>,
                                   ACE_Equal_To<ACE_CString>,
                                   ACE_Null_Mutex> HASH_IMR_ITER;

  /// Initializes the Server Repository
  int init (void);

  /// Add a new server to the Repository
  int add (const ACE_CString POA_name,
           const ACE_CString logical_server_name,
           const ACE_CString startup_command,
           const ImplementationRepository::EnvironmentList
                 environment_vars,
           const ACE_CString working_dir,
           const ImplementationRepository::ActivationMode activation);

  /// Update the associated process information.
  int update (const ACE_CString POA_name,
              const ACE_CString location,
              const ACE_CString server_object_ior);

  /// Returns information related to startup.
  int get_startup_info (const ACE_CString POA_name,
                        ACE_CString &logical_server_name,
                        ACE_CString &startup_command,
                        ImplementationRepository::EnvironmentList
                           &environment_vars,
                        ACE_CString &working_dir,
                        ImplementationRepository::ActivationMode
                        &activation);

  /// Returns information related to a running copy.
  int get_running_info (const ACE_CString POA_name,
                        ACE_CString &location,
                        ACE_CString &server_object_ior);

  /// Checks the starting_up_ variable in the Server_Info and
  /// returns the previous value or -1 if the POA_name wasn't found
  int starting_up (const ACE_CString POA_name, int new_value);

  /// Same as above but does not alter the value.
  int starting_up (const ACE_CString POA_name);

  /// Removes the server from the Repository.
  int remove (const ACE_CString POA_name);

  /// Write into the xml file.
  int write_to_xml (const ACE_CString POA_name,
                    const ACE_CString logical_server_name,
                    const ACE_CString startup_command,
                    const ImplementationRepository::EnvironmentList
                      environment_vars,
                    const ACE_CString working_dir,
                    const ImplementationRepository::ActivationMode activation);
  /// Returns a new iterator that travels over the repository.
  HASH_IMR_ITER *new_iterator (void);

  /// Returns the number of entries in the repository.
  size_t get_repository_size (void);

private:

  HASH_IMR_MAP repository_;
  ACE_Configuration_Section_Key servers_;
  XML_ContentHandler *handler_;
};


#endif /* REPOSITORY_H */
