//==============================================================
/**
 *  @file  CRDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_CRDD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CRDD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ConnectionResourceDeploymentDescription;
}

namespace DAnCE
{

  namespace Config_Handlers
  {

    class ConnectionResourceDeploymentDescription;

    /*
     * @class CRDD_Handler
     *
     * @brief Handler class for <ConnectionResourceDeploymentDescription> types.
     *
     * This class defines handler methods to map values from
     * XSC ConnectionResourceDeploymentDescription objects, parsed from
     * the descriptor files, to the corresponding CORBA IDL type.
     *
     */

    class Config_Handlers_Export CRDD_Handler{

    public:

      CRDD_Handler (void);
      virtual ~CRDD_Handler (void);

      ///This method takes a <Deployment::ConnectionResourceDeploymentDescription>
      ///and maps the values from the passed in XSC
      ///ConnectionResourceDeploymentDescription to its members.
      void get_ConnectionResourceDeploymentDescription (Deployment::ConnectionResourceDeploymentDescription& toconfig,
                                                        const ConnectionResourceDeploymentDescription& desc);

      static ConnectionResourceDeploymentDescription
      connection_resource_depl_desc (
                                     const ::Deployment::ConnectionResourceDeploymentDescription& src);

    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CRDD_HANDLER_H*/
