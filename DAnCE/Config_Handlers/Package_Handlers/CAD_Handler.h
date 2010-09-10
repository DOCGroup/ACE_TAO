/**
 * @file CAD_Handler.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */

#ifndef CIAO_PACKAGING_CAD_HANDLER_H
#define CIAO_PACKAGING_CAD_HANDLER_H

#include /**/ "ace/pre.h"

#include "XML_Typedefs.h"
#include "DAnCE/Deployment/DeploymentC.h"
#include "Package_Handlers/Packaging_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ComponentAssemblyDescription;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class ComponentAssemblyDescription;

    namespace Packaging
    {

      /**
       * @class CAD_Handler
       *
       * @brief Handler class for ComponentAssemblyDescription.
       */
      class Packaging_Handlers_Export CAD_Handler
      {
      public:
        static void component_assem_descr (const ComponentAssemblyDescription &desc,
                                           ::Deployment::ComponentAssemblyDescription &toconfig);

        static ComponentAssemblyDescription
        component_assem_descr (const ::Deployment::ComponentAssemblyDescription &src);

      private:
        static ComponentAssemblyDescription * resolve_cad (const char *uri);
      };

    }
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_PACKAGING_CAD_HANDLER_H */

