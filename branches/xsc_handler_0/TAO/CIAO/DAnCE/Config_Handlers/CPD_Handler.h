//================================================
/**
 *  @file  CPD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef CPD_HANDLER_H
#define CPD_HANDLER_H

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

      CPD_Handler (void);
      virtual ~CPD_Handler (void);

      ///This method maps the values from the
      ///XSC object <ComponentInterfaceDescription> to
      ///the CORBA IDL type <Deployment::ComponentInterfaceDescription>.
      void
      get_ComponentPortDescription (
        Deployment::ComponentPortDescription& toconfig,
        ComponentPortDescription& desc);
       };

 }
}

#include /**/ "ace/post.h"
#endif /* CPD_HANDLER_H */
