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

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ComponentExternalPortEndpoint;
}

namespace CIAO
{
  namespace Config_Handlers
  {

    struct ComponentExternalPortEndpoint;

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

    class Config_Handlers_Export CEPE_Handler
    {
    public:
      CEPE_Handler (void);
      virtual ~CEPE_Handler (void);

      /// This method takes a
      /// <Deployment::ComponentExternalPortEndpoint> and maps the
      /// values from the passed in XSC  ComponentExternalPortEndpoint
      /// to its members.
      void get_ComponentExternalPortEndpoint (
                Deployment::ComponentExternalPortEndpoint& toconfig,
                ComponentExternalPortEndpoint& desc);

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CEPE_HANDLER_H*/
