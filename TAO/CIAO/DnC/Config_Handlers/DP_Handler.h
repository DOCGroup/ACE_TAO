//==================================================================
/**
 *  @file  DP_Handler.h
 *
 *  $Id$
 *
 *  @author Aniruddha S. Gokhale <gokhale@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef CEPE_HANDLER_H
#define CEPE_HANDLER_H
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
     * @class DP_Handler
     *
     * @brief Handler class for <DeploymentPlanning> type 
     *
     * This class is within the Execution Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the descriptor files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export DP_Handler
    {
    public:

      /// constructor
      DP_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      DP_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~DP_Handler(void);

      /// Process the deployment plan
      void process_DeploymentPlan (::Deployment::DeploymentPlan &dp);

      /// Process the label attribute
      void process_label (const XMLCh* name, ::Deployment::DeploymentPlan &dp);

      /// Process the UUID attribute
      void process_UUID (const XMLCh* name, ::Deployment::DeploymentPlan &dp);

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

#endif /* DP_HANDLER_H */
