//==================================================================
/**
 *  @file  Bridge_Handler.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef BRIDGE_HANDLER_H
#define BRIDGE_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
#include "Config_Handler_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include "XercesString.h"

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMText;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;

namespace CIAO 
{
  namespace Config_Handler
  {
    /**
     * @class Bridge_Handler
     *
     * @brief Handler class for <Bridge> type 
     *
     * This class is within the Target Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the description files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export Bridge_Handler
    {
    public:

      /// constructor
      Bridge_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      Bridge_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~Bridge_Handler(void);

      /// Process the Bridge type
      void process_bridge (::Deployment::Bridge &bridge);

      /// Process the name attribute
      void process_name (const XMLCh* name, ::Deployment::Bridge &bridge);

      /// Process the label attribute
      void process_label (const XMLCh* label,
                          ::Deployment::Bridge &bridge);

      /// Process the connectRef attribute
      void process_connect_ref (const XMLCh* connect_ref,
                                ::Deployment::Bridge &bridge);

    private:

      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;

    };
  };
};

#include /**/ "ace/post.h"

#endif /* BRIDGE_HANDLER_H */
