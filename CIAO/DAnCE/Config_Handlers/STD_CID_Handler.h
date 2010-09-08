//================================================
/**
 *  @file  STD_CID_Handler.h
 *
 *  $Id$
 *
 *  @author Ming Xiong <mxiong@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_STD_CID_HANDLER_H
#define CIAO_CONFIG_HANDLERS_STD_CID_HANDLER_H

#include /**/ "ace/pre.h"

#include "IDREF_Base.h"
#include "Config_Handlers_Export.h"
#include "DAnCE/Deployment/DeploymentC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace DAnCE
{
  namespace Config_Handlers
  {
    class ComponentImplementationDescription;

    /*
     * @class STD_CID_Handler
     *
     * @brief Handler class for <CCMComponentImplementationDescription> types.
     *
     * This class defines handler methods to map values from
     * XSC objects, parsed from the descriptor files, to the
     * corresponding CORBA IDL type for the schema element.
     *
     */
    class Config_Handlers_Export STD_CID_Handler
    {

    public:

      STD_CID_Handler (void)
      {
      }

      /// Maps the values from the XSC object
      /// <ComponentImplementationDescription> to the CORBA IDL type
      /// <Deployment::ComponentImplementationDescription>.
      bool component_impl_descr (
            const ComponentImplementationDescription &desc,
            ::Deployment::ComponentImplementationDescription &toconfig);

      static IDREF_Base<CORBA::ULong> IDREF;

//@@Note: This part has yet to be added in the future
//    ComponentImplementationDescription
//    component_port_description (
//      const Deployment::ComponentImplementationDescription& src);

    private:
      // disable the default constructor
      STD_CID_Handler ()
        :xml_helper_ (0)
      {
      }
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_STD_CID_HANDLER_H*/
