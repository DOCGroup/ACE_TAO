
      //==============================================================
/**
 *  @file  IDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_IDD_Handler_H
#define CIAO_CONFIG_HANDLERS_IDD_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class InstanceDeploymentDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class InstanceDeploymentDescription;


   /*
    * @class IDD_Handler
    *
    * @brief Handler class for <InstanceDeploymentDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC InstanceDeploymentDescription objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export IDD_Handler {
     
      public:

        IDD_Handler (void);
        virtual ~IDD_Handler (void);

        static void instance_deployment_descr (
             const InstanceDeploymentDescription& desc,
             Deployment::InstanceDeploymentDescription& toconfig);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_IDD_Handler_H */

