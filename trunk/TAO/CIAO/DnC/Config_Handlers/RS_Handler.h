//==================================================================
/**
 *  @file  RS_Handler.h
 *
 *  $Id$
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef RS_HANDLER_H
#define RS_HANDLER_H
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
     * @class RS_Handler
     *
     * @brief Handler class for <RequirementSatisfier> type
     *
     * This class defines handler methods to parse RequirementSatisfier
     * types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export RS_Handler
    {
    public:
      static Deployment::RequirementSatisfier *
        process_RequirementSatisfier (DOMNodeIterator * iter);
      // process elements of type RequirementSatisfier definitions in
      // the descriptor files

    };
  }
}

#include /**/ "ace/post.h"

#endif /* ID_HANDLER_H */
