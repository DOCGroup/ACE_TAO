//==================================================================
/**
 *  @file  SR_Handler.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef SR_HANDLER_H
#define SR_HANDLER_H
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
     * @class SR_Handler
     *
     * @brief Handler class for <SharedResource> type 
     *
     * This class is within the Target Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the description files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export SR_Handler
    {
    public:

      /// constructor
      SR_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      SR_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~SR_Handler(void);

      /// Process the SharedResource type
      void process_sr (::Deployment::SharedResource &sr);

      /// Process the name attribute
      void process_name (const XMLCh* name, ::Deployment::SharedResource &sr);

      /// Process the resourceType attribute
      void process_resource_type (const XMLCh* resource_type,
                                  ::Deployment::SharedResource &sr);

      /// Process the nodeRef attribute
      void process_node_ref (const XMLCh* node_ref,
                             ::Deployment::SharedResource &sr);

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

#endif /* SR_HANDLER_H */
