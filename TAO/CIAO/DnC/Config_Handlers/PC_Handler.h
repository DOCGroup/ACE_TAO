//==================================================================
/**
 *  @file  PC_Handler.h
 *
 *  $Id$
 *
 *  @author George Edwards <g.edwards@vanderbilt.edu>
 */
//=====================================================================

#ifndef PC_HANDLER_H
#define PC_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
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
 * @class PC_Handler
 *
 * @brief Handler class for <PackageConfiguration> type 
 *
 * This class is within the Component Data Model subpackage of the
 * Deployment & Configuration package.
 *
 * This class defines handler methods to parse the aforementioned type
 * in the description files. The corresponding CORBA IDL type for this
 * element is returned.
 */

class Config_Handler_Export PC_Handler: public Basic_Handler
{
public:

  /// constructor
  PC_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  PC_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// Process the package configuration
  void process_PackageConfiguration (::Deployment::PackageConfiguration &pc);

  /// Process the specializedConfig attribute
  void process_specializedConfig
  (const XMLCh* specializedConfig, ::Deployment::PackageConfiguration &pc);

};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* PC_HANDLER_H */
