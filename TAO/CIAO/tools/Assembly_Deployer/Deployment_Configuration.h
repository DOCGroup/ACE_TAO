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
 */
//=============================================================================


#ifndef CIAO_DEPLOYMENT_CONFIGURATION_H
#define CIAO_DEPLOYMENT_CONFIGURATION_H
#include /**/ "ace/pre.h"

#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "CCM_DeploymentC.h"

namespace CIAO
{
  /**
   * @class Deployment_Configuration
   *
   * @brief A class that provides strategies on deployment topology
   *
   * This class provides strategies on how the Assembly framework
   * should deploy an assembly.  This is achieved by providing
   * mappings from deployment destination names to actually CIAO daemon
   * IORs, and the strategy for which default CIAO daemon a deployment
   * mechanism should use.
   *
   * This is a trivial implementation of the deployment configuration
   * strategy.  We can enhance this class later on to provide
   * different deployment location strategies.
   */
  class Deployment_Configuration
  {
  public:
    typedef struct _activator_info
    {
      _activator_info (const char *ior = 0)
      {
        IOR_ = ior;
      }

      ACE_CString IOR_;
      Components::Deployment::ServerActivator_var activator_;
    } Activator_Info;

    Deployment_Configuration (CORBA::ORB_ptr o);

    /// Destructor
    virtual ~Deployment_Configuration (void);

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
     * @retval 0 if no valid name were found.
     */
    virtual const char *get_activator_ior (const char *name);

    /**
     * Return the default ServerActivator the Assembly mechanism
     * should use to deploy a component.  In this implementation, the
     * first entry in the deployment configuration data file is always
     * the entry for default activator.
     *
     * @retval 0 if no valid daemon is configured.
     */
    virtual const char *get_default_activator_ior (void);

    /**
     * @retval nil if no valid name were found.
     */
    virtual Components::Deployment::ServerActivator_ptr
    get_activator (const char *name);

    /**
     * Return the reference to the default ServerActivator the
     * Assembly mechanism should use to deploy a component.
     *
     * @retval nil if no valid daemon is configured.
     */
    virtual Components::Deployment::ServerActivator_ptr
    get_default_activator (void);

  protected:
    CORBA::ORB_var orb_;

    ACE_Hash_Map_Manager_Ex<ACE_CString,
                            Activator_Info,
                            ACE_Hash<ACE_CString>,
                            ACE_Equal_To<ACE_CString>,
                            ACE_Null_Mutex> deployment_info_;

    Activator_Info default_activator_;
  };

}

#if defined (__ACE_INLINE__)
# include "Deployment_Configuration.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* CIAO_DEPLOYMENT_CONFIGURATION_H */
