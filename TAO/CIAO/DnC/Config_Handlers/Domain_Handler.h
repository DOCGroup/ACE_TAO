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

    class Config_Handler_Export Domain_Handler
    {
    public:

      /// constructor
      Domain_Handler (DOMDocument* doc, unsigned long filter_);

      /// constructor
      Domain_Handler (DOMNodeIterator* iter, bool release = false);

      /// destructor
      ~Domain_Handler(void);

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
  };
};

#include /**/ "ace/post.h"

#endif /* DOMAIN_HANDLER_H */
