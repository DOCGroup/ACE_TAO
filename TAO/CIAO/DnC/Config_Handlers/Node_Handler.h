//==================================================================
/**
 *  @file  Node_Handler.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef NODE_HANDLER_H
#define NODE_HANDLER_H
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
     * @class Node_Handler
     *
     * @brief Handler class for <Node> type 
     *
     * This class is within the Target Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the description files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export Node_Handler
    {
    public:

      /// constructor
      Node_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      Node_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~Node_Handler(void);

      /// Process the Node type when the iterator is not passed
      void process_node (::Deployment::Node &node);

      /// Process the name attribute
      void process_name (const XMLCh* name, ::Deployment::Node &node);

      /// Process the label attribute
      void process_label (const XMLCh* label,
                          ::Deployment::Node &node);

      /// Process the sharedResourceRef attribute
      void process_shared_resource_ref (const XMLCh* shared_resource_ref,
                                        ::Deployment::Node &node);

      /// Process the connectionRef attribute
      void process_connection_ref (const XMLCh* connection_ref,
                                   ::Deployment::Node &node);

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

#endif /* NODE_HANDLER_H */
