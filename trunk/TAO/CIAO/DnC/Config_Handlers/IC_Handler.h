//==================================================================
/**
 *  @file  IC_Handler.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef IC_HANDLER_H
#define IC_HANDLER_H
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
     * @class IC_Handler
     *
     * @brief Handler class for <Interconnect> type 
     *
     * This class is within the Target Data Model subpackage of the
     * Deployment & Configuration package.
     *
     * This class defines handler methods to parse the aforementioned type
     * in the description files. The corresponding CORBA IDL type for this
     * element is returned.
     */

    class Config_Handler_Export IC_Handler
    {
    public:

      /// constructor
      IC_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      IC_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~IC_Handler(void);

      /// Process the Interconnect type when the iterator is not passed
      void process_ic (::Deployment::Interconnect &ic);

      /// Process the name attribute
      void process_name (const XMLCh* name, ::Deployment::Interconnect &ic);

      /// Process the label attribute
      void process_label (const XMLCh* label,
                          ::Deployment::Interconnect &ic);

      /// Process the connectRef attribute
      void process_connect_ref (const XMLCh* connect_ref,
                                ::Deployment::Interconnect &ic);

      /// Process the connectionRef attribute
      void process_connection_ref (const XMLCh* connection_ref,
                                   ::Deployment::Interconnect &ic);

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

#endif /* IC_HANDLER_H */
