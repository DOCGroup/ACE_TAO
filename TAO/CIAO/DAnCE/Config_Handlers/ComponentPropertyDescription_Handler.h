//================================================
/**
 *  @file  ComponentPropertyDescription_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef COMPONENTPROPERTYDESCRIPTION_HANDLER_H
#define COMPONENTPROPERTYDESCRIPTION_HANDLER_H

#include /**/ "ace/pre.h"

#include "ciao/DeploymentC.h"
#include "Basic_Deployment_Data.hpp"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace CIAO{

  namespace Config_Handlers{

  /*
   * @class ComponentPropertyDescription_Handler
   *
   * @brief Handler class for <ComponentPortDescription> types.
   *
   * This class defines handler methods to map values from
   * XSC objects, parsed from the descriptor files, to the
   * corresponding CORBA IDL type for the schema element.
   *
   */

    // @@ Jules, all of the classes require the export macro to be usable.

   class ComponentPropertyDescription_Handler{

    public:

      ComponentPropertyDescription_Handler (void);
      virtual ~ComponentPropertyDescription_Handler (void);

      ///This method maps the values from the
      ///XSC object <ComponentPropertyDescription> to
      ///the CORBA IDL type <Deployment::ComponentPropertyDescription>.
      void
      get_ComponentPropertyDescription (
          Deployment::ComponentPropertyDescription& toconfig,
          ComponentPropertyDescription& desc);
   };

 }
}

#include /**/ "ace/post.h"
#endif /* COMPONENTPROPERTYDESCRIPTION_HANDLER_H */
