//==============================================================
/**
 *  @file  IDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct InstanceDeploymentDescriptionn; 
}

namespace CIAO
{

  namespace Config_Handlers
  {
    
   struct InstanceDeploymentDescriptionn;
   
   /*
    * @class IDD_Handler
    *
    * @brief Handler class for <InstanceDeploymentDescriptionn> types.
    *
    * This class defines handler methods to map values from
    * XSC InstanceDeploymentDescriptionn objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */
    
    class Config_Handlers_Export IDD_Handler{
     
      public:
       
        IDD_Handler (void);
        virtual ~IDD_Handler (void);
  
        ///This method takes a <Deployment::InstanceDeploymentDescriptionn>
        ///and maps the values from the passed in XSC 
        ///InstanceDeploymentDescriptionn to its members.
        void get_InstanceDeploymentDescriptionn (
                    Deployment::InstanceDeploymentDescriptionn& toconfig,
                    InstanceDeploymentDescriptionn& desc);          

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IDD_HANDLER_H*/
