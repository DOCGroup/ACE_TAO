// $Id$
/**========================================================
 *
 * @file   RTNodeApp_Configurator.h
 *
 * @Brief  This file contains the implementation of
 *         the RTNodeApp_Configurator, a concrete NodeApp_Configurator for
 *         setting up real-time support for NodeApplication.
 *
 *
 * @author Nanbor Wang <nanbor@exothermic.txcorp.com>
 *========================================================*/

#ifndef RTNODEAPP_CONFIGURATOR_H
#define RTNODEAPP_CONFIGURATOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "NodeApp_Configurator.h"
#include "RTNA_Configurator_Export.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "RTConfig_Manager.h"

namespace CIAO
{
  /**
   * @class RTNodeApp_Configurator
   *
   * @brief This class implements a concrete NodeApp_Configurator for
   * setting up the NodeApplication process for RT support.
   **/
  class CIAO_RTNA_Configurator_Export RTNodeApp_Configurator :
    public NodeApp_Configurator
  {
  public:
    /// Default destructor.
    virtual ~RTNodeApp_Configurator (void);

    /**
     * @brief "pre_orb_initialize" is called before ORB_init.
     */
    virtual int pre_orb_initialize (void);

    /**
     * @brief "initialize" is called after NodeApplication get a hold
     * at this object.
     */
    virtual int post_orb_initialize (CORBA::ORB_ptr o);

    /**
     * @brief "init_resource_manager" is called by NodeApplication when
     * it receives an "install" commands.
     */
    virtual int
    init_resource_manager (const ::Deployment::Properties &properties);

    /**
     * @brief get a policyset by its name.
     */
    virtual CORBA::PolicyList *
    find_container_policies (const ::Deployment::Properties &properties);

  protected:
    CORBA::ORB_var orb_;

    RTCORBA::RTORB_var rtorb_;

    RTResource_Config_Manager config_manager_;
  };
}

#include /**/ "ace/post.h"
#endif /* RTNODEAPP_CONFIGURATOR_H */
