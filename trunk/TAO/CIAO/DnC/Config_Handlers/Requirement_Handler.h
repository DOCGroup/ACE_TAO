//==================================================================
/**
 *  @file  Requirement_Handler.h
 *
 *  $Id$ 
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef REQUIREMENT_HANDLER_H
#define REQUIREMENT_HANDLER_H
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
     * @class Requirement_Handler
     *
     * @brief Handler class for <ImplementationDependency> type
     *
     * This class defines handler methods to parse Implementation
     * Dependency types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export Requirement_Handler
    {
    public:
      static Deployment::Requirement *
        process_Requirement (DOMNodeIterator * iter);
      // process elements of type Requirement in the descriptor files

    };
  }

}

#include /**/ "ace/post.h"

#endif /* REQUIREMENT_HANDLER_H */
