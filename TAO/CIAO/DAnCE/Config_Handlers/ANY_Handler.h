//==============================================================
/**
 *  @file  ANY_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ANY_HANDLER_H
#define CIAO_CONFIG_HANDLERS_ANY_HANDLER_H
#include /**/ "ace/pre.h"

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Basic_Deployment_Data.hpp"

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

        ANY_Handler (void);
        virtual ~ANY_Handler (void);

        static void get_Any (CORBA::Any& toconfig, Any& desc);

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ANY_HANDLER_H*/
