//==============================================================
/**
 *  @file  CEPE_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_CEPE_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CEPE_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Common_Export.h"
#include "Utils/Functors.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  class ComponentExternalPortEndpoints;
  struct ComponentExternalPortEndpoint;
}

namespace DAnCE
{
  namespace Config_Handlers
    {
      class PlanConnectionDescription;
      class ComponentExternalPortEndpoint;

      /*
       * @class CEPE_Handler
       *
       * @brief Handler class for <ComponentExternalPortEndpoint> types.
       *
       * This class defines handler methods to map values from
       * XSC ComponentExternalPortEndpoint objects, parsed from
       * the descriptor files, to the corresponding CORBA IDL type.
       *
       */

      class Config_Handlers_Common_Export CEPE_Handler
    {
    public:
      static void external_port_endpoints (const PlanConnectionDescription &src,
                           ::Deployment::ComponentExternalPortEndpoints &dest);

      static ComponentExternalPortEndpoint
        external_port_endpoint (const ::Deployment::ComponentExternalPortEndpoint &src);

      static void handle_external_port_endpoint (const ComponentExternalPortEndpoint &src,
                             ::Deployment::ComponentExternalPortEndpoint &dest);
    };

      typedef Sequence_Handler < ComponentExternalPortEndpoint,
    ::Deployment::ComponentExternalPortEndpoints,
    ::Deployment::ComponentExternalPortEndpoint,
    CEPE_Handler::handle_external_port_endpoint > CEPE_Functor;
    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CEPE_HANDLER_H*/
