//==================================================================
/**
 *  @file  IAD_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef IAD_HANDLER_H
#define IAD_HANDLER_H
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

      class IAD_Handler
        {
        public:

          /// constructor
          IAD_Handler (DOMDocument* doc, unsigned long filter_);

          /// constructor
          IAD_Handler (DOMNodeIterator* iter, bool release = false);

          /// destructor
          ~IAD_Handler();

          /// Process the component package description
          void process_ImplementationArtifactDescription (::Deployment::ImplementationArtifactDescription &iad);

        protected:

          /// Process the label attribute
          void process_label (const XMLCh* label, ::Deployment::ImplementationArtifactDescription &iad);

          /// Process the UUID attribute
          void process_UUID (const XMLCh* UUID, ::Deployment::ImplementationArtifactDescription &iad);

          /// Process the location attribute
          void process_location (const XMLCh* location, ::Deployment::ImplementationArtifactDescription &iad);

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

#endif /* IAD_HANDLER_H */
