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
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "RTConfigurationC.h"

namespace CIAO
{
  /**
   * @brief RTResource_Config_Manager maintains a collection of
   * RTCORBA related resources.
   *
   * This class is responsible to create and manage a list of RTCORBA
   * related resources that are global to an RT ComponentServer.  The
   * information required to create and configure these resources are
   * passed in via the @a info argument in the @c init method.  These
   * resources are identified by names.  This class is used by the
   * RTPolicy_Set_Manager for associating these resources with related
   * RTPolicies.  These resources are destoryed along with this class.
   *
   * Currently, the only resources supported (and managed) by this
   * class are Threadpool and Threadpool_with_Lanes.
   */
  class RTResource_Config_Manager
  {
  public:
    RTResource_Config_Manager (RTCORBA::RTORB_ptr orb);
    ~RTResource_Config_Manager ();

    /// Initializing the RTResource_Config_Manager
    void init (const CIAO::RTConfiguration::RTORB_Resources_Info &info
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Finalizing the RTResource_Config_Manager and the resources it
    /// manages.
    void fini (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Query a thread pool by name.
    RTCORBA::ThreadpoolId find_threadpool_by_name (const char *name
                                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /*
    /// Query a priority bands infor by name.
    RTCORBA::PriorityBands *find_priority_bands_by_name (const char *name
                                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS);
    // @@ THROW SPEC?
    */

  private:
    // Cached an RTORB reference.
    RTCORBA::RTORB_var rtorb_;

    /// Hash_Map stuff.
    typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                    RTCORBA::ThreadpoolId,
                                    ACE_Hash<ACE_CString>,
                                    ACE_Equal_To<ACE_CString>,
                                    ACE_Null_Mutex> TP_MAP;

    /// Internal TP names to id map.
    TP_MAP threadpool_map_;
  };

  /**
   * @brief This class creates and maintains named policy sets which
   * can be indexed via names.
   *
   * This class requires to use the RTResource_Config_Manager.
   */
  class RTPolicy_Set_Manager
  {
  public:
    RTPolicy_Set_Manager (RTResource_Config_Manager &resource_config,
                          CORBA::ORB_ptr orb,
                          RTCORBA::RTORB_ptr rtorb);
    ~RTPolicy_Set_Manager ();

    /// Initialize the RTPolicy_Set_Manager and the policies it manages.
    void init (const CIAO::RTConfiguration::Policy_Sets &sets
               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Finalizing the RTPolicy_Set_Manager and the policies it
    /// creates.
    void fini (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Query a policy set by name
    CORBA::PolicyList *find_policies_by_name (const char *name
                                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    /// Reference to Resource Manager
    RTResource_Config_Manager &resource_manager_;

    /// Cached an ORB reference.
    CORBA::ORB_var orb_;

    /// Cached an RTORB reference.
    RTCORBA::RTORB_var rtorb_;

    /// create a single policy
    CORBA::Policy_ptr create_single_policy
    (const CIAO::RTConfiguration::Policy_Config &policy_config
     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

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

#if defined (__ACE_INLINE__)
# include "RTConfig_Manager.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* CIAO_RTCONFIG_MANAGER_H */
