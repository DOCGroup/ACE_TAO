//==================================================================
/**
 *  @file  Cap_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */
//==================================================================

#ifndef CAP_HANDLER_H
#define CAP_HANDLER_H
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

/**
 * @class CAP_Handler
 *
 * @brief Handler class for <Capability> type
 *
 * This class defines handler methods to parse Capability
 * types in the descriptor files. The corresponding
 * CORBA IDL type for the schema element is returned.
 *
 */
class Config_Handler_Export CAP_Handler: public Basic_Handler
{
public:

  /// constructor
  CAP_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  CAP_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// constructor
  CAP_Handler (DOMDocument* doc, DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (doc, iter, release) { }

  void process_Capability (Deployment::Capability &ret_struct);
  // process elements of type Capability definitions in
  // the descriptor files
};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* ID_HANDLER_H */
