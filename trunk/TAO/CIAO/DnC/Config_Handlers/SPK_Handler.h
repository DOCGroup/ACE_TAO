//==================================================================
/**
 *  @file  SPK_Handler.h
 *
 *  $Id$ 
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef SPK_HANDLER_H
#define SPK_HANDLER_H
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
     * @class SPK_Handler
     *
     * @brief Handler class for <SatisfierPropertyKind> type
     *
     * This class defines handler methods to parse Implementation
     * Dependency types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export SPK_Handler
    {
    public:
      static Deployment::SatisfierPropertyKind 
        process_SatisfierPropertyKind (DOMNodeIterator * iter);
      // process elements of type SatisfierPropertyKind definitions in
      // the descriptor files

    };
  }

}

#include /**/ "ace/post.h"

#endif /* SPK_HANDLER_H */
