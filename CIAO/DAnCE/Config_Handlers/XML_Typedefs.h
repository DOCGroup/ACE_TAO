/**
 * @file XML_Typedefs.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * $Id$
 * Typedefs from the XML Utilities.
 */
#ifndef XML_TYPEDEFS_H
#define XML_TYPEDEFS_H

#include "XSC_XML_Handlers_Export.h"
#include "XML/XML_Helper.h"
#include "XML/XML_Schema_Resolver.h"
#include "XML/XML_Error_Handler.h"

namespace CIAO
{
  namespace Config_Handlers
  {
    class XSC_XML_Handlers_Export XML_Helper
    {
    public:
      typedef ::CIAO::XML::Environment_Resolver PATH_RESOLVER;
      typedef ::CIAO::XML::XML_Schema_Resolver< ::CIAO::XML::Environment_Resolver > XML_RESOLVER;
      typedef ::CIAO::XML::XML_Error_Handler ERROR_HANDLER;
      typedef ::CIAO::XML::XML_Helper< XML_RESOLVER, ERROR_HANDLER > HELPER;
      
      static ERROR_HANDLER _xml_error_handler;
      
    public:
      static HELPER XML_HELPER;
    };
  }
}


#endif /* XML_TYPEDEFS_H */
