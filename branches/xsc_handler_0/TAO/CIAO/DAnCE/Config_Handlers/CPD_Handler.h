//================================================
/**
 *  @file  CPD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_CPD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_CPD_HANDLER_H

#include /**/ "ace/pre.h"

#include "ciao/DeploymentC.h"
#include "Basic_Deployment_Data.hpp"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO{

  namespace Config_Handlers{

  /*
   * @class CPD_Handler
   *
   * @brief Handler class for <CCMComponentPortDescription> types.
   *
   * This class defines handler methods to map values from
   * XSC objects, parsed from the descriptor files, to the
   * corresponding CORBA IDL type for the schema element.
   *
   */

   class CPD_Handler
   {

    public:

      /// Maps the values from the XSC object
      /// <ComponentInterfaceDescription> to the CORBA IDL type
      /// <Deployment::ComponentInterfaceDescription>.
     static void component_port_description (
       ::Deployment::ComponentPortDescription& toconfig,
       ComponentPortDescription& desc);
   };

 }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_CPD_HANDLER_H*/
