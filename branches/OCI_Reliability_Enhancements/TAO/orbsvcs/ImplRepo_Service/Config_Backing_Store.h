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

class ACE_Configuration;

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
                       const CORBA::ORB_var& orb,
                       ACE_Configuration& config);

  virtual ~Config_Backing_Store();

protected:
  virtual int init_repo(const PortableServer::POA_var& imr_poa);

  virtual int persistent_update(const Server_Info_Ptr& info, bool add);

  virtual int persistent_update(const Activator_Info_Ptr& info, bool add);

  virtual int persistent_remove(const ACE_CString& name, bool activator);

  int remove(const ACE_CString& name, const ACE_TCHAR* key);

  ACE_Configuration& config_;
  int status_;

private:
  void loadServers();
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
                     const CORBA::ORB_var& orb);

  virtual ~Heap_Backing_Store();

  virtual const char* repo_mode() const;

private:
  const ACE_TString filename_;
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
                         const CORBA::ORB_var& orb);

  virtual ~Registry_Backing_Store();

  virtual const char* repo_mode() const;

private:
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
  ACE_Configuration_Win32Registry win32registry_;
#else
  // invalid config to simplify #defines
  ACE_Configuration_Heap invalid_config_;
#endif
};


#endif /* CONFIG_BACKING_STORE_H */
