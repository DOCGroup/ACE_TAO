//==================================================================
/**
 *  @file  Property_Handler.h
 *
 *  $Id$
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//===================================================================

#ifndef PROPERTY_HANDLER_H
#define PROPERTY_HANDLER_H
#include /**/ "ace/pre.h"

#include "ciao/DeploymentC.h"

#include "Config_Handler_export.h"
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include "XercesString.h"
using Config_Handler::XStr;
using xercesc::XMLString;
using xercesc::DOMText;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;

namespace CIAO
{
  namespace Config_Handler
  {
    /**
     * @class Property_Handler
     *
     * @brief Handler class for <Property> type
     *
     * This class defines handler methods to parse Property
     * types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export Property_Handler
    {
    public:
      static void process_Property (DOMNodeIterator * iter,
                                    Deployment::Property &property);
      // process elements of type Property definitions in
      // the descriptor files

    };
  }
}

#include /**/ "ace/post.h"

#endif /* PROPERTY_HANDLER_H */
