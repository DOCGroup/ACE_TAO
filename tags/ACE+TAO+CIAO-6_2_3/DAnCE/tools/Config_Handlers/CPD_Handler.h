//================================================
/**
 *  @file  CPD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_CPD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CPD_HANDLER_H

#include /**/ "ace/pre.h"

#include "Config_Handlers_Common_Export.h"
#include "Utils/Functors.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ComponentPortDescription;
  class ComponentPortDescriptions;
}

namespace DAnCE
{
  namespace Config_Handlers
    {
      class ComponentPortDescription;
      /*
       * @class CPD_Handler
       *
       * @brief Handler class for <CCMComponentPortDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL type for the schema element.
       *
       */

      class Config_Handlers_Common_Export CPD_Handler
    {

    public:
      /// Maps the values from the XSC object
      /// <ComponentInterfaceDescription> to the CORBA IDL type
      /// <Deployment::ComponentInterfaceDescription>.
      static void handle_component_port_description (const ComponentPortDescription &desc,
                             ::Deployment::ComponentPortDescription &toconfig);

      static ComponentPortDescription
        component_port_description (const Deployment::ComponentPortDescription& src);
    };

      typedef Sequence_Handler < ComponentPortDescription,
                 ::Deployment::ComponentPortDescriptions,
                 ::Deployment::ComponentPortDescription,
                 CPD_Handler::handle_component_port_description > CPD_Functor;

    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CPD_HANDLER_H*/
