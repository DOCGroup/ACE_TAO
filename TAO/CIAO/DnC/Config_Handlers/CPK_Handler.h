//==================================================================
/**
 *  @file  CPK_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay <emre.turkay@vanderbilt.edu>
 */
//=====================================================================

#ifndef CPK_HANDLER_H
#define CPK_HANDLER_H
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
     * @class CPK_Handler
     *
     * @brief Handler class for <CCMComponentPortKind> type
     *
     * This class defines handler methods to parse Implementation
     * Dependency types in the descriptor files. The corresponding
     * CORBA IDL type for the schema element is returned.
     *
     */
    class Config_Handler_Export CPK_Handler
    {
    public:
      static void process_CCMComponentPortKind (DOMNodeIterator * iter,
                                                Deployment::CCMComponentPortKind &kind);
      // process elements of type CCMComponentPortKind definitions in
      // the descriptor files

    };
  }

}

#include /**/ "ace/post.h"

#endif /* CPK_HANDLER_H */
