// $Id$

#ifndef SPE_HANDLER_H
#define SPE_HANDLER_H
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
    class SPE_Handler
    {
    public:

      SPE_Handler (DOMDocument* doc, unsigned long filter_);

      SPE_Handler (DOMNodeIterator* iter, bool release = false);

      ~SPE_Handler();

      void process_SubcomponentPortEndpoint
      (::Deployment::SubcomponentPortEndpoint &spe);

      void process_portName (const XMLCh* name,
                             ::Deployment::SubcomponentPortEndpoint &spe);

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

#endif /* SPE_HANDLER_H */
