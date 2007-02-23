//$Id$
/**========================================================
 *
 * @file   NodeApp_Configurator.h
 *
 * @Brief  This file contains the implementation of
 *         the basic NodeApp_Configurator interface, the one
 *         that does nothing more
 *
 * @author Nanbor Wang <nanbor@exothermic.txcorp.com>
 *========================================================*/

#ifndef NODEAPP_CONFIGURATOR_H
#define NODEAPP_CONFIGURATOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DAnCE/Deployment/Deployment_CoreC.h"

namespace CIAO
{
  /**
   *
   * @class NodeApp_Configurator
   *
   * @brief This class defines and implements the default
   * NodeApp_Configurator base class that's needed for configuring the
   * NodeApplication process.
   **/
  class NodeApp_Configurator
  {
  public:
    /// Default destructor.
    virtual ~NodeApp_Configurator (void) {}

    /**
     * @brief "pre_orb_initialize" is called before ORB_init.
     */
    virtual int pre_orb_initialize (void) = 0;

    /**
     * @brief "post_orb_initialize" is called after NodeApplication
     * get a hold at this object.
     */
    virtual int post_orb_initialize (CORBA::ORB_ptr o) = 0;

    /**
     * @brief "init_resource_manager" is called by NodeApplication when
     * it receives an "install" commands.
     */
    virtual int
    init_resource_manager (const ::Deployment::Properties &properties) = 0;

    /**
     * @brief get a policyset by its name.
     */
    virtual CORBA::PolicyList *
    find_container_policies (const ::Deployment::Properties &properties) = 0;

    /// @@ Perhaps we also need a finalizer method here.  Perhaps we
    /// even need to differentiate finalizer before ORB is destroyed
    /// and the one after that.
  };
}

/**
 * For dynamically linkable concrete NodeApp_Configurator
 * implementation, remember to create a factory method using "C"
 * calling convention in the CPP file as follow:

  extern "C" EXPORT_MACRO CIAO::NodeApp_Configurator *create_nodeapp_configurator (void);

  CIAO::NodeApp_Configurator *
  create_nodeapp_configurator (void)
  {
    concrete_NodeApp_Configurator *config;
    ACE_NEW_RETURN (config, concrete_NodeApp_Configurator, 0);
    return config;
  }

 */


#include /**/ "ace/post.h"
#endif /* NODEAPP_CONFIGURATOR_H */
