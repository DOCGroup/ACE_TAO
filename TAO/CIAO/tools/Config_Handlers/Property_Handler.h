//================================================
/**
 *  @file  Property_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================

#ifndef CIAO_CONFIG_HANDLERS_PROPERTY_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PROPERTY_HANDLER_H

#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct Property;
}

namespace CIAO
{
  namespace Config_Handlers
  {
    class Property;

    /*
     * @class Property_Handler
     *
     * @brief Handler class for <CCMComponentPortDescription> types.
     *
     * This class defines handler methods to map values from
     * XSC objects, parsed from the descriptor files, to the
     * corresponding CORBA IDL type for the schema element.
     *
     */

   class Config_Handlers_Export Property_Handler
   {
   public:
     Property_Handler (void);
     virtual ~Property_Handler (void);

     /// This method maps the values from the XSC object
     /// <CIAO::Config_Handlers::Property> to the CORBA IDL type
     /// <Deployment::Property>.
     static void get_property (const Property& desc,
       ::Deployment::Property& toconfig);
     static Property get_property (
       const ::Deployment::Property& src);
   };

 }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_PROPERTY_HANDLER_H*/
