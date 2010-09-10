/**
 * @file Comp_Intf_Descr_Handler.h
 * @author William Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 */

#ifndef CIAO_PACKAGING_COMP_INTF_DESCR_HANDLER_H
#define CIAO_PACKAGING_COMP_INTF_DESCR_HANDLER_H

#include /**/ "ace/pre.h"

#include "XML_Typedefs.h"
#include "DAnCE/Deployment/Deployment_Packaging_DataC.h"
#include "Package_Handlers/Packaging_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace DAnCE
{
  namespace Config_Handlers
  {
    class ComponentInterfaceDescription;

    namespace Packaging
    {

      /**
       * @class Comp_Intf_Descr_Handler
       *
       * @brief Handler class for ComponentInterfaceDescription.
       */
      class Packaging_Handlers_Export Comp_Intf_Descr_Handler
      {
      public:
        static void comp_intf_descr (const ComponentInterfaceDescription &descr,
                                     ::Deployment::ComponentInterfaceDescription &toconfig);

        static ComponentInterfaceDescription
        comp_intf_descr (const ::Deployment::ComponentInterfaceDescription &src);

      private:
        static ComponentInterfaceDescription *resolve_cid (const ACE_TCHAR *uri);
      };
    }
  }
}

#include /**/ "ace/post.h"

#endif /* CIAO_PACKAGING_COMP_INTF_DESCR_HANDLER_H */
