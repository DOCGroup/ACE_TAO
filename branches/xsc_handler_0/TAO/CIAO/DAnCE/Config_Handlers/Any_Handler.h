//==============================================================
/**
 *  @file  Any_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ANY_HANDLER_H
#define CIAO_CONFIG_HANDLERS_ANY_HANDLER_H
#include /**/ "ace/pre.h"


#include "Basic_Deployment_Data.hpp"
#include "Config_Handlers_export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace CORBA
{
  class Any;
}

// @@ Jules, can you please change the name of this file?
namespace CIAO
{

  namespace Config_Handlers
  {
   /*
    * @class Any_Handler
    *
    * @brief Handler class for <ComponentInterfaceDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC Any objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export Any_Handler{
     
      public:

        Any_Handler (void);
        virtual ~Any_Handler (void);

        static void get_Any (CORBA::Any& toconfig, Any& desc);

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ANY_HANDLER_H*/
