//==================================================================
/**
 *  @file  IR_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef IR_HANDLER_H
#define IR_HANDLER_H
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
#include "RUK_Handler.h"

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

class Config_Handler_Export IR_Handler: public Basic_Handler
{
public:

  /// constructor
  IR_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  IR_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// Process the package configuration
  void process_ImplementationRequirement (::Deployment::ImplementationRequirement &ir);

protected:
  /// Process the resourceUsage attribute
  void process_resourceUsage (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

  /// Process the resourcePort attribute
  void process_resourcePort (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

  /// Process the componentPort attribute
  void process_componentPort (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

  /*
   * Derived from Requirement
   */

  /// Process the label attribute
  void process_name (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

  /// Process the label attribute
  void process_resourceType (const XMLCh* name, ::Deployment::ImplementationRequirement &ir);

};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* IR_HANDLER_H */
