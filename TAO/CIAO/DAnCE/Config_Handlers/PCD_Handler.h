//==============================================================
/**
 *  @file  PCD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_PCD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PCD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct PlanConnectionDescription; 
}

namespace CIAO
{

  namespace Config_Handlers
  {
    
   struct PlanConnectionDescription;
   
   /*
    * @class PCD_Handler
    *
    * @brief Handler class for <PlanConnectionDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC PlanConnectionDescription objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */
    
    class Config_Handlers_Export PCD_Handler{
     
      public:
       
        PCD_Handler (void);
        virtual ~PCD_Handler (void);
  
        ///This method takes a <Deployment::PlanConnectionDescription>
        ///and maps the values from the passed in XSC 
        ///PlanConnectionDescription to its members.
        void get_PlanConnectionDescription (
                    Deployment::PlanConnectionDescription& toconfig,
                    PlanConnectionDescription& desc);          

	//This method takes a <Deployment::PlanConnectionDescription>
	//converts it into a <Config_Handler::PlanConnectionDescription>
	//and returns the value
	static PlanConnectionDescription get_PlanConnectionDescription (
	    const Deployment::PlanConnectionDescription &src);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_PCD_HANDLER_H*/
