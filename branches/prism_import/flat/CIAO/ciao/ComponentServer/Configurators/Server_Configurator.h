//$Id$
/**========================================================
 *
 * @file   ComponentServer_Configurator.h
 *
 * @Brief  This file contains the implementation of
 *         the basic ComponentServer_Configurator interface, the one
 *         that does nothing more
 *
 * @author Nanbor Wang <nanbor@exothermic.txcorp.com>
 *========================================================*/

#ifndef COMPONENTSERVER_CONFIGURATOR_H
#define COMPONENTSERVER_CONFIGURATOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <ace/DLL.h>
#include <ace/Auto_Ptr.h>
#include <ccm/CCM_StandardConfiguratorC.h>
#include "ComponentServer_Configurator_export.h"
#include "Config_Manager.h"

namespace CIAO
{
  namespace Deployment
  {
    /**
     *
     * @class ComponentServer_Configurator
     *
     * @brief This class defines and implements the default
     * ComponentServer_Configurator base class that's needed for configuring the
     * ComponentServerlication process.
     **/
    class ComponentServer_Configurator_Export ComponentServer_Configurator 
    {
    public:

      /// Default constructor.
      ComponentServer_Configurator (bool rt);

      /// Default destructor.
      virtual ~ComponentServer_Configurator (void);

      /**
       * @brief "pre_orb_initialize" is called before ORB_init.
       */
      virtual int pre_orb_initialize (void);

      /**
       * @brief "post_orb_initialize" is called after ComponentServer
       * get a hold at this object.
       */
      virtual int post_orb_initialize (CORBA::ORB_ptr o);

      /**
       * @brief "init_resource_manager" is called by ComponentServer when
       * it receives an "install" commands.
       */
      virtual int
        init_resource_manager (const ::Components::ConfigValues &properties);

      /**
       * @brief get a policyset by its name.
       */
      virtual CORBA::PolicyList *
        find_container_policies (const ::Components::ConfigValues &properties);

      /// @@ Perhaps we also need a finalizer method here.  Perhaps we
      /// even need to differentiate finalizer before ORB is destroyed
      /// and the one after that.

      bool rt_support (void);
      
      void set_rt_support (void);

      virtual bool create_config_managers (void);

      bool policy_exists (const char *policy_set_id);

      CORBA::PolicyList *
        find_policies_by_name (const char *name);

    protected:

      int rt_support_;

      CORBA::ORB_var orb_;

      auto_ptr<Config_Manager> rt_config_manager_;

      auto_ptr<Config_Manager> na_config_manager_;

      ACE_DLL config_dll_;
    };
  }
}

/**
 * For dynamically linkable concrete ComponentServer_Configurator
 * implementation, remember to create a factory method using "C"
 * calling convention in the CPP file as follow:

 extern "C" EXPORT_MACRO CIAO::ComponentServer_Configurator *create_nodeapp_configurator (void);

 CIAO::ComponentServer_Configurator *
 create_nodeapp_configurator (void)
 {
 concrete_ComponentServer_Configurator *config;
 ACE_NEW_RETURN (config, concrete_ComponentServer_Configurator, 0);
 return config;
 }

*/


#include /**/ "ace/post.h"
#endif /* NODEAPP_CONFIGURATOR_H */

