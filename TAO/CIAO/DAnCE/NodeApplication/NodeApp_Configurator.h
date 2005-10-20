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

#include "ciao/Deployment_CoreC.h"

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
    /// Default constructor.
    NodeApp_Configurator (CORBA::ORB_ptr o);

    /// Default destructor.
    virtual ~NodeApp_Configurator (void) = 0;

    /**
     * @brief "initialize" is called after NodeApplication get a hold
     * at this object.
     */
    virtual int initialize (void) = 0;

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

  protected:
    CORBA::ORB_var orb_;
  };
}

#include /**/ "ace/post.h"
#endif /* NODEAPP_CONFIGURATOR_H */
