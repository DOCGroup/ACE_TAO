
      //==============================================================
/**
 *  @file  MDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_MDD_Handler_H
#define CIAO_CONFIG_HANDLERS_MDD_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class MonolithicDeploymentDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class MonolithicDeploymentDescription;


   /*
    * @class MDD_Handler
    *
    * @brief Handler class for <MonolithicDeploymentDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC MonolithicDeploymentDescription objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export MDD_Handler {
     
      public:

        MDD_Handler (void);
        virtual ~MDD_Handler (void);

        static void monolithic_deployment_descr (
             const MonolithicDeploymentDescription& desc,
             Deployment::MonolithicDeploymentDescription& toconfig);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_MDD_Handler_H */

