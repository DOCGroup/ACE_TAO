//================================================
/**
 *  @file  Prop_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef PROP_HANDLER_H
#define PROP_HANDLER_H

#include /**/ "ace/pre.h"
                                                                                
#include "ciao/DeploymentC.h"
#include "Basic_Deployment_Data.hpp"
             
                                                                                
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO{
    
  namespace Config_Handlers{

  /*
   * @class Prop_Handler
   * 
   * @brief Handler class for <CCMComponentPortDescription> types.
   * 
   * This class defines handler methods to map values from 
   * XSC objects, parsed from the descriptor files, to the
   * corresponding CORBA IDL type for the schema element.
   * 
   */

   class Prop_Handler{
 
    public:
       
      Prop_Handler (void);
      virtual ~Prop_Handler (void);
              
      ///This method maps the values from the
      ///XSC object <CIAO::Config_Handlers::Property> to
      ///the CORBA IDL type <Deployment::Property>.    
      void get_Property (Deployment::Property& toconfig,
                         Property& desc);
   };

 }
}

#include /**/ "ace/post.h"
#endif /* PROP_HANDLER_H */
