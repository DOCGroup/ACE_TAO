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

typedef ACE_Hash_Map_Manager<ACE_TString, int, ACE_SYNCH_RW_MUTEX> REF_MAP;
typedef ACE_Hash_Map_Iterator<ACE_TString, int, ACE_SYNCH_RW_MUTEX> REF_ITER;
typedef ACE_Hash_Map_Manager<int, ACE_TString, ACE_SYNCH_RW_MUTEX> IDREF_MAP;

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

      /// Process the UUID attribute
      void process_uuid (const XMLCh* uuid,
                         Deployment::Domain& domain);

      /// Process the label attribute
      void process_label (const XMLCh* label,
                          Deployment::Domain& domain);

      /// process the node attribute
      void process_domain_node (DOMDocument* doc,
                                DOMNodeIterator* iter,
                                Deployment::Node& domain_node);

      /// process the node attribute
      void process_domain_bridge (DOMDocument* doc,
                                  DOMNodeIterator* iter,
                                  Deployment::Bridge& domain_bridge);

      /// process the node attribute
      void process_domain_interconnect (DOMDocument* doc,
                                        DOMNodeIterator* iter,
                                        Deployment::Interconnect& domain_ic);

      /// process the node attribute
      void process_domain_sr (DOMDocument* doc,
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

      /// dump the XML descriptors
      void dump (Deployment::Domain& domain);

      /// create a document
      DOMDocument* create_document (const char *url);

      /// parse the resource in a HREF format
      void parse_resource_href_doc (DOMDocument* doc, unsigned long filter,
                                    Deployment::Resource& domain_resource);

      /// parse the property in a HREF format
      void parse_property_href_doc (DOMDocument* doc, unsigned long filter,
                                    Deployment::Property& property);

      /// parse the satisfier property in a HREF format
      void parse_satisfier_property_href_doc (DOMDocument* doc, unsigned long filter,
                                              Deployment::SatisfierProperty& satisfier_property);

    private:

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
