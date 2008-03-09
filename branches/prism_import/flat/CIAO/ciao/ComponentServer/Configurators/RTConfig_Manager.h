// -*- C++ -*-

//=============================================================================
/**
 *  @file    RTConfig_Manager.h
 *
 *  $Id$
 *
 *  This file contains classes that manage name to configuration
 *  mapping info in a RTComponentServer.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef CIAO_RTCONFIG_MANAGER_H
#define CIAO_RTCONFIG_MANAGER_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ace/Null_Mutex.h>
#include <ace/SString.h>
#include <ace/Hash_Map_Manager_T.h>
#include <tao/RTCORBA/RTCORBA.h>
#include <ciao/ComponentServer/CIAO_ServerResourcesC.h>

#include "Config_Manager.h"
#include "RTNA_Configurator_Export.h"

namespace CIAO
{
  namespace Deployment
  {
    /**
     * @brief RTResource_Config_Manager maintains a collection of
     * RTCORBA related resources.
     *
     * This class is responsible to create and manage a list of RTCORBA
     * related resources that are global to an RT ComponentServer.  The
     * information required to create and configure these resources are
     * passed in via the @a info argument in the @c init method.  These
     * resources are identified by names and can be used to associate
     * these resources with related RTPolicies.  These resources are
     * destoryed along with this class.
     *
     * Currently, the only resources supported (and managed) by this
     * class are Threadpool and Threadpool_with_Lanes.
     */
    class RTResource_Config_Manager : public Config_Manager
    {
    public:
      RTResource_Config_Manager (void);
      ~RTResource_Config_Manager (void);

      virtual void init (CORBA::ORB_ptr orb);

      virtual int pre_orb_initialize (void);

      virtual int post_orb_initialize (CORBA::ORB_ptr o);

      /// Initializing the RTResource_Config_Manager
      virtual void init_resources (const ServerResource &info);

      /// Finalizing the RTResource_Config_Manager and the resources it
      /// manages.
      void fini ();

      /// Query a policy set by name
      virtual CORBA::PolicyList *find_policies_by_name (const char *name);

      virtual bool policy_exists (const char *name);

    protected:
      void print_resources (const ServerResource &info);

      /// Query a thread pool by name.
      RTCORBA::ThreadpoolId find_threadpool_by_name (const char *name);


      /// Query a priority bands info by name.
      RTCORBA::PriorityBands *find_priority_bands_by_name (const char *name);

    private:
      /// Cached an ORB reference.
      CORBA::ORB_var orb_;

      // Cached an RTORB reference.
      RTCORBA::RTORB_var rtorb_;

      /// Hash_Map stuff.
      typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
				      RTCORBA::ThreadpoolId,
				      ACE_Hash<ACE_CString>,
				      ACE_Equal_To<ACE_CString>,
				      ACE_Null_Mutex> TP_MAP;

      typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
				      RTCORBA::PriorityBands_var,
				      ACE_Hash<ACE_CString>,
				      ACE_Equal_To<ACE_CString>,
				      ACE_Null_Mutex> PB_MAP;

    /// Internal TP name to id map.
    TP_MAP threadpool_map_;

    /// Internal PB name to priority bands map.
    PB_MAP priority_bands_map_;

    /// create a single policy
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

#if defined (__ACE_INLINE__)
# include "RTConfig_Manager.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_RTCONFIG_MANAGER_H */

