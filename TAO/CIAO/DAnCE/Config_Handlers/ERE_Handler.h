//==============================================================
/**
 *  @file  ERE_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ERE_HANDLER_H
#define CIAO_CONFIG_HANDLERS_ERE_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ExternalReferenceEndpoint; 
}

namespace CIAO
{

  namespace Config_Handlers
  {
    
   struct ExternalReferenceEndpoint;
   
   /*
    * @class ERE_Handler
    *
    * @brief Handler class for <ExternalReferenceEndpoint> types.
    *
    * This class defines handler methods to map values from
    * XSC ExternalReferenceEndpoint objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */
    
    class Config_Handlers_Export ERE_Handler{
     
      public:
       
        ERE_Handler (void);
        virtual ~ERE_Handler (void);
  
        ///This method takes a <Deployment::ExternalReferenceEndpoint>
        ///and maps the values from the passed in XSC 
        ///ExternalReferenceEndpoint to its members.
        void get_ExternalReferenceEndpoint (
                    Deployment::ExternalReferenceEndpoint& toconfig,
                    ExternalReferenceEndpoint& desc);          

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ERE_HANDLER_H*/
