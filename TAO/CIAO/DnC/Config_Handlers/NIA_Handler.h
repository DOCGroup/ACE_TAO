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

class Config_Handler_Export NIA_Handler: public Basic_Handler
{
public:

  /// constructor
  NIA_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  NIA_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// Process the package configuration
  void process_NamedImplementationArtifact (::Deployment::NamedImplementationArtifact &nia);
};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* NIA_HANDLER_H */
