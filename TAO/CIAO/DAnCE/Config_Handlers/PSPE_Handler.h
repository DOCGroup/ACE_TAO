//==============================================================
/**
 *  @file  PSPE_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_PSPE_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PSPE_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct PlanSubcomponentPortEndpoint; 
}

namespace CIAO
{

  namespace Config_Handlers
  {
    
   struct PlanSubcomponentPortEndpoint;
   
   /*
    * @class PSPE_Handler
    *
    * @brief Handler class for <PlanSubcomponentPortEndpoint> types.
    *
    * This class defines handler methods to map values from
    * XSC PlanSubcomponentPortEndpoint objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */
    
    class Config_Handlers_Export PSPE_Handler{
     
      public:
       
        PSPE_Handler (void);
        virtual ~PSPE_Handler (void);
  
        ///This method takes a <Deployment::PlanConnectionDescription>
        ///and maps the values from the passed in XSC 
        ///PlanConnectionDescription to its members.
        void get_PlanSubcomponentPortEndpoint (
                    Deployment::PlanSubcomponentPortEndpoint& toconfig,
                    PlanSubcomponentPortEndpoint& desc);          

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_PSPE_HANDLER_H*/
