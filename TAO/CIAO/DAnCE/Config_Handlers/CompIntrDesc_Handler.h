//$Id$

#ifndef COMPINTRDESC_HANDLER_H
#define COMPINTRDESC_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
#include "Config_Handler_export.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "tao/Exception.h"
#include "XercesString.h"
#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLURL.hpp>
#include <xercesc/util/XMLUri.hpp>

#include "Basic_Handler.h"

using Config_Handler::XStr;
using xercesc::XMLUni;
using xercesc::XMLUri;
using xercesc::XMLURL;
using xercesc::XMLString;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::DOMBuilder;
using xercesc::DOMImplementationRegistry;
using xercesc::DOMImplementationLS;
using xercesc::DOMImplementation;
using xercesc::DOMInputSource;
using xercesc::DOMText;
using xercesc::DOMNamedNodeMap;
using xercesc::DOMLocator;
using xercesc::DOMError;
using xercesc::DOMDocument;
using xercesc::DOMDocumentTraversal;
using xercesc::DOMNodeIterator;
using xercesc::DOMNode;
using xercesc::DOMNodeFilter;
using xercesc::DOMNamedNodeMap;

BEGIN_DEPLOYMENT_NAMESPACE

class Config_Handler_Export CompIntrDesc_Handler: public Basic_Handler
{
public:

  /// constructor
  CompIntrDesc_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  CompIntrDesc_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { };

  /// constructor
  CompIntrDesc_Handler (DOMDocument* doc, 
                        DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (doc, iter, release) { };

  /// Process the component package description
  void process_ComponentInterfaceDescription (::Deployment::ComponentInterfaceDescription &CompIntrDesc);

protected:
  /// process port element
  void process_port (DOMNodeIterator* iter,
                     Deployment::ComponentPortDescription& port);

  /// process component property element
  void process_comp_property (DOMNodeIterator* iter,
                              Deployment::ComponentPropertyDescription& property);
};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif // COMPINTRDESC_HANDLER_H
