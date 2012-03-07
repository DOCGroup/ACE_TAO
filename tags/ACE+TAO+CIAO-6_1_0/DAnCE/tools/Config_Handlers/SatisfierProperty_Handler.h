//================================================
/**
 *  @file  SatisfierProperty_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_SATIS_PROPERTY_HANDLER_H
#define CIAO_CONFIG_HANDLERS_SATIS_PROPERTY_HANDLER_H

#include /**/ "ace/pre.h"

#include "Config_Handlers_Common_Export.h"
#include "Utils/Functors.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct SatisfierProperty;
  class SatisfierProperties;
}

namespace DAnCE
{
  namespace Config_Handlers
    {
      class SatisfierProperty;

      /*
       * @class SatisfierProperty_Handler
       *
       * @brief Handler class for <CCMComponentPortDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL type for the schema element.
       *
       */

      class Config_Handlers_Common_Export SatisfierProperty_Handler
    {
    public:
      SatisfierProperty_Handler (void);
      virtual ~SatisfierProperty_Handler (void);

      /// This method maps the values from the XSC object
      /// <DAnCE::Config_Handlers::SatisfierProperty> to the CORBA IDL type
      /// <Deployment::SatisfierProperty>.
      static void handle_sat_property (const SatisfierProperty& desc,
                       ::Deployment::SatisfierProperty& toconfig);
      static SatisfierProperty get_sat_property (const ::Deployment::SatisfierProperty& src);
    };

      typedef Sequence_Handler < SatisfierProperty,
    ::Deployment::SatisfierProperties,
    ::Deployment::SatisfierProperty,
    SatisfierProperty_Handler::handle_sat_property> SatisfierProperty_Functor;

    }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_SATIS_PROPERTY_HANDLER_H*/
