
      //==============================================================
/**
 *  @file  Req_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_Req_Handler_H
#define CIAO_CONFIG_HANDLERS_Req_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class Requirement;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class Requirement;


   /*
    * @class Req_Handler
    *
    * @brief Handler class for <Requirement> types.
    *
    * This class defines handler methods to map values from
    * XSC Requirement objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export Req_Handler {
     
      public:

        Req_Handler (void);
        virtual ~Req_Handler (void);

        static void requirement (
             const Requirement& desc,
             Deployment::Requirement& toconfig);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_Req_Handler_H */

