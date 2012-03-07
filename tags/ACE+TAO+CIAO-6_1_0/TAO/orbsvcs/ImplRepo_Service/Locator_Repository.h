/* -*- C++ -*- */

//=============================================================================
/**
*  @file Locator_Repository.h
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
#include "Activator_Info.h"
#include "Locator_Options.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Configuration.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
* @class Locator_Repository
*
* @brief Database containing all ImR persistent information.
*
*/
class Locator_Repository
{
public:
  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
    Server_Info_Ptr,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> SIMap;

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
    Activator_Info_Ptr,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex> AIMap;

  Locator_Repository();

  /// Initializes the Server Repository
  int init (const Options& opts);

  int unregister_if_address_reused (const ACE_CString& server_id,
                                    const ACE_CString& name,
                                    const char* partial_ior);

  /// Add a new server to the Repository
  int add_server (const ACE_CString& server_id,
    const ACE_CString& name,
    const ACE_CString& aname,
    const ACE_CString& startup_command,
    const ImplementationRepository::EnvironmentList& environment_vars,
    const ACE_CString& working_dir,
    ImplementationRepository::ActivationMode activation,
    int start_limit,
    const ACE_CString& partial_ior = ACE_CString(""),
    const ACE_CString& ior = ACE_CString(""),
    ImplementationRepository::ServerObject_ptr svrobj = ImplementationRepository::ServerObject::_nil()
    );
  /// Add a new activator to the Repository
  int add_activator (const ACE_CString& name,
    const CORBA::Long token,
    const ACE_CString& ior = ACE_CString(""),
    ImplementationRepository::Activator_ptr act = ImplementationRepository::Activator::_nil()
    );

  /// Update the associated information.
  int update_server (const Server_Info& info);
  /// Update the associated information.
  int update_activator (const Activator_Info& info);

  /// Returns information related to startup.
  Server_Info_Ptr get_server (const ACE_CString& name);
  /// Returns information related to startup.
  Activator_Info_Ptr get_activator (const ACE_CString& name);

  bool has_activator(const ACE_CString& name);

  /// Removes the server from the Repository.
  int remove_server (const ACE_CString& name);
  /// Removes the activator from the Repository.
  int remove_activator (const ACE_CString& name);

  /// Returns the internal hash map containing the server information.
  SIMap& servers(void);
  /// Returns the internal hash map containing the activator information.
  AIMap& activators(void);

  const char* repo_mode();

private:
  /// Type mechanism to use for persistence.
  Options::RepoMode rmode_;
  /// The in-memory list of the server information.
  SIMap server_infos_;
  /// The in-memory list of the activator information.
  AIMap activator_infos_;
  /// Several rmode_ values require this.
  ACE_Auto_Ptr<ACE_Configuration> config_;
  /// XML requires the file name
  ACE_CString fname_;

  unsigned int debug_;
};


#endif /* REPOSITORY_H */
