//==================================================================
/**
 *  @file  CompIntrDesc_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef COMPINTRDESC_HANDLER_H
#define COMPINTRDESC_HANDLER_H
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

    class CompIntrDesc_Handler
    {
    public:

      /// constructor
      CompIntrDesc_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      CompIntrDesc_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~CompIntrDesc_Handler();

      /// Process the component package description
      void process_ComponentInterfaceDescription (::Deployment::ComponentInterfaceDescription &CompIntrDesc);

    protected:
      /// Process the label attribute
      void process_label (const XMLCh* label, ::Deployment::ComponentInterfaceDescription &compintrdesc);

      /// Process the UUID attribute
      void process_UUID (const XMLCh* UUID, ::Deployment::ComponentInterfaceDescription &compintrdesc);

      /// Process the specificType attribute
      void process_specificType (const XMLCh* specificType, ::Deployment::ComponentInterfaceDescription &compintrdesc);

      /// Process the supportedType attribute
      void process_supportedType (const XMLCh* supportedType, ::Deployment::ComponentInterfaceDescription &compintrdesc);

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

#endif /* COMPINTRDESC_HANDLER_H */
