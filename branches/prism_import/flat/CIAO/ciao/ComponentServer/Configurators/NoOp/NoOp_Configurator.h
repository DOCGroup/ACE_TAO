/** $Id$
 * @file  NoOp_Configurator.h
 * @brief This file contains the noop configurator.
 */

#ifndef NOOP_CONFIGURATOR_H
#define NOOP_CONFIGURATOR_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ccm/CCM_StandardConfiguratorC.h>

#include "Server_Configurator.h"
#include "NoOp_Configurator_export.h"

namespace CIAO
{
  namespace Deployment
  {
    /**
     * @class NoOP_Configurator
     *
     * @brief The NoOp_Configurator doesn't do anything.
     */
    class NoOp_Configurator_Export NoOp_Configurator
      : public ComponentServer_Configurator
    {
    public:
      /// Default destructor.
      virtual ~NoOp_Configurator (void);

      /**
       * @brief "pre_orb_initialize" is called before ORB_init.
       */
      virtual int pre_orb_initialize (void);

      /**
       * @brief "post_orb_initialize" is called after NodeApplication
       * get a hold at this object.
       */
      virtual int post_orb_initialize (CORBA::ORB_ptr o);

      /**
       * @brief "init_resource_manager" is called by NodeApplication when
       * it receives an "install" commands.
       */
      virtual int
	init_resource_manager (const ::Components::ConfigValues &properties);

      /**
       * @brief get a policyset by its name.
       */
      virtual CORBA::PolicyList *
	find_container_policies (const ::Components::ConfigValues &properties);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* NOOP_CONFIGURATOR_H */
