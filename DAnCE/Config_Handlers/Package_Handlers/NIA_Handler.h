/**
 * @file NIA_Handler.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 * $Id$
 */

#ifndef CIAO_PACKAGING_NIA_HANDLER_H
#define CIAO_PACKAGING_NIA_HANDLER_H

#include /**/ "ace/pre.h"
#include "Package_Handlers/Packaging_Handlers_Export.h"
#include "Utils/Functors.h"
#include "DAnCE/Deployment/Deployment_Packaging_DataC.h"
#include "iad.hpp"

namespace Deployment
{
  struct NamedImplementationArtifact;
  class NamedImplementationArtifacts;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class NamedImplementationArtifact;

    namespace Packaging
    {
      /**
       * @class NIA_Handler
       * @brief Handler class for NamedImplementationArtifact types.
       */
      class Packaging_Handlers_Export NIA_Handler
      {
      public:
        static void handle_nia (const NamedImplementationArtifact &desc,
                             ::Deployment::NamedImplementationArtifact &toconfig)
        {
          toconfig.name = ACE_TEXT_ALWAYS_CHAR (desc.name ().c_str ());
          IAD_Handler::impl_artifact_descr (desc.referencedArtifact (),
                                            toconfig.referencedArtifact);
        }


        static NamedImplementationArtifact
        get_nia (const ::Deployment::NamedImplementationArtifact &src)
        {
          return NamedImplementationArtifact (ACE_TEXT_CHAR_TO_TCHAR (src.name.in ()),
                                              IAD_Handler::impl_artifact_descr (src.referencedArtifact));
        }

      };

      typedef Sequence_Handler < NamedImplementationArtifact,
                                 ::Deployment::NamedImplementationArtifacts,
                                 ::Deployment::NamedImplementationArtifact,
        NIA_Handler::handle_nia > NIA_Functor;
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_PACKAGING_NIA_HANDLER_H */
