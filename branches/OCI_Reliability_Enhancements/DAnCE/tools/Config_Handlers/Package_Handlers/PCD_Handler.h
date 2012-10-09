//================================================
/**
 *  @file  PCD_Handler.h
 *
 *  $Id$
 *
 *  @author William Otte <wotte@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_PCD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PCD_HANDLER_H

#include /**/ "ace/pre.h"

#include "Packaging_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct PackageConfiguration;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class PackageConfiguration;

    namespace Packaging
    {
      /*
       * @class PCD_Handler
       *
       * @brief Handler class for <CCMPackageConfiguration> types.
       *
       * This class defines handler methods to map values from
       * XSC objects, parsed from the descriptor files, to the
       * corresponding CORBA IDL type for the schema element.
       *
       */
      class Packaging_Handlers_Export PCD_Handler
      {

      public:
        static void package_config (const ACE_TCHAR *uri,
                                    ::Deployment::PackageConfiguration &toconfig);

        /// Maps the values from the XSC object
        /// <PackageConfiguration> to the CORBA IDL type
        /// <Deployment::PackageConfiguration>.
        static void package_config (const PackageConfiguration &desc,
                                    ::Deployment::PackageConfiguration &toconfig);

        static PackageConfiguration package_config (const Deployment::PackageConfiguration& src);

      private:
         static PackageConfiguration *resolve_package_config (const ACE_TCHAR *uri);
      };
    }

  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_STD_PCD_HANDLER_H*/
