//==============================================================
/**
 *  @file  MDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_MDD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_MDD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Basic_Deployment_Data.hpp"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct MonolithicDeploymentDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {
   /*
    * @class MDD_Handler
    *
    * @brief Handler class for <MonolithicDeploymentDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC MonolithicDeploymentDescription objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */
    
    class MDD_Handler{
     
      public:
       
        MDD_Handler (void);
        virtual ~MDD_Handler (void);
  
        ///This method takes a <Deployment::MonolithicDeploymentDescription>
        ///and maps the values from the passed in XSC 
        ///MonolithicDeploymentDescription to its members.
        void get_MonolithicDeploymentDescription (
                    Deployment::MonolithicDeploymentDescription& toconfig,
                    MonolithicDeploymentDescription& desc);          

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_MDD_HANDLER_H*/
