// $Id$

#ifndef MID_HANDLER_H
#define MID_HANDLER_H
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
    class MID_Handler
    {
    public:

      MID_Handler (DOMDocument* doc, unsigned long filter_);

      MID_Handler (DOMNodeIterator* iter, bool release = false);

      ~MID_Handler();

      void process_MonolithicImplementationDescription (::Deployment::MonolithicImplementationDescription &mid);

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

    private:

      typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_Null_Mutex> REF_MAP;
      typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_Null_Mutex> REF_ITER;
      typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_Null_Mutex> IDREF_MAP;
      DOMDocumentTraversal* traverse_;

      DOMDocument* doc_;

      DOMNode* root_;

      unsigned long filter_;

      DOMNodeIterator* iter_;

      bool release_;
      int index_;
      REF_MAP id_map_;
      IDREF_MAP idref_map_;

    };
  }
}

#include /**/ "ace/post.h"

#endif /* MID_HANDLER_H */
