//==============================================================
/**
 *  @file  REQ_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_REQ_HANDLER_H
#define CIAO_CONFIG_HANDLERS_REQ_HANDLER_H
#include /**/ "ace/pre.h"

#include "Basic_Deployment_Data.hpp"
#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct Requirement; 
}

namespace CIAO
{

  namespace Config_Handlers
  {
   /*
    * @class REQ_Handler
    *
    * @brief Handler class for <Requirement> types.
    *
    * This class defines handler methods to map values from
    * XSC Requirement objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */
    
    class Config_Handlers_Export Req_Handler{
     
      public:
       
        Req_Handler (void);
        virtual ~Req_Handler (void);
  
        ///This method takes a <Deployment::Requirement>
        ///and maps the values from the passed in XSC 
        ///Requirement to its members.
        static void get_Requirement (
                    Deployment::Requirement& toconfig,
                    Requirement& desc);          
        static Requirement
        Req_Handler::get_requirement (
                         const Deployment::Requirement& src);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_REQ_HANDLER_H*/
