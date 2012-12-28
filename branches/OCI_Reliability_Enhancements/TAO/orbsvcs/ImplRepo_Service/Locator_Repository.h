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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Reactor;

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

  Locator_Repository(const Options& opts, CORBA::ORB_ptr orb);

  virtual ~Locator_Repository();

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

  virtual const char* repo_mode() const = 0;

  static ACE_CString lcase (const ACE_CString& s);

  int init(PortableServer::POA_ptr root_poa,
           PortableServer::POA_ptr imr_poa,
           const char* this_ior);

  bool multicast() const;

protected:
  virtual int init_repo(PortableServer::POA_ptr imr_poa);

  virtual int sync_load();

  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  virtual int persistent_remove(const ACE_CString& name, bool activator);

  virtual int report_ior(PortableServer::POA_ptr root_poa,
                         PortableServer::POA_ptr imr_poa);

  void report(IORTable::Table_ptr ior_table,
              const char* name,
              const char* imr_ior);

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

  virtual const char* repo_mode() const;

private:
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
  UpdateableServerInfo(Locator_Repository* repo, const ACE_CString& name);
  UpdateableServerInfo(Locator_Repository* repo, const Server_Info_Ptr& si);
  UpdateableServerInfo(const Server_Info& si);

  ~UpdateableServerInfo();

  void update_repo();

  const Server_Info* operator->() const;

  const Server_Info& operator*() const;

  const Server_Info_Ptr& edit();

  void needs_update();

  bool null() const;
private:
  UpdateableServerInfo(const UpdateableServerInfo& );
  const UpdateableServerInfo& operator=(const UpdateableServerInfo& );

  Locator_Repository* const repo_;
  const Server_Info_Ptr si_;
  bool needs_update_;
};

#endif /* REPOSITORY_H */
