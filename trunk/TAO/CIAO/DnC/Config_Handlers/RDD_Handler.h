//==================================================================
/**
 *  @file  RDD_Handler.h
 *
 *  $Id$
 *
 *  @author Aniruddha S. Gokhale <gokhale@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef RDD_HANDLER_H
#define RDD_HANDLER_H
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
     * @class RDD_Handler
     *
     * @brief Handler class for <ResourceDeploymentDescriptor> type 
     *
     * This class is within the Execution Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the descriptor files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export RDD_Handler
    {
    public:

      /// constructor
      RDD_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      RDD_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~RDD_Handler(void);

      /// Process the resource deployment description
      void process_ResourceDeploymentDescription (::Deployment::ResourceDeploymentDescription &rdd);

      /// Process the requirement name attribute
      void process_requirementName (const XMLCh* name, ::Deployment::ResourceDeploymentDescription &rdd);

      /// Process the resource name attribute
      void process_resourceName (const XMLCh* name, ::Deployment::ResourceDeploymentDescription &rdd);

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

#endif /* RDD_HANDLER_H */
