
      //==============================================================
/**
 *  @file  CPD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_CPD_Handler_H
#define CIAO_CONFIG_HANDLERS_CPD_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class ComponentPortDescription;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class ComponentPortDescription;


   /*
    * @class CPD_Handler
    *
    * @brief Handler class for <ComponentPortDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC ComponentPortDescription objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    
    class Config_Handlers_Export CPD_Handler {
     
      public:

        CPD_Handler (void);
        virtual ~CPD_Handler (void);

        static void comp_port_descr (
             const ComponentPortDescription& desc,
             Deployment::ComponentPortDescription& toconfig);

    };
  }
}

#include /**/ "ace/post.h" 
#endif /* CIAO_CONFIG_HANDLERS_CPD_Handler_H */

