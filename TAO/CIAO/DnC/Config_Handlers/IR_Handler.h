//==================================================================
/**
 *  @file  IR_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef IR_HANDLER_H
#define IR_HANDLER_H
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
    class ResourceUsageKind_Handler {
    public:

      /// Process elements of type ResourceUsageKind definitions in
      /// the description files.
      static void process_ResourceUsageKind (DOMNodeIterator *iter,
					     ::Deployment::ResourceUsageKind &ruq);
    };

    class Req_Handler
    {
    public:

      /// constructor
      Req_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      Req_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~Req_Handler();

      /// Process the package configuration
      void process_Requirement (::Deployment::Requirement &req);

      /// Process the label attribute
      void process_name (const XMLCh* name, ::Deployment::Requirement &req);

      /// Process the label attribute
      void process_resourceType (const XMLCh* name, ::Deployment::Requirement &req);

    private:

      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;

    };

    // TODO: This class suppose to derive drom Req_Handler?
    class IR_Handler
    {
    public:

      /// constructor
      IR_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      IR_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~IR_Handler();

      /// Process the package configuration
      void process_ImplementationRequirement (::Deployment::ImplementationRequirement &ir);

      /// Process the resourceUsage attribute
      void process_resourceUsage (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

      /// Process the resourcePort attribute
      void process_resourcePort (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

      /// Process the componentPort attribute
      void process_componentPort (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

    private:

      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;

    };

  }

}

#include /**/ "ace/post.h"

#endif /* IR_HANDLER_H */
