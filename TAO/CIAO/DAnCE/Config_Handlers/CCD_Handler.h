
      //==============================================================
/**
 *  @file  CCD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_CCD_Handler_H
#define CIAO_CONFIG_HANDLERS_CCD_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class ComponentInterfaceDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class ComponentInterfaceDescription;


   /*
    * @class CCD_Handler
    *
    * @brief Handler class for <ComponentInterfaceDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC ComponentInterfaceDescription objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export CCD_Handler {
     
      public:

        CCD_Handler (void);
        virtual ~CCD_Handler (void);

        static void comp_interface_descr (
             const ComponentInterfaceDescription& desc,
             Deployment::ComponentInterfaceDescription& toconfig);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_CCD_Handler_H */

