//==================================================================
/**
 *  @file  IDD_Handler.h
 *
 *  $Id$
 *
 *  @author Aniruddha S. Gokhale <gokhale@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef IDD_HANDLER_H
#define IDD_HANDLER_H
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
     * @class IDD_Handler
     *
     * @brief Handler class for <InstanceDeploymentDescription> type 
     *
     * This class is within the Execution Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the description files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export IDD_Handler
    {
    public:

      /// constructor
      IDD_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      IDD_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~IDD_Handler(void);

      /// Process the instance deployment description
      void process_InstanceDeploymentDescription (::Deployment::InstanceDeploymentDescription &idd);

      /// Process the name attribute
      void process_name (const XMLCh* name, ::Deployment::InstanceDeploymentDescription &idd);

      /// Process the node attribute
      void process_node (const XMLCh* node, ::Deployment::InstanceDeploymentDescription &idd);

      /// Process the source attribute
      void process_source (const XMLCh* source, ::Deployment::InstanceDeploymentDescription &idd);

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

#endif /* IDD_HANDLER_H */
