//================================================
/**
 *  @file  CPD_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_STD_CPD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_STD_CPD_HANDLER_H

#include /**/ "ace/pre.h"

#include <memory>
#include "XML_Typedefs.h"
#include "Utils/Functors.h"
#include "Package_Handlers/Packaging_Handlers_Export.h"

namespace Deployment
{
  struct ComponentPackageDescription;
  class ComponentPackageDescriptions;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class ComponentPackageDescription;

    namespace Packaging
    {

      /*
       * @class CPD_Handler
       *
       * @brief Handler class for <CCMComponentPackageDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL type for the schema element.
       *
       */
      class Packaging_Handlers_Export CPD_Handler
      {

      public:
        /// Maps the values from the XSC object
        /// <ComponentPackageDescription> to the CORBA IDL type
        /// <Deployment::ComponentPackageDescription>.
        static void handle_component_package_descr (const ComponentPackageDescription &desc,
                                             ::Deployment::ComponentPackageDescription &toconfig);

        static ComponentPackageDescription
      component_package_descr (const Deployment::ComponentPackageDescription& src);
      private:
        static ComponentPackageDescription * resolve_cpd (const ACE_TCHAR *uri);

      };

      typedef Sequence_Handler < ComponentPackageDescription,
                                 ::Deployment::ComponentPackageDescriptions,
                                 ::Deployment::ComponentPackageDescription,
                                 CPD_Handler::handle_component_package_descr > CPD_Functor;

    }

  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CPD_HANDLER_H*/
