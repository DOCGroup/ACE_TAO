//==================================================================
/**
 *  @file  Any_Handler.h
 *
 *  $Id$
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef ANY_HANDLER_H
#define ANY_HANDLER_H
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
     * @class Any_Handler
     *
     * @brief Handler class for <Deployment:Any> type
     *
     * This class defines handler methods to parse Implementation
     * Dependency types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export Any_Handler
    {
    public:
      static void process_Any (DOMNodeIterator * iter,
                               ::CORBA::Any &any_obj);
      // process elements of type Deployment:Any in the descriptor files
    };
  }
}

#include /**/ "ace/post.h"

#endif /* ID_HANDLER_H */
