// $Id$

#ifndef MID_HANDLER_H
#define MID_HANDLER_H
#include /**/ "ace/pre.h"

#include "DeploymentC.h"
#include "Config_Handler_export.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "Basic_Handler.h"

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

class Config_Handler_Export MID_Handler: public Basic_Handler
{
public:

  MID_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  MID_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  MID_Handler (DOMDocument* doc, DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (doc, iter, release) { }

  void process_MonolithicImplementationDescription (::Deployment::MonolithicImplementationDescription &mid);

protected:
  /// process exec parameter element
  void process_exec_parameter_element (DOMNode* node,
                                       DOMDocument* doc, DOMNodeIterator* iter,
                                       Deployment::MonolithicImplementationDescription& mid);

  /// process attributes for property element
  void process_attributes_for_property (DOMNamedNodeMap* named_node_map,
                                        DOMDocument* doc,
                                        DOMNodeIterator* iter,
                                        int value,
                                        Deployment::Property& ccd_property);

  /// process attributes for deploy requirement
  void process_attributes_for_deploy_requirement (DOMNamedNodeMap* nm,
                                                  DOMDocument* doc,
                                                  DOMNodeIterator* iter,
                                                  int value,
                                                  Deployment::ImplementationRequirement& req);

  /// process attributes for NIA
  void process_attributes_for_nia (DOMNamedNodeMap* nm,
                                   DOMDocument* doc,
                                   DOMNodeIterator* iter,
                                   int value,
                                   Deployment::NamedImplementationArtifact& nia);

  /// process IDREFS
  void process_refs (DOMNamedNodeMap* named_node_map);

};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* MID_HANDLER_H */
