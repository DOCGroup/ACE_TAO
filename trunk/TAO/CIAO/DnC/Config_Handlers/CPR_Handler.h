//==================================================================
/**
 *  @file  CPR_Handler.h
 *
 *  $Id$
 *
 *  @author Aniruddha S. Gokhale <gokhale@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef CPR_HANDLER_H
#define CPR_HANDLER_H
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

#include "Basic_Handler.h"

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
 * @class CPR_Handler
 *
 * @brief Handler class for <ComponentPackageReference> type 
 *
 * This class is within the Execution Data Model subpackage of the
 * Deployment & Configuration package.
 *
 * This class defines handler methods to parse the aforementioned type
 * in the descriptor files. The corresponding CORBA IDL type for this
 * element is returned.
 */

class Config_Handler_Export CPR_Handler: public Basic_Handler
{
public:

  /// constructor
  CPR_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  CPR_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// Process the component package reference
  void process_ComponentPackageReference (::Deployment::ComponentPackageReference &cpr);

  /// Process the UUID attribute
  void process_requiredUUID (const XMLCh* UUID, ::Deployment::ComponentPackageReference &cpr);

  /// Process the requirement name attribute
  void process_requiredName (const XMLCh* name, ::Deployment::ComponentPackageReference &cpr);

  /// Process the required type attribute
  void process_requiredType (const XMLCh* type, ::Deployment::ComponentPackageReference &cpr);

};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* CPR_HANDLER_H */
