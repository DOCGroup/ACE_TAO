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

      /// process the shared resource element
      void process_sr_element (DOMNode* node,
                               DOMDocument* doc,
                               DOMNodeIterator* iter,
                               Deployment::Domain& domain);
      /*
      /// process the shared resource element
      void process_node_element (DOMNode* node,
                                 DOMDocument* doc,
                                 DOMNodeIterator* iter,
                                 Deployment::Domain& domain);
      */
      /// process the shared resource element
      void process_ic_element (DOMNode* node,
                               DOMDocument* doc,
                               DOMNodeIterator* iter,
                               Deployment::Domain& domain);

      /// process the shared resource element
      void process_bridge_element (DOMNode* node,
                                   DOMDocument* doc,
                                   DOMNodeIterator* iter,
                                   Deployment::Domain& domain);

      /// process the shared resource element
      void process_property_element (DOMNode* node,
                                     DOMDocument* doc,
                                     DOMNodeIterator* iter,
                                     Deployment::Domain& domain);

      /// Process the attributes for the shared resource
      void process_attributes_for_sr (DOMNamedNodeMap* named_node_map,
                                      DOMDocument* doc,
                                      DOMNodeIterator* iter,
                                      int length,
                                      Deployment::SharedResource& domain_sr);
      /*
      /// Process the attributes for the Node
      void process_attributes_for_node (DOMNamedNodeMap* named_node_map,
                                        DOMDocument* doc,
                                        DOMNodeIterator* iter,
                                        int length,
                                        Deployment::Node& domain_node);
      */
      /// Process the attributes for the interconnect
      void process_attributes_for_ic (DOMNamedNodeMap* named_node_map,
                                      DOMDocument* doc,
                                      DOMNodeIterator* iter,
                                      int length,
                                      Deployment::Interconnect& domain_ic);

      /// Process the attributes for the bridge
      void process_attributes_for_bridge (DOMNamedNodeMap* named_node_map,
                                          DOMDocument* doc,
                                          DOMNodeIterator* iter,
                                          int length,
                                          Deployment::Bridge& domain_bridge);

      /// Process the attributes for the resource
      void process_attributes_for_resource (DOMNamedNodeMap* named_node_map,
                                            DOMDocument* doc,
                                            DOMNodeIterator* iter,
                                            int length,
                                            Deployment::Resource& res);

      /// Process the attributes for the property
      void process_attributes_for_property (DOMNamedNodeMap* named_node_map,
                                            DOMDocument* doc,
                                            DOMNodeIterator* iter,
                                            int length,
                                            Deployment::Property& pro);

      /// Process the attributes for the property
      void process_attributes_for_satisfier_property (DOMNamedNodeMap* named_node_map,
                                                      DOMDocument* doc,
                                                      DOMNodeIterator* iter,
                                                      int length,
                                                      Deployment::SatisfierProperty& pro);

      /// process the node attribute
      void process_node (DOMNodeIterator*& iter,
                         Deployment::Node& domain_node);

      /// process the node attribute
      void process_bridge (DOMDocument* doc,
                           DOMNodeIterator* iter,
                           Deployment::Bridge& domain_bridge);

      /// process the node attribute
      void process_interconnect (DOMDocument* doc,
                                 DOMNodeIterator* iter,
                                 Deployment::Interconnect& domain_ic);

      /// process the resource attribute
      void process_resource (DOMDocument* doc,
                             DOMNodeIterator* iter,
                             Deployment::Resource& domain_resource);

      /// process the node attribute
      void process_sr (DOMDocument* doc,
                       DOMNodeIterator* iter,
                       Deployment::SharedResource& domain_sr);

      /// process the Node name attribute
      void process_node_name (const XMLCh* name,
                              Deployment::Node& domain_node);

      /// process the Interconnect name attribute
      void process_ic_name (const XMLCh* name,
                            Deployment::Interconnect& domain_ic);

      /// process the Bridge name attribute
      void process_bridge_name (const XMLCh* name,
                                Deployment::Bridge& domain_bridge);

      /// process the SharedResource name attribute
      void process_sr_name (const XMLCh* name,
                            Deployment::SharedResource& domain_sr);

      /// process the Node label attribute
      void process_node_label (const XMLCh* label,
                               Deployment::Node& domain_node);

      /// process the Bridge label attribute
      void process_bridge_label (const XMLCh* label,
                                 Deployment::Bridge& domain_bridge);

      /// process the Interconnect label attribute
      void process_ic_label (const XMLCh* label,
                             Deployment::Interconnect& domain_ic);

      /// process the Node name attribute
      void process_sr_resource_type (const XMLCh* resource_type,
                                     Deployment::SharedResource& domain_sr);

      /// create a document
      DOMDocument* create_document (const char *url);

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
