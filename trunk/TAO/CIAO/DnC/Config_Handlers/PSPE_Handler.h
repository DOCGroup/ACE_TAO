//==================================================================
/**
 *  @file  PSPE_Handler.h
 *
 *  $Id$
 *
 *  @author Aniruddha S. Gokhale <gokhale@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef PSPE_HANDLER_H
#define PSPE_HANDLER_H
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
     * @class PSPE_Handler
     *
     * @brief Handler class for <PlanSubcomponentPortEndpoint> type 
     *
     * This class is within the Execution Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the description files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export PSPE_Handler
    {
    public:

      /// constructor
      PSPE_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      PSPE_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~PSPE_Handler(void);

      /// Process the instance deployment description
      void process_PlanSubcomponentPortEndpoint (::Deployment::PlanSubcomponentPortEndpoint &pspe);

      /// Process the port name attribute
      void process_portName (const XMLCh* name, ::Deployment::PlanSubcomponentPortEndpoint &pspe);

      /// Process the providerattribute
      void process_provider (const XMLCh* provider, ::Deployment::PlanSubcomponentPortEndpoint &pspe);

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

#endif /* PSPE_HANDLER_H */
