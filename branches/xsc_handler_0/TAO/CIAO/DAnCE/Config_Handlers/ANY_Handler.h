//================================================
/**
 *  @file  ANY_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef ANY_HANDLER_H
#define ANY_HANDLER_H

#include /**/ "ace/pre.h"
                                                                                
#include "ciao/DeploymentC.h"
#include "Basic_Deployment_Data.hpp"
             
                                                                                
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO{
    
  namespace Config_Handlers{


   /*
    * @class ANY_Handler
    * 
    * @brief Handler class for <ComponentInterfaceDescription> types.
    * 
    * This class defines handler methods to map values from 
    * XSC Any objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    * 
    */
    
    class ANY_Handler{
     
      public:
       
      ANY_Handler();
      virtual ~ANY_Handler();
  
      static CORBA::Any& get_Any(CORBA::Any& toconfig, Any& desc); 
         
    };

  }
   
}
#include /**/ "ace/post.h"
#endif /* ANY_HANDLER_H */
