//==================================================================
/**
 *  @file  CEPE_Handler.h
 *
 *  $Id$
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef CEPE_HANDLER_H
#define CEPE_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
#include "Config_Handler_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include "XercesString.h"

using Config_Handler::XStr;
using xercesc::XMLString;
using xercesc::DOMImplementation;
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
     * @brief Handler class for <ComponentExternalPortEndpoint> type
     *
     * This class defines handler methods to parse the aforementioned type
     * in the descriptor files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export CEPE_Handler
    {
    public:
      static void
        process_ComponentExternalPortEndpoint (DOMNodeIterator * iter,
                                               Deployment::ComponentExternalPortEndpoint &ret_struct);
      // process <ComponentExternalPortEndPoint> definitions in the descriptor
      // files

    };
  }

}

#include /**/ "ace/post.h"

#endif /* CEPE_HANDLER_H */
