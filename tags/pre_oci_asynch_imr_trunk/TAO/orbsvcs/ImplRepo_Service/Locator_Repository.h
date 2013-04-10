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

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/IOR_Multicast.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Configuration.h"
#include "ace/Auto_Ptr.h"
#include "ace/Reactor.h"

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
  typedef ACE_CString ServerKey;
  typedef ACE_Hash_Map_Manager_Ex<ServerKey,
    Server_Info_Ptr,
    ACE_Hash<ServerKey>,
    ACE_Equal_To<ServerKey>,
    ACE_Null_Mutex> SIMap;

  typedef ACE_CString ActivatorKey;
  typedef ACE_Hash_Map_Manager_Ex<ActivatorKey,
    Activator_Info_Ptr,
    ACE_Hash<ActivatorKey>,
    ACE_Equal_To<ActivatorKey>,
    ACE_Null_Mutex> AIMap;

  Locator_Repository(const Options& opts, CORBA::ORB_ptr orb);

  virtual ~Locator_Repository();

  int unregister_if_address_reused (const ACE_CString& server_id,
                                    const ACE_CString& name,
                                    const char* partial_ior);

  /// Add a new server to the Repository
  int add_server (const ACE_CString& server_id,
    const ACE_CString& name,
    bool jacorbs,
    const ACE_CString& aname,
    const ACE_CString& startup_command,
    const ImplementationRepository::EnvironmentList& environment_vars,
    const ACE_CString& working_dir,
    ImplementationRepository::ActivationMode activation,
    int start_limit,
    const ACE_CString& partial_ior = ACE_CString(""),
    const ACE_CString& ior = ACE_CString(""),
    ImplementationRepository::ServerObject_ptr svrobj =
      ImplementationRepository::ServerObject::_nil()
    );
  /// Add a new activator to the Repository
  int add_activator (const ACE_CString& name,
    const CORBA::Long token,
    const ACE_CString& ior = ACE_CString(""),
    ImplementationRepository::Activator_ptr act =
      ImplementationRepository::Activator::_nil()
    );

  /// Update the associated information.
  int update_server (const Server_Info_Ptr& info);
  /// Update the associated information.
  int update_activator (const Activator_Info_Ptr& info);

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
  const SIMap& servers(void) const;
  /// Returns the internal hash map containing the activator information.
  AIMap& activators(void);
  const AIMap& activators(void) const;

  /// indicate the persistence mode for the repository
  virtual const ACE_TCHAR* repo_mode() const = 0;

  /// convert to lower case
  static ACE_CString lcase (const ACE_CString& s);

  /// Initialize the repo
  int init(PortableServer::POA_ptr root_poa,
           PortableServer::POA_ptr imr_poa,
           const char* this_ior);

  /// Indicate if multicast should be used
  bool multicast() const;

protected:
  /// perform repo mode specific initialization
  virtual int init_repo(PortableServer::POA_ptr imr_poa) = 0;

  /// perform sync of repo with backing store
  /// defaults to no-op, only shared backing stores
  /// need to sync
  virtual int sync_load();

  /// perform server persistent update
  virtual int persistent_update(const Server_Info_Ptr& info, bool add) = 0;

  /// perform activator persistent update
  virtual int persistent_update(const Activator_Info_Ptr& info, bool add) = 0;

  /// perform persistent remove
  virtual int persistent_remove(const ACE_CString& name, bool activator) = 0;

  /// report the ImR Locator's IOR
  virtual int report_ior(PortableServer::POA_ptr imr_poa);

  /// recover the ImR Locator's IOR from the persisted file
  virtual int recover_ior(void);

  int setup_multicast (ACE_Reactor* reactor, const char* imr_ior);
  void teardown_multicast();

  bool registered() const;

  const Options& opts_;
  TAO_IOR_Multicast ior_multicast_;
  const CORBA::ORB_var orb_;
  CORBA::String_var imr_ior_;

private:
  bool registered_;
  /// The in-memory list of the server information.
  SIMap server_infos_;
  /// The in-memory list of the activator information.
  AIMap activator_infos_;
};

/**
* @class XML_Backing_Store
*
* @brief XML backing store interface containing all ImR persistent information
* in a single file
*
*/
class No_Backing_Store : public Locator_Repository
{
public:
  No_Backing_Store(const Options& opts,
                   CORBA::ORB_ptr orb);

  virtual ~No_Backing_Store();

  /// indicate the persistence mode for the repository
  virtual const ACE_TCHAR* repo_mode() const;

private:
  /// perform repo mode specific initialization (no-op)
  virtual int init_repo(PortableServer::POA_ptr imr_poa);

  /// perform server persistent update (no-op)
  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  /// perform activator persistent update (no-op)
  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  /// perform persistent remove (no-op)
  virtual int persistent_remove(const ACE_CString& name, bool activator);
};


/**
* @class UpdateableServerInfo
*
* @brief Class for managing changes to ServerInfo memory to ensure
* it is persisted
*
*/
class UpdateableServerInfo
{
public:
  /// constructor
  /// @param repo the repo to report updates to
  /// @param name the name of the server to retrieve
  UpdateableServerInfo(Locator_Repository* repo, const ACE_CString& name);

  /// constructor
  /// @param repo the repo to report updates to
  /// @param si an already retrieved Server_Info_Ptr
  UpdateableServerInfo(Locator_Repository* repo, const Server_Info_Ptr& si);

  /// constructor (no repo updates will be performed)
  /// @param si a Server_Info to create a non-stored Server_Info_Ptr from
  UpdateableServerInfo(const Server_Info& si);

  /// destructor (updates repo if needed)
  ~UpdateableServerInfo();

  /// explicitly update repo if needed
  void update_repo();

  /// const Server_Info access
  const Server_Info* operator->() const;

  /// const Server_Info& access
  const Server_Info& operator*() const;

  /// retrieve smart pointer to non-const Server_Info
  /// and indicate repo update required
  const Server_Info_Ptr& edit();

  /// force indication of update needed
  void needs_update();

  /// indicate it Server_Info_Ptr is null
  bool null() const;
private:
  UpdateableServerInfo(const UpdateableServerInfo& );
  const UpdateableServerInfo& operator=(const UpdateableServerInfo& );

  /// the repo
  Locator_Repository* const repo_;

  /// the retrieved, passed, or non-stored server info
  const Server_Info_Ptr si_;

  /// the server info has changes and needs to be updated to the repo
  bool needs_update_;
};

#endif /* REPOSITORY_H */
