//================================================
/**
 *  @file  IAD_Handler.h
 *
 *  $Id$
 *
 *  @author William Otte  <wotte@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_STD_IAD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_STD_IAD_HANDLER_H

#include /**/ "ace/pre.h"

#include "Package_Handlers/Packaging_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ImplementationArtifactDescription;
  struct ImplementationArtifactDescriptions;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class ImplementationArtifactDescription;
    namespace Packaging
    {
      /*
       * @class IAD_Handler
       *
       * @brief Handler class for <CCMImplementationArtifactDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL type for the schema element.
       *
       */
      class Packaging_Handlers_Export IAD_Handler
      {
      public:
        /// Maps the values from the XSC object
        /// <ImplementationArtifactDescription> to the CORBA IDL type
        /// <Deployment::ImplementationArtifactDescription>.
        static void
        impl_artifact_descr (const ImplementationArtifactDescription &desc,
                             ::Deployment::ImplementationArtifactDescription &toconfig);


        static ImplementationArtifactDescription
        impl_artifact_descr (const Deployment::ImplementationArtifactDescription& src);

        static ImplementationArtifactDescription * resolve_iad (const ACE_TCHAR *uri);
      };
    }
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_STD_IAD_HANDLER_H*/
