//==================================================================
/**
 *  @file  Cap_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef CAP_HANDLER_H
#define CAP_HANDLER_H
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
     * @class CAP_Handler
     *
     * @brief Handler class for <Capability> type
     *
     * This class defines handler methods to parse Capability
     * types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export CAP_Handler
    {
    public:
      static void
        process_Capability (DOMNodeIterator * iter,
			    Deployment::Capability &ret_struct);
      // process elements of type Capability definitions in
      // the descriptor files

    };
  }
}

#include /**/ "ace/post.h"

#endif /* ID_HANDLER_H */
