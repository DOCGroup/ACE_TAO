//==================================================================
/**
 *  @file  Domain_Handler.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef DOMAIN_HANDLER_H
#define DOMAIN_HANDLER_H
#include /**/ "ace/pre.h"

#include "ciao/DeploymentC.h"
#include "Config_Handler_export.h"
#include "Basic_Handler.h"
#include "XercesString.h"

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
using xercesc::DOMNodeFilter;
using xercesc::DOMNamedNodeMap;

BEGIN_DEPLOYMENT_NAMESPACE

/**
 * @class Domain_Handler
 *
 * @brief Handler class for <Domain> type
 *
 * This class is within the Target Data Model subpackage of the
 * Deployment & Configuration package.
 *
 * This class defines handler methods to parse the aforementioned type
 * in the description files. The corresponding CORBA IDL type for this
 * element is returned.
 */

class Config_Handler_Export Domain_Handler: public Basic_Handler
{
public:

  /// constructor
  Domain_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  Domain_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// constructor
  Domain_Handler (DOMDocument* doc, DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (doc, iter, release) { }

  /// Process the Domain type
  void process_domain (Deployment::Domain& domain);

  /// process the node attribute
  void process_node (DOMNodeIterator* iter,
                     Deployment::Node& domain_node);

  /// process the node attribute
  void process_bridge (DOMNodeIterator* iter,
                       Deployment::Bridge& domain_bridge);

  /// process the node attribute
  void process_interconnect (DOMNodeIterator* iter,
                             Deployment::Interconnect& domain_ic);

  /// process the resource attribute
  void process_resource (DOMNodeIterator* iter,
                         Deployment::Resource& domain_resource);

  /// process the node attribute
  void process_sr (DOMNodeIterator* iter,
                   Deployment::SharedResource& domain_sr);

  /// update node refs
  void update_node_refs (Deployment::Domain& domain);

  /// update ic refs
  void update_ic_refs (Deployment::Domain& domain);

  /// update bridge refs
  void update_bridge_refs (Deployment::Domain& domain);

  /// update sr refs
  void update_sr_refs (Deployment::Domain& domain);

  /// process IDREFs
  void process_refs (DOMNamedNodeMap* named_node_map);

};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* DOMAIN_HANDLER_H */
