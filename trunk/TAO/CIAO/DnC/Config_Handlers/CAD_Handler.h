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

namespace CIAO 
{
  namespace Config_Handler
  {
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

    class Config_Handler_Export CAD_Handler
    {
    public:

      /// constructor
      CAD_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      CAD_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~CAD_Handler(void);

      /// Process the component assembly description
      void process_ComponentAssemblyDescription (::Deployment::ComponentAssemblyDescription &cad);
  
      /// process the instance element
      void process_instance_element (DOMNode* node,
         DOMDocument* doc,
         DOMNodeIterator* iter,
         Deployment::ComponentAssemblyDescription& cad);

      /// process the connection element
      void process_connection_element (DOMNode* node,
         DOMDocument* doc,
         DOMNodeIterator* iter,
         Deployment::ComponentAssemblyDescription& cad);

      /// process the property element
      void process_property_element (DOMNode* node,
         DOMDocument* doc,
         DOMNodeIterator* iter,
         Deployment::ComponentAssemblyDescription& cad);

      /// process instance
      void process_instance (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::SubcomponentInstantiationDescription& sid);

      /// process instance name
      void process_instance_name (const XMLCh* name,
        Deployment::SubcomponentInstantiationDescription& sid);

      /// process attributes for instance
      void process_attributes_for_instance (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::SubcomponentInstantiationDescription& sid);

      /// process attributes for reference
      void process_attributes_for_reference (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::ComponentPackageReference& sid_ref);

      /// process attributes for package
      void process_attributes_for_package (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::ComponentPackageDescription& sid_package);

      /// process attributes for config property
      void process_attributes_for_config_property (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::Property& config_property);

      /// process attributes for requirement
      void process_attributes_for_req (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::Requirement& req);

      /// process connection element
      void process_connection (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::AssemblyConnectionDescription& acd);

      /// process attributes for connection
      void process_attributes_for_connection (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::AssemblyConnectionDescription& acd);

      /// process property element
      void process_property (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::AssemblyPropertyMapping& acd);

      /// process attributes for property
      void process_attributes_for_property (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::AssemblyPropertyMapping& acd);

      /// create a document from a URI
      DOMDocument* create_document (const char *url);

      /// process CPD
      void process_package (DOMDocument* doc,
        DOMNodeIterator* iter,
        Deployment::ComponentPackageDescription& sid_package);

     /// process CPR
     void process_reference (DOMDocument* doc,
       DOMNodeIterator* iter,
        Deployment::ComponentPackageReference& sid_ref);

     /// process CPR UUID
     void process_ref_uuid (const XMLCh* name,
       Deployment::ComponentPackageReference& sid_ref);

     /// process CPR name
     void process_ref_name (const XMLCh* name,
       Deployment::ComponentPackageReference& sid_ref);

     /// process CPR type
     void process_ref_type (const XMLCh* name,
       Deployment::ComponentPackageReference& sid_ref);

    private:

      typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
      typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_Null_Mutex> REF_ITER;
      typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_Null_Mutex> IDREF_MAP;
      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;

      int index_;

      REF_MAP id_map_;
      IDREF_MAP idref_map_;

    };
  };
};

#include /**/ "ace/post.h"

#endif /* CAD_HANDLER_H */
