//==================================================================
/**
 *  @file  NIA_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef NIA_HANDLER_H
#define NIA_HANDLER_H
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

    class NIA_Handler
    {
    public:

      /// constructor
      NIA_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      NIA_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~NIA_Handler();

      /// Process the package configuration
      void static 
	process_NamedImplementationArtifact (::Deployment::NamedImplementationArtifact &nia);

    protected:

      /// Process the name attribute
      void process_name (const XMLCh* name, ::Deployment::NamedImplementationArtifact &nia);

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

#endif /* NIA_HANDLER_H */
