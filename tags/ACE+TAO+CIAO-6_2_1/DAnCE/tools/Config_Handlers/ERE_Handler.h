//==============================================================
/**
 *  @file  ERE_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ERE_HANDLER_H
#define CIAO_CONFIG_HANDLERS_ERE_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Common_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Utils/Functors.h"

namespace Deployment
{
  class ExternalReferenceEndpoints;
  struct ExternalReferenceEndpoint;
}

namespace DAnCE
{

  namespace Config_Handlers
    {
      class PlanConnectionDescription;
      class ExternalReferenceEndpoint;

      /*
       * @class ERE_Handler
       *
       * @brief Handler class for <ExternalReferenceEndpoint> types.
       *
       * This class defines handler methods to map values from
       * XSC ExternalReferenceEndpoint objects, parsed from
       * the descriptor files, to the corresponding CORBA IDL type.
       *
       */

  class Config_Handlers_Common_Export ERE_Handler
    {
    public:
      static void external_ref_endpoints (const PlanConnectionDescription &src,
                          ::Deployment::ExternalReferenceEndpoints &dest);

      static ExternalReferenceEndpoint external_ref_endpoint (
                                  const Deployment::ExternalReferenceEndpoint& src);

      static void handle_external_ref_endpoint (const ExternalReferenceEndpoint &src,
                                                Deployment::ExternalReferenceEndpoint &dest);
    };

      typedef Sequence_Handler < ExternalReferenceEndpoint,
        ::Deployment::ExternalReferenceEndpoints,
        ::Deployment::ExternalReferenceEndpoint,
        ERE_Handler::handle_external_ref_endpoint > ERE_Functor;

    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ERE_HANDLER_H*/
