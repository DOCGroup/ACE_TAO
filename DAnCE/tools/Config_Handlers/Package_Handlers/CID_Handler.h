/**
 *  @file  CID_Handler.h
 *
 *  $Id$
 *
 *  @author William Otte <wotte@dre.vanderbilt.edu
 */

#ifndef CIAO_PACKAGING_CID_HANDLER_H
#define CIAO_PACKAGING_CID_HANDLER_H

#include /**/ "ace/pre.h"

#include "IDREF_Base.h"
#include "Package_Handlers/Packaging_Handlers_Export.h"
#include "tao/Basic_Types.h"

namespace Deployment
{
  struct ComponentImplementationDescription;
}


namespace DAnCE
{
  namespace Config_Handlers
  {
    class ComponentImplementationDescription;

    namespace Packaging
    {

      /**
       * @class CID_Handler
       *
       * @brief Handler class for <CCMComponentImplementationDescription> types.
       *
       * This class defines handler methods to map values from
       * XSC objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL type for the schema element.
       *
       */
      class Packaging_Handlers_Export CID_Handler
      {

      public:
        /// Maps the values from the XSC object
        /// <ComponentImplementationDescription> to the CORBA IDL type
        /// <Deployment::ComponentImplementationDescription>.
        static void component_impl_descr (const ComponentImplementationDescription &desc,
                                          ::Deployment::ComponentImplementationDescription &toconfig);

        static IDREF_Base<CORBA::ULong> IDREF;

        static ComponentImplementationDescription
        component_impl_descr (const Deployment::ComponentImplementationDescription& src);

      private:

        static ComponentImplementationDescription * resolve_cid (const ACE_TCHAR *uri);
      };
    }
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_PACKAGING_CID_HANDLER_H*/
