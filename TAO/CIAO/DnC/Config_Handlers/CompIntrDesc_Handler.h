//==================================================================
/**
 *  @file  CCD_Handler.h
 *
 *  $Id$
 *
 *  @author Emre Turkay  <turkaye@dre.vanderbilt.edu>
 */
//=====================================================================

#ifndef CCD_HANDLER_H
#define CCD_HANDLER_H
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

    class CCD_Handler
    {
    public:

      /// constructor
      CCD_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      CCD_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~CCD_Handler();

      /// Process the component package description
      void process_ComponentInterfaceDescription (::Deployment::ComponentInterfaceDescription &CompIntrDesc);

    protected:
      /// Process the label attribute
      void process_label (const XMLCh* label,
        Deployment::ComponentInterfaceDescription &compintrdesc);

      /// Process the UUID attribute
      void process_UUID (const XMLCh* UUID,
        Deployment::ComponentInterfaceDescription &compintrdesc);

      /// Process the specificType attribute
      void process_specificType (const XMLCh* specificType,
        Deployment::ComponentInterfaceDescription &compintrdesc);

      /// Process the supportedType attribute
      void process_supportedType (const XMLCh* supportedType,
        Deployment::ComponentInterfaceDescription &compintrdesc);

      /// process config property element
      void process_config_property_element (DOMNode* node,
        DOMDocument* doc, DOMNodeIterator* iter,
        Deployment::ComponentInterfaceDescription& cid);

      /// process info property element
      void process_info_property_element (DOMNode* node,
        DOMDocument* doc, DOMNodeIterator* iter,
        Deployment::ComponentInterfaceDescription& cid);

      /// process attributes for property
      void process_attributes_for_property (DOMNamedNodeMap* named_node_map,
        DOMDocument* doc,
        DOMNodeIterator* iter,
        int value,
        Deployment::Property& ccd_property);

      /// create a XML document
      DOMDocument* create_document (const char *url);

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

#endif /* CCD_HANDLER_H */
