//==============================================================
/**
 *  @file  ComponentPropertyDescription_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ComponentPropertyDescription_Handler_H
#define CIAO_CONFIG_HANDLERS_ComponentPropertyDescription_Handler_H
#include /**/ "ace/pre.h"


#include "Basic_Deployment_Data.hpp"
#include "Config_Handlers_export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class ComponentPropertyDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class ComponentPropertyDescription;


   /*
    * @class ComponentPropertyDescription_Handler
    *
    * @brief Handler class for <ComponentPropertyDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC ComponentPropertyDescription objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export ComponentPropertyDescription_Handler{
     
      public:

        ComponentPropertyDescription_Handler (void);
        virtual ~ComponentPropertyDescription_Handler (void);

        void get_ComponentPropertyDescription (
             Deployment::ComponentPropertyDescription& toconfig, 
             ComponentPropertyDescription& desc);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_ComponentPropertyDescription_Handler_H */

