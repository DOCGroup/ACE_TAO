//==================================================================
/**
 *  @file  CompPropDesc_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef COMPPROPDESC_HANDLER_H
#define COMPPROPDESC_HANDLER_H
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

    class CompPropDesc_Handler
    {
    public:

      /// constructor
      CompPropDesc_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      CompPropDesc_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~CompPropDesc_Handler();

      /// Process the package configuration
      void process_ComponentPropertyDescription (::Deployment::ComponentPropertyDescription &comppropdesc);

      /// Process the label attribute
      void process_name (const XMLCh* name, ::Deployment::ComponentPropertyDescription &comppropdesc);

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

#endif /* COMPPROPDESC_HANDLER_H */
