// $Id$

#ifndef APM_HANDLER_H
#define APM_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"

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
    class APM_Handler
    {
    public:

      APM_Handler (DOMDocument* doc, unsigned long filter_);

      APM_Handler (DOMNodeIterator* iter, bool release = false);

      ~APM_Handler();

      void process_AssemblyPropertyMapping
      (::Deployment::AssemblyPropertyMapping &apm);

      void process_name (const XMLCh* name,
      ::Deployment::AssemblyPropertyMapping &apm);

      void process_externalName (const XMLCh* name,
      ::Deployment::AssemblyPropertyMapping &apm);

    private:

      DOMDocumentTraversal* traverse_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;

    };
  }
}

#include /**/ "ace/post.h"

#endif /* APM_HANDLER_H */
