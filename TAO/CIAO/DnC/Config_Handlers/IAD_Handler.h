//==================================================================
/**
 *  @file  IAD_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef IAD_HANDLER_H
#define IAD_HANDLER_H
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

class Config_Handler_Export IAD_Handler: public Basic_Handler
{
public:

  /// constructor
  IAD_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  IAD_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// Process the component package description
  void process_ImplementationArtifactDescription (::Deployment::ImplementationArtifactDescription &iad);

protected:

  /// Process the label attribute
  void process_label (const XMLCh* label, ::Deployment::ImplementationArtifactDescription &iad);

  /// Process the UUID attribute
  void process_UUID (const XMLCh* UUID, ::Deployment::ImplementationArtifactDescription &iad);

  /// Process the location attribute
  void process_location (const XMLCh* location, ::Deployment::ImplementationArtifactDescription &iad);
 
  /// process exec parameter element
  void process_exec_parameter_element (DOMNode* node,
                                       DOMDocument* doc, DOMNodeIterator* iter,
                                       Deployment::ImplementationArtifactDescription& iad);

  /// process info property element
  void process_info_property_element (DOMNode* node,
                                      DOMDocument* doc, DOMNodeIterator* iter,
                                      Deployment::ImplementationArtifactDescription& iad);

  /// process attributes for property element
  void process_attributes_for_property (DOMNamedNodeMap* named_node_map,
                                        DOMDocument* doc,
                                        DOMNodeIterator* iter,
                                        int value,
                                        Deployment::Property& ccd_property);

  /// parse a document
  DOMDocument* create_document (const char *url);

  /// process attributes for deploy requirement
  void process_attributes_for_deploy_requirement (DOMNamedNodeMap* nm,
                                                  DOMDocument* doc,
                                                  DOMNodeIterator* iter,
                                                  int value,
                                                  Deployment::Requirement& req);

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

#endif /* IAD_HANDLER_H */
