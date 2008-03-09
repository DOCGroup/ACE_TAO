// -*- C++ -*-

//=============================================================================
/**
 *  @file    NAConfig_Manager.h
 *
 *  $Id$
 *
 */
//=============================================================================


#ifndef CIAO_NACONFIG_MANAGER_H
#define CIAO_NACONFIG_MANAGER_H
#include /**/ <ace/pre.h>

#include <ace/config-all.h>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ace/Null_Mutex.h>
#include <ace/SString.h>
#include <ace/Hash_Map_Manager_T.h>
#include <ciao/CIAO_ServerResourcesC.h>

#include "Config_Manager.h"
#include "Basic_Config_Manager_Export.h" 

namespace CIAO
{
  namespace Deployment
  {
    /**
     */
    class Basic_Config_Manager_Export Basic_Config_Manager 
      : public Config_Manager
    {
    public:
      virtual ~Basic_Config_Manager (void) {}

      virtual int pre_orb_initialize (void);

      virtual int post_orb_initialize (CORBA::ORB_ptr o);

      virtual void init (CORBA::ORB_ptr orb);

      /// Initializing the NAResource_Config_Manager
      virtual void init_resources (const ServerResource &info);

      /// Query a policy set by name
      virtual CORBA::PolicyList *find_policies_by_name (const char *name);

      virtual bool policy_exists (const char *name);

    private:
      /// Cached an ORB reference.
      CORBA::ORB_var orb_;

      CORBA::Policy_ptr create_single_policy
	(const PolicyDef &policy_def);

      /// Hash_Map stuff.
      typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
	CORBA::PolicyList_var,
	ACE_Hash<ACE_CString>,
	ACE_Equal_To<ACE_CString>,
	ACE_Null_Mutex> POLICY_MAP;

    /// Internal TP names to id map.
    POLICY_MAP policy_map_;
  };
}
  }

#include /**/ "ace/post.h"
#endif /* CIAO_NACONFIG_MANAGER_H */

