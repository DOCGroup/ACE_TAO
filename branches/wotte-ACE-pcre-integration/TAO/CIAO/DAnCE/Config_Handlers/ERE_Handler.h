
//==================================================================
/**
 *  @file  ERE_Handler.h
 *
 *  $Id$
 *
 *  @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef ERE_HANDLER_H
#define ERE_HANDLER_H
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
     * @class ERE_Handler
     *
     * @brief Handler class for <ExternalReferenceEndpoint> tag
     *
     * This class defines handler methods to parse the aforementioned type
     * in the descriptor files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export ERE_Handler
    {
    public:
      static void  
        process_ExternalReferenceEndpoint (DOMNodeIterator * iter,
                                           Deployment::ExternalReferenceEndpoint &ret_struct);
      // process <ExternalReferenceEndpoint> definitions in the descriptor
      // files

    };
  }
}

#include /**/ "ace/post.h"

#endif /* ERE_HANDLER_H */
