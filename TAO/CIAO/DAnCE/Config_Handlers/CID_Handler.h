//================================================
/**
 *  @file  CID_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_CID_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CID_HANDLER_H

#include /**/ "ace/pre.h"

#include "cid.hpp"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  namespace ComponentInterfaceDescription;
}

namespace CIAO
{
  namespace Config_Handlers
  {
   /*
    * @class CID_Handler
    *
    * @brief Handler class for <ComponentInterfaceDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL type for the schema element.
    *
    */
    class CID_Handler
    {
      public:

      CID_Handler (void);
      virtual ~CID_Handler(void);

      /// This method maps the values from the XSC object
      /// <ComponentPortDescription> to the CORBA IDL type
      /// <Deployment::ComponentPortDescription>. It handles the
      /// creation of the Deployment::ComponentPort's,
      /// Deployment::ComponentPortDescriptions's, and
      /// Deployment::Properties and delegates mapping the values from
      /// their corresponding XSC objects to their handlers.
      bool  get_component_interface_descr (
          Deployment::ComponentInterfaceDescription& toconfig,
          ComponentInterfaceDescription& desc);
    };

  }

}

#include /**/ "ace/post.h"
#endif /* CID_HANDLER_H */
