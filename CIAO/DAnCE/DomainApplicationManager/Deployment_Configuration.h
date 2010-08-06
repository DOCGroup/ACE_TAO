// -*- C++ -*-

//=============================================================================
/**
 *  @file    Deployment_Configuration.h
 *
 *  $Id$
 *
 *  The Deployment_Configuration provides abstraction for the
 *  deployment mechanisms.
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author Gan Deng <gan.deng@vanderbilt.edu>
 */
//=============================================================================
#ifndef CIAO_DEPLOYMENT_CONFIGURATION_H
#define CIAO_DEPLOYMENT_CONFIGURATION_H
#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DAnCE/Deployment/Deployment_NodeManagerC.h"
#include "tao/Valuetype/ValueBase.h"
#include "tao/Valuetype/Valuetype_Adapter_Impl.h"
#include "ace/SString.h"

#include "DomainApplicationManager/DomainApplicationManager_Export.h"

namespace CIAO
{
  /**
   * @class Deployment_Configuration
   *
   * @brief A class that provides strategies on deployment topology.
   *
   * This class provides strategies on how the DomainApplicationManager
   * should deploy an deployment plan at domain-level.  This is achieved
   * by providing mappings from deployment destination names to actually
   * NodeManager daemon IORs, and the strategy for which default NodeManager
   * daemons a deployment mechanism should use.
   *
   * This is a trivial implementation of the deployment configuration
   * strategy.  We can enhance this class later on to provide
   * different deployment location strategies, such as naming service.
   */
  class DomainApplicationManager_Export Deployment_Configuration
  {
  public:
    typedef struct _node_manager_info
    {
      _node_manager_info (const char *ior = 0)
      {
        IOR_ = ior;
      }

      ACE_CString IOR_;
      ::Deployment::NodeManager_var node_manager_;
    } Node_Manager_Info;

    Deployment_Configuration (CORBA::ORB_ptr o);

    /// Destructor
    ~Deployment_Configuration (void);

    /**
     * Init method takes the filename to a configuration file which
     * has a simple line format of name, ior string delimited by a
     * space in each line.
     *
     *     name IOR-string
     *
     * @retval 0 on success.
     * @retval -1 otherwise.
     */
    int init (const char *filename);

    /**
     * @retval 0 if no valid name were found.  When @c name = 0, then
     * this function behave exactly as get_default_activator_ior.
     */
    const char *get_node_manager_ior (const char *name) const;

    /**
     * Return the default NodeManager the DomainApplicationManager
     * should use to deploy a component.  In this implementation, the
     * first entry in the deployment configuration data file is always
     * the entry for default activator.
     *
     * @retval 0 if no valid daemon is configured.
     */
    const char *get_default_node_manager_ior () const;

    /**
     * @retval nil if no valid name were found.
     */
    ::Deployment::NodeManager_ptr
    get_node_manager (const char *name);

    /**
     * Return the reference to the default NodeManager the
     * DomainApplicationManager should use to deploy a node-level
     * deployment plan..
     *
     * @retval nil if no valid daemon is configured.
     */
    ::Deployment::NodeManager_ptr
    get_default_node_manager ();

  protected:
    CORBA::ORB_var orb_;

    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            Node_Manager_Info,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> deployment_info_;

    Node_Manager_Info default_node_manager_;
  };

}

#include /**/ "ace/post.h"
#endif /* CIAO_DEPLOYMENT_CONFIGURATION_H */
