//==================================================================
/**
 *  @file  CompPortDesc_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef COMPPORTDESC_HANDLER_H
#define COMPPORTDESC_HANDLER_H
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

      class Config_Handler_Export CompPortDesc_Handler
        {
        public:

          /// constructor
          CompPortDesc_Handler (DOMDocument* doc, unsigned long filter_);

          /// constructor
          CompPortDesc_Handler (DOMNodeIterator* iter, bool release = false);

          /// destructor
          ~CompPortDesc_Handler();

          /// Process the package configuration
          void process_ComponentPortDescription (::Deployment::ComponentPortDescription &compportdesc);

        protected:
          /// Process the label attribute
          void process_name (const XMLCh* name, ::Deployment::ComponentPortDescription &compportdesc);

          /// Process the specificType attribute
          void process_specificType (const XMLCh* specificType, ::Deployment::ComponentPortDescription &compportdesc);

          /// Process the supportedType attribute
          void process_supportedType (const XMLCh* supportedType, ::Deployment::ComponentPortDescription &compportdesc);

          /// Process the provider attribute
          void process_provider (const XMLCh* provider, ::Deployment::ComponentPortDescription &compportdesc);

          /// Process the exclusiveProvider attribute
          void process_exclusiveProvider (const XMLCh* exclusiveProvider, ::Deployment::ComponentPortDescription &compportdesc);

          /// Process the exclusiveUser attribute
          void process_exclusiveUser (const XMLCh* name, ::Deployment::ComponentPortDescription &compportdesc);

          /// Process the optional attribute
          void process_optional (const XMLCh* name, ::Deployment::ComponentPortDescription &compportdesc);

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

#endif /* COMPPORTDESC_HANDLER_H */
