// $Id$

#ifndef CAP_HANDLER_H
#define CAP_HANDLER_H
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

    class RS_Handler
    {
    public:

      /// constructor
      RS_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      RS_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~RS_Handler();

      /// Process the package configuration
      void process_RequirementSatisfier (::Deployment::RequirementSatisfier &rs);

      /// Process the label attribute
      void process_name (const XMLCh* name, ::Deployment::RequirementSatisfier &rs);

      /// Process the label attribute
      void process_resourceType (const XMLCh* name, ::Deployment::RequirementSatisfier &rs);

    private:

      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;

    };

    // TODO: This class suppose to derive drom RS_Handler?
    //       If not then process_name and process_resourceType should be
    //       defined here
    class Cap_Handler
    {
    public:

      /// constructor
      Cap_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      Cap_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~Cap_Handler();

      /// Process the package configuration
      void process_Capability (::Deployment::Capability &cap);

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

#endif /* CAP_HANDLER_H */
