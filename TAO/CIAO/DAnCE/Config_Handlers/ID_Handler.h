//==================================================================
/**
 *  @file  ID_Handler.h
 *
 *  $Id$
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef ID_HANDLER_H
#define ID_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
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
     * @class ID_Handler
     *
     * @brief Handler class for <ImplementationDependency> type
     *
     * This class defines handler methods to parse Implementation
     * Dependency types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     * KNOWN ISSUE:
     * The IDL mapping for this type defines a struct containing a
     * string type. However, the schema defines an unbounded string.
     * Currently, this Handler handles what the IDL requires as this is
     * valid via the schema definition also.
     *
     */
    class Config_Handler_Export ID_Handler
    {
    public:
      static void
        process_ImplementationDependency (DOMNodeIterator * iter,
                                          Deployment::ImplementationDependency &ret_struct);
      // process elements of type Implementation Dependency definitions in
      // the descriptor files

    };
  }

}

#include /**/ "ace/post.h"

#endif /* ID_HANDLER_H */
