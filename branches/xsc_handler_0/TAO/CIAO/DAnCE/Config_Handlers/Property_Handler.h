
      //==============================================================
/**
 *  @file  Property_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_Property_Handler_H
#define CIAO_CONFIG_HANDLERS_Property_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class Property;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class Property;


   /*
    * @class Property_Handler
    *
    * @brief Handler class for <Property> types.
    *
    * This class defines handler methods to map values from
    * XSC Property objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export Property_Handler {
     
      public:

        Property_Handler (void);
        virtual ~Property_Handler (void);

        static void property (
             const Property& desc,
             Deployment::Property& toconfig);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_Property_Handler_H */

