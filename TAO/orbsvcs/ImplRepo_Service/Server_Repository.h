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
#include "Activator_Options.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Configuration.h"
#include "ace/Auto_Ptr.h"


//class XML_ContentHandler;


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
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  Server_Info_Ptr,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex> SIMap;

  /// @param rmode One of REPO_NONE, REPO_XML_FILE, REPO_HEAP_FILE, or REPO_REGISTRY
  /// @param name  The name of a file or registry key used for persistence.
  Server_Repository(const ACE_CString& activator_name);

  /// Initializes the Server Repository
  int init (Options::RepoMode rmode, const ACE_CString& name);

  /// Add a new server to the Repository
  int add (const ACE_CString& server_name,
           const ACE_CString& startup_command,
           const ImplementationRepository::EnvironmentList& environment_vars,
           const ACE_CString& working_dir,
           ImplementationRepository::ActivationMode activation,
           const ACE_CString& partial_ior = ACE_CString(""),
           const ACE_CString& ior = ACE_CString(""));

  /// Update the associated process information.
  int update (const Server_Info& info);

  /// Returns information related to startup.
  Server_Info_Ptr get_server_info (const ACE_CString& server_name);

  /// Removes the server from the Repository.
  int remove (const ACE_CString& server_name);

  /// Returns the internal hash map containing the server information.
  SIMap& servers(void);

  const char* activator_name(void) const;

private:
  // Type mechanism to use for persistence.
  Options::RepoMode rmode_;
  // The in-memory list of the server information.
  SIMap server_infos_; 
  // Several rmode_ values require this.
  ACE_Auto_Ptr<ACE_Configuration> config_;
  // XML requires the file name
  ACE_CString fname_;
  ACE_CString activator_name_;
};


#endif /* REPOSITORY_H */
