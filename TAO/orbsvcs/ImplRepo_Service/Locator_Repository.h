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

#ifndef LOCATOR_REPOSITORY_H
#define LOCATOR_REPOSITORY_H

#include "Server_Info.h"
#include "Activator_Info.h"
#include "Locator_Options.h"
#include "ImR_LocatorC.h"

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

class ImR_Locator_i;

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

  int unregister_if_address_reused (const ACE_CString& fqname,
                                    const char* partial_ior,
                                    ImR_Locator_i* imr_locator);

  /// Add a new server to the Repository
  int add_server (const ACE_CString& fqname,
                  const ImplementationRepository::StartupOptions &options);

  int add_server (const ACE_CString& fqname,
                  const ACE_CString& partial_ior,
                  const ACE_CString& ior,
                  ImplementationRepository::ServerObject_ptr svrobj);
  int add_server_i (Server_Info *si);

  /// create new records for poas that share a server instance. This is
  /// a two step process, first the base poa must be registered then a
  /// list of peers may be added.
  int link_peers (Server_Info_Ptr base,
                  const CORBA::StringSeq peers);

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

  /// Update the peer's access state
  virtual void notify_remote_access (const char *id,
                                     ImplementationRepository::AAM_Status state);

  /// Returns information related to startup.
  Server_Info_Ptr get_active_server (const ACE_CString& name, int pid = 0);
  Server_Info_Ptr get_info (const ACE_CString& name);
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
  virtual const ACE_TCHAR* repo_mode(void) const = 0;

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
  virtual int init_repo (PortableServer::POA_ptr imr_poa) = 0;

  /// perform sync of repo with backing store
  /// defaults to no-op, only shared backing stores
  /// need to sync
  virtual int sync_load (void);

  /// perform server persistent update
  virtual int persistent_update (const Server_Info_Ptr& info, bool add) = 0;

  /// perform activator persistent update
  virtual int persistent_update (const Activator_Info_Ptr& info, bool add) = 0;

  /// perform persistent remove
  virtual int persistent_remove (const ACE_CString& name, bool activator) = 0;

  /// report the ImR Locator's IOR
  virtual int report_ior (PortableServer::POA_ptr imr_poa);

  /// recover the ImR Locator's IOR from the persisted file
  virtual int recover_ior (void);

  int setup_multicast (ACE_Reactor* reactor, const char* imr_ior);
  void teardown_multicast ();

  bool registered () const;

  const Options& opts_;
  TAO_IOR_Multicast ior_multicast_;
  const CORBA::ORB_var orb_;
  CORBA::String_var imr_ior_;

private:
  Server_Info_Ptr find_by_poa (const ACE_CString &name);

  bool registered_;
  /// The in-memory list of the server information.
  SIMap server_infos_;
  /// The in-memory list of the activator information.
  AIMap activator_infos_;
};

/**
* @class No_Backing_Store
*
* @brief Null implementation, used when persistence isn't desired
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

#endif /* LOCATOR_REPOSITORY_H */
