//==================================================================
/**
 *  @file  CAD_Handler.h
 *
 *  $Id$
 *
 *  @author Aniruddha S. Gokhale <gokhale@dre.vanderbilt.edu>
 */
//=====================================================================
#ifndef CAD_HANDLER_H
#define CAD_HANDLER_H
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
 * @class CAD_Handler
 *
 * @brief Handler class for <ComponentAssemblyDescription> type 
 *
 * This class is within the Component Data Model subpackage of the
 * Deployment & Configuration package.
 *
 * This class defines handler methods to parse the aforementioned type
 * in the descriptor files. The corresponding CORBA IDL type for this
 * element is returned.
 */

class Config_Handler_Export CAD_Handler: public Basic_Handler
{
public:

  /// constructor
  CAD_Handler (DOMDocument* doc, unsigned long filter_)
    : Basic_Handler (doc, filter_) { }

  /// constructor
  CAD_Handler (DOMNodeIterator* iter, bool release = false)
    : Basic_Handler (iter, release) { }

  /// Process the component assembly description
  void process_ComponentAssemblyDescription (::Deployment::ComponentAssemblyDescription &cad);
  
  /// process instance
  void process_instance (DOMNodeIterator* iter,
                         Deployment::SubcomponentInstantiationDescription& sid);

  /// process connection element
  void process_connection (DOMNodeIterator* iter,
                           Deployment::AssemblyConnectionDescription& acd);

  /// process property element
  void process_property (DOMNodeIterator* iter,
                         Deployment::AssemblyPropertyMapping& acd);

  /// process spe element
  void process_spe (DOMNodeIterator* iter,
                    Deployment::SubcomponentPortEndpoint& spe);

  /// process spr element
  void process_spr (DOMNodeIterator* iter,
                    Deployment::SubcomponentPropertyReference& spr);

  /// process CPD
  void process_package (DOMNodeIterator* iter,
                        Deployment::ComponentPackageDescription& sid_package);

  /// process CPR
  void process_reference (DOMNodeIterator* iter,
                          Deployment::ComponentPackageReference& sid_ref);

  /// update spe refs
  void update_spe_refs (Deployment::ComponentAssemblyDescription& cad);

  /// update spr refs
  void update_spr_refs (Deployment::ComponentAssemblyDescription& cad);

};

END_DEPLOYMENT_NAMESPACE

#include /**/ "ace/post.h"

#endif /* CAD_HANDLER_H */
