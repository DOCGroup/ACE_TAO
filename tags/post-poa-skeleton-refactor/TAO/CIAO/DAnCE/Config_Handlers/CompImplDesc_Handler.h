//==================================================================
/**
 *  @file  CompImplDesc_Handler.h
 *
 *  $Id$
 *
 *  @author Aniruddha S. Gokhale <gokhale@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef CompImplDesc_HANDLER_H
#define CompImplDesc_HANDLER_H
#include /**/ "ace/pre.h"

#include "ciao/DeploymentC.h"
#include "Config_Handler_export.h"
#include "Basic_Handler.h"

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

BEGIN_DEPLOYMENT_NAMESPACE

/**
 * @class CompImplDesc_Handler
 *
 * @brief Handler class for <ComponentImplementationDescription> type
 *
 * This class is within the Execution Data Model subpackage of the
 * Deployment & Configuration package.
 *
 * This class defines handler methods to parse the aforementioned type
 * in the description files. The corresponding CORBA IDL type for this
 * element is returned.
 */

class Config_Handler_Export CompImplDesc_Handler: public Basic_Handler
{
 public:

  /// constructor
  CompImplDesc_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  CompImplDesc_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// constructor
  CompImplDesc_Handler (DOMDocument* doc,
                        DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (doc, iter, release) { }

  /// Process the monolithic deployment description
  void process_ComponentImplementationDescription (::Deployment::ComponentImplementationDescription &cid);

};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* CompImplDesc_HANDLER_H */
