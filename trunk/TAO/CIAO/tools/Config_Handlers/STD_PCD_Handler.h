//================================================
/**
 *  @file  STD_PCD_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_STD_PCD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_STD_PCD_HANDLER_H

#include /**/ "ace/pre.h"
#include "Utils/XML_Helper.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct PackageConfiguration;
}

namespace CIAO
{
  namespace Config_Handlers
  {
    class PackageConfiguration;

    /*
     * @class STD_PCD_Handler
     *
     * @brief Handler class for <CCMPackageConfiguration> types.
     *
     * This class defines handler methods to map values from
     * XSC objects, parsed from the descriptor files, to the
     * corresponding CORBA IDL type for the schema element.
     *
     */
    class Config_Handlers_Export STD_PCD_Handler
    {

    public:
      STD_PCD_Handler (XML_Helper *xml_helper)
        :xml_helper_ (xml_helper)
      {
      }

      /// Maps the values from the XSC object
      /// <PackageConfiguration> to the CORBA IDL type
      /// <Deployment::PackageConfiguration>.
      bool package_config (
            const PackageConfiguration &desc,
            ::Deployment::PackageConfiguration &toconfig);

//@@Note: This part has yet to be added in the future
//     PackageConfiguration
//     package_config (
//       const Deployment::ComponentPortDescription& src);

    private:
      // Disable the default constructor
      STD_PCD_Handler ()
        :xml_helper_ (0)
      {
      }

      XML_Helper *xml_helper_;
    };
 }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_STD_PCD_HANDLER_H*/
