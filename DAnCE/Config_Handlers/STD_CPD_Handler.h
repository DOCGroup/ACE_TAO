//================================================
/**
 *  @file  STD_CPD_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_STD_CPD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_STD_CPD_HANDLER_H

#include /**/ "ace/pre.h"

#include "Utils/XML_Helper.h"
#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct ComponentPackageDescription;
}

namespace DAnCE
{
  namespace Config_Handlers
  {
    class ComponentPackageDescription;

    /*
     * @class STD_CPD_Handler
     *
     * @brief Handler class for <CCMComponentPackageDescription> types.
     *
     * This class defines handler methods to map values from
     * XSC objects, parsed from the descriptor files, to the
     * corresponding CORBA IDL type for the schema element.
     *
     */
    class Config_Handlers_Export STD_CPD_Handler
    {

    public:

      STD_CPD_Handler (XML_Helper *xml_helper)
        :xml_helper_ (xml_helper)
      {
      }

      /// Maps the values from the XSC object
      /// <ComponentPackageDescription> to the CORBA IDL type
      /// <Deployment::ComponentPackageDescription>.
     bool component_package_descr(
            const ComponentPackageDescription &desc,
            ::Deployment::ComponentPackageDescription &toconfig);

      //@@Note: This part has yet to be added in the future
      //     ComponentPortDescription
      //     component_package_descr (
      //         const Deployment::ComponentPortDescription& src);
    private:
      // disable the default constructor
      STD_CPD_Handler ()
        :xml_helper_ (0)
      {
      }
      
      XML_Helper *xml_helper_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CPD_HANDLER_H*/
