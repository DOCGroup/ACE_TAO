/* -*- C++ -*- */

//=============================================================================
/**
*  @file Config_Backing_Store.h
*
*  $Id$
*
*  These classes define ACE_Configuration implementations of the backing store.
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*  @author Priyanka Gontla <gontla_p@ociweb.com>
*/
//=============================================================================

#ifndef CONFIG_BACKING_STORE_H
#define CONFIG_BACKING_STORE_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Locator_Repository.h"

#include "ace/Configuration.h"

/**
* @class Config_Backing_Store
*
* @brief ACE_Configuration backing store interface containing all ImR persistent
* information.
*
*/
class Config_Backing_Store : public Locator_Repository
{
public:
  Config_Backing_Store(const Options& opts,
                       CORBA::ORB_ptr orb,
                       ACE_Configuration& config);

  virtual ~Config_Backing_Store();

protected:
  /// perform config backing store specific initialization
  /// (loads servers and activators from the backing store)
  virtual int init_repo(PortableServer::POA_ptr imr_poa);

  /// perform server persistent update
  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  /// perform activator persistent update
  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  /// perform persistent remove
  virtual int persistent_remove(const ACE_CString& name, bool activator);

  /// remove the section with the given name from the key section
  int remove(const ACE_CString& name, const ACE_TCHAR* key);

  /// the ACE_Configuration backing store
  ACE_Configuration& config_;
  /// the status of the config_
  int status_;

private:
  /// load all servers
  void loadServers();
  /// load all activators
  void loadActivators();
};

/**
* @class Heap_Backing_Store
*
* @brief Heap file backing store containing all ImR persistent information.
*
*/
class Heap_Backing_Store : public Config_Backing_Store
{
public:
  Heap_Backing_Store(const Options& opts,
                     CORBA::ORB_ptr orb);

  virtual ~Heap_Backing_Store();

  /// indicate the Heap filename as the persistence mode for the repository
  virtual const ACE_TCHAR* repo_mode() const;

private:
  /// the Heap filename
  const ACE_TString filename_;
  /// the Heap used for the actual backing store
  ACE_Configuration_Heap heap_;
};

/**
* @class Registry_Backing_Store
*
* @brief Win32 registry backing store containing all ImR persistent
* information.
*
*/
class Registry_Backing_Store : public Config_Backing_Store
{
public:
  Registry_Backing_Store(const Options& opts,
                         CORBA::ORB_ptr orb);

  virtual ~Registry_Backing_Store();

  /// indicate Registry as the persistence mode for the repository
  virtual const ACE_TCHAR* repo_mode() const;

private:
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
  /// the Win32 registry used for the actual backing store
  ACE_Configuration_Win32Registry win32registry_;
#else
  // invalid config (used to simplify #defines)
  ACE_Configuration_Heap invalid_config_;
#endif
};


#endif /* CONFIG_BACKING_STORE_H */
