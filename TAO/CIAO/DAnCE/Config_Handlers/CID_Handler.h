//================================================
/**
 *  @file  CID_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef CID_HANDLER_H
#define CID_HANDLER_H

#include /**/ "ace/pre.h"
                                                                                
#include "ciao/DeploymentC.h"
#include "ccd.hpp"
#include "Basic_Deployment_Data.hpp"
             
                                                                                
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO{
	
  namespace Config_Handlers{


   /*
    * @class CID_Handler
    * 
    * @brief Handler class for <ComponentInterfaceDescription> types.
    * 
    * This class defines handler methods to map values from 
    * XSC objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL type for the schema element.
    * 
    */
    
    class CID_Handler{
     
      public:
		
      CID_Handler();
	  virtual ~CID_Handler();
	
      ///This method maps the values from the
      ///XSC object <ComponentPortDescription> to
      ///the CORBA IDL type <Deployment::ComponentPortDescription>.   
      ///It handles the creation of the Deployment::ComponentPort's,
      ///Deployment::ComponentPortDescriptions's, and 
      ///Deployment::Properties and delegates mapping the values
      ///from their corresponding XSC objects to their handlers.     
      Deployment::ComponentInterfaceDescription&
      get_ComponentInterfaceDescription(
		  Deployment::ComponentInterfaceDescription& toconfig,
          ComponentInterfaceDescription& desc);
			
    };

  }
	
}
#include /**/ "ace/post.h"
#endif /* CID_HANDLER_H */
